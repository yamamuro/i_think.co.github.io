#include "stdafx.hpp"

#include "../GimmickBase/CGimmickBase.hpp"
#include "CSwitch.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CSwitch::CSwitch( void )
	: m_lpPushTexture( nullptr )
{

	m_FilePass += "Object/Gimmick/Switch/Switch.x";
	// メッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	// アクターデータ
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_SWITCH;
	m_ActorData.Dimension = NxVec3( 5.2f , 3.0f , 5.2f );	// 大きさ
	m_ActorData.AngularDamping = 1000000.0f;
	m_ActorData.FixFlag = true;

	// テクスチャの読み込み
	m_lpPushTexture = TEXTURE_MANAGER->Load( &std::string( "Grey.jpg" ) );

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CSwitch::DiscreteInit( void )
{


	// スイッチ部分
	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );


	// 鉄の部分のアクターを作成する
	// デスクリプタ
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();							// 初期化
	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();								// 初期化
	boxDesc.dimensions = NxVec3( 8.2f , 2.0f , 8.2f );
	NxActorDesc actorDesc;
	actorDesc.setToDefault();							// 初期化
	actorDesc.body = NULL;
	actorDesc.globalPose.t = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	actorDesc.shapes.pushBack( &boxDesc );				// ボックスをアクターに登録

	m_pPhysX->GetScene()->createActor( actorDesc );

	// オーバーラップ時に使用するデータ
	m_worldBox.extents = m_ActorData.Dimension + NxVec3( 0.0f , 0.1f , 0.0f );		// 少し余裕を持たせる
	m_worldBox.center = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );

}



/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CSwitch::~CSwitch( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CSwitch::Exec( void )
{

	D3DXMATRIXA16 mWorld;
	CDPConverter::SetMatrix( &mWorld , m_Actor );

	// 座標を抽出
	m_vPos.x = mWorld._41;
	m_vPos.y = mWorld._42;
	m_vPos.z = mWorld._43;

	// スイッチのプッシュ判定更新
	NxMat33 NxMat;
	NxQuat quat( 0.0f , NxVec3( 0 , 1 , 0 ) ); 
	NxMat.fromQuat( quat );
	m_worldBox.rot = NxMat;
	if ( m_pPhysX->GetScene()->checkOverlapOBB( m_worldBox , NX_DYNAMIC_SHAPES , CPhysX::BitShift( CPhysX::CG_HEAVY_OBJECT ) | CPhysX::BitShift( CPhysX::CG_PLAYER ) ) == true )
		m_bWorking = true;
	else
		m_bWorking = false;

}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CSwitch::Draw( CDirect3D &_d3d )
{
	
	if( m_lpXFile == nullptr ){ return; }

	if( m_bWorking )
		// 押されている状態のテクスチャを張り付ける
		m_lpXFile->GetTexture()[1]->SetTexture( m_lpPushTexture->GetTexture() );
	else 
		// 稼働時のテクスチャを張り付ける
		m_lpXFile->GetTexture()[1]->Restore();
	
	D3DXMATRIXA16 matWorld;
	// マトリックスの転置
	CDPConverter::SetMatrix( &matWorld , m_Actor );
	_d3d.SetWorldTrans( &matWorld );
	m_lpXFile->Draw( _d3d.GetDevice() );

}


