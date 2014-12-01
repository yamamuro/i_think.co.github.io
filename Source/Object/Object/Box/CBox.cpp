#include "stdafx.hpp"

#include "../../ObjectBase/CObjectBase.hpp"
#include "../Object/Player/Player/CPlayer.hpp"
#include "CBox.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CBox::CBox( void )
	: m_MovablePower( 0 )
	, m_isJoint( false )
{

	m_FilePass += "Object/Object/Box/Box.x";
	// メッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( DIRECTX->GetDevice() , m_FilePass );

	// マテリアル情報を保存する
	m_Material = m_lpXFile->GetMaterial();

	// アクターデータ
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_BOX;
	m_ActorData.Dimension = NxVec3( 3.7f , 3.7f , 3.7f );	// 大きさ
	m_ActorData.FixFlag = false;
	m_ActorData.AngularDamping = 0.0f;						// 転がりやすさ
	m_ActorData.Density = 80000000000.0f;					// 密度
	
}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CBox::DiscreteInit( void )
{
	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_HEAVY_OBJECT );
	m_Mass = m_Actor->getMass();
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CBox::~CBox( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CBox::Exec( void )
{

	// 座標を取得
	D3DXMATRIXA16 mWorld;
	CDPConverter::SetMatrix( &mWorld , m_Actor );
	m_vPos.x = mWorld._41;
	m_vPos.y = mWorld._42;
	m_vPos.z = mWorld._43;

	// レーザーヒット時エネルギーを貯める
	AddAntiGravity_Energy( 30 );
	

	if( m_AntiGravity_Energy )
	{


		NxVec3 UpVec( 0.0f , 0.1f , 0.0f ); 
		// 質量を軽くする
		if( !m_isJoint )
		{
			m_Actor->setMass( m_Mass / 10000 );
			m_Actor->raiseBodyFlag( NX_BF_DISABLE_GRAVITY );
		}

		
		// 上方向へのベクトルをセットし浮遊させる
		m_Actor->setLinearVelocity( UpVec * ( ( m_AntiGravity_Energy/(float)MaxEnergy ) ) * 200.0f );
		
		// アクターに回転を加える
		m_Actor->addTorque( NxVec3( 6.0f , 12.0f , 9.2f )*20000000000.0f );
		
		// プレイヤーと接触したらプレイヤーの進行方向へ移動する
		ACTOR_DATA* BoxUserData = static_cast<ACTOR_DATA*>( m_Actor->userData );
		if( BoxUserData->ContactPairFlag > 0 )
		{
			// 接触したアクターのユーザーデータを取得
			NxActor *pContactActor = BoxUserData->pContactActor;
			if( pContactActor != nullptr )
			{
				ACTOR_DATA *ContactUserData = static_cast<ACTOR_DATA*>( pContactActor->userData );
				if( ContactUserData != nullptr )
				{ 
					// 接触したアクターのオブジェクトを調べ、それがプレイヤーならば衝突された方向へ力を加える
					CObjectBase *Object = ContactUserData->pObject;
					if( Object->GetName() == "Player" )
					{
						m_MovableVec = OBJECT_MANAGER->GetCPlayerObject()->GetPos() - m_vPos;
						m_MovableVec = -m_MovableVec;
						m_MovableVec.y = 0.0f;
						D3DXVec3Normalize( &m_MovableVec , &m_MovableVec );
						//m_MovablePower += (int)D3DXVec3Length( &CDPConverter::toDirectX( m_Actor->getLinearVelocity() ) );
						
						static const int MaxMovablePower = 50;
						m_MovablePower = MaxMovablePower;
						if( m_MovablePower > MaxMovablePower ){ m_MovablePower = MaxMovablePower; }
					}
				}
			}

		}
		if( m_MovablePower )
		{
			m_Actor->setLinearVelocity( CDPConverter::toPhysX( m_MovableVec ) * (float)m_MovablePower );

			m_MovablePower -= 1;
			if( m_MovablePower < 0 ){ m_MovablePower = 0; }
		}
	}
	else
	{
		if( !m_isJoint )
		{ 
			m_Actor->setMass( m_Mass ); 
			m_Actor->clearBodyFlag( NX_BF_DISABLE_GRAVITY );
			// 地面に押し付ける
			m_Actor->addForce( NxVec3( 0.0f , -2800000000000000.0f , 0.0f ) );
		}
	}

	// 浮遊可能時間の経過
	SubAntiGravity_Energy( 1 );

}


/*================================================*/
//! \brief 描画
//! \param[in] &_d3d	描画する為に使用
/*================================================*/
void CBox::Draw( CDirect3D & _d3d )
{
	if ( m_lpXFile == nullptr ){ return; }

	ColorUpdate();
	
	m_lpXFile->SetMaterial( m_Material );

	D3DXMATRIXA16 matWorld;
	// マトリックスの転置
	CDPConverter::SetMatrix( &matWorld , m_Actor );
	_d3d.SetWorldTrans( &matWorld );
	m_lpXFile->Draw( _d3d.GetDevice() );

}


/*=========================================================*/
//! \brief オブジェクトの色を更新する
/*=========================================================*/
void CBox::ColorUpdate( void )
{

	// 浮遊時間が長いほど色が青くなる
	m_Material[0].Diffuse.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Diffuse.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Diffuse.b = 1.0f;

	m_Material[0].Emissive.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Emissive.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Emissive.b = 1.0f;
	
	m_Material[0].Ambient.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Ambient.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Ambient.b = 1.0f;



	m_Material[1].Diffuse.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Diffuse.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Diffuse.b = 1.0f;

	m_Material[1].Emissive.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Emissive.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Emissive.b = 1.0f;
	
	m_Material[1].Ambient.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Ambient.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Ambient.b = 1.0f;

	m_Material[0].Power = 40.0f;

}


/*=======================================================*/
//! \brief 行列を指定して描画
//! \param[in] &_d3d		描画する為に使用
//! \param[in] *_mWorld		行列の指定
/*=======================================================*/
void CBox::Draw( CDirect3D & _d3d , D3DXMATRIX *_mWorld )
{
	if ( m_lpXFile == nullptr ){ return; }

	ColorUpdate();

	m_lpXFile->SetMaterial( m_Material );

	_d3d.SetWorldTrans( _mWorld );
	m_lpXFile->Draw( _d3d.GetDevice() );
}
