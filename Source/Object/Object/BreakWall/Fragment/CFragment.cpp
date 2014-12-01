#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../CBreakWall.hpp"
#include "CFragment.hpp"


int CFragment::m_MaterialIndex = -1;


/*===============================================*/
//! \brief コンストラクタ
/*===============================================*/
CFragment::CFragment( const int _Type , D3DXVECTOR3 _Pos )
	: m_Type( 0 )
{

	Name = "Fragment"+std::to_string( _Type );

	m_FilePass += "Object/Object/BreakWall/FragmentType" + std::to_string( _Type ) + ".x";
	// メッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	m_Type = _Type;
	m_vPos = _Pos;

	// オブジェクトのサイズを乱数で決める
	CRandomize Rand;
	m_Scale = (float)Rand.GetRandomize( 0.5 , 1.4 );

 	m_DrawFlag = true;

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CFragment::DiscreteInit( void )
{


	NxVec3 Dimensions[CBreakWall::FragmentType] = {   NxVec3( 0.60f , 0.30f , 1.15f ) 
													, NxVec3( 0.25f , 0.15f , 1.65f )
													, NxVec3( 0.60f , 0.30f , 1.65f ) };
	
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();
	NxActorDesc actorDesc;
	actorDesc.setToDefault();
	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();	
	boxDesc.skinWidth = 0.0f;
	actorDesc.density = 1000000.0f;
	actorDesc.body = &bodyDesc;
	actorDesc.globalPose.t = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	NxMat33 Mat;
	CRandomize Rand;
	D3DXVECTOR3 Rot( (float)Rand.GetRandomize( 0.0 , 359.0 ) , (float)Rand.GetRandomize( 0.0 , 359.0 ) , (float)Rand.GetRandomize( 0.0 , 359.0 ) );
	CDPConverter::SetRot( &Mat , &Rot );
	actorDesc.globalPose.M = Mat;
	bodyDesc.angularDamping = 0.0f;
	boxDesc.dimensions = Dimensions[m_Type-1]*m_Scale;


	if( m_MaterialIndex == -1 )
	{
		// アクターのマテリアルを設定する
		NxMaterialDesc materialDesc;
		materialDesc.restitution		= 0.4f;
		materialDesc.dynamicFriction	= 3.8f;
		materialDesc.staticFriction		= 3.5f;
		m_MaterialIndex					= m_pPhysX->GetScene()->createMaterial( materialDesc )->getMaterialIndex();

		boxDesc.materialIndex = m_MaterialIndex;
	}
	else
	{
		boxDesc.materialIndex = m_MaterialIndex;
	}
	actorDesc.shapes.pushBack( &boxDesc );
	m_Actor = m_pPhysX->GetScene()->createActor( actorDesc );
	m_Actor->putToSleep();

	m_Material = m_lpXFile->GetMaterial();

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CFragment::~CFragment( void )
{
	m_MaterialIndex = -1;
}


/*================================*/
//! \brief 更新
/*================================*/
void CFragment::Exec( void )
{

	// レーザーヒット時エネルギーを貯める
	AddAntiGravity_Energy( 260 );
	

	if( m_AntiGravity_Energy )
	{

		m_Actor->setLinearVelocity( NxVec3( 0.0f , 15.0f , 0.0f ) );
		// アクターに回転を加える
		m_Actor->addTorque( NxVec3( 6.0f , 18.0f , 9.2f ) );
	}

	SubAntiGravity_Energy( 2 );


}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CFragment::Draw( CDirect3D & _d3d )
{
	if( m_lpXFile == nullptr ){ return; }

	m_Material[0].Ambient.r = 0.6f - m_AntiGravity_Energy / (float)MaxEnergy * 0.3f;
	m_Material[0].Ambient.g = 0.6f - m_AntiGravity_Energy / (float)MaxEnergy * 0.3f;
	m_Material[0].Ambient.b = 0.6f + m_AntiGravity_Energy / (float)MaxEnergy * 0.4f;

	m_Material[0].Emissive.r = 0.0f;
	m_Material[0].Emissive.g = 0.0f;
	m_Material[0].Emissive.b = m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;

	m_Material[1].Ambient.r = 0.6f - m_AntiGravity_Energy / (float)MaxEnergy * 0.3f;
	m_Material[1].Ambient.g = 0.6f - m_AntiGravity_Energy / (float)MaxEnergy * 0.3f;
	m_Material[1].Ambient.b = 0.6f + m_AntiGravity_Energy / (float)MaxEnergy * 0.4f;

	m_Material[1].Emissive.r = 0.0f;
	m_Material[1].Emissive.g = 0.0f;
	m_Material[1].Emissive.b = m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;

	m_lpXFile->SetMaterial( m_Material );

	D3DXMATRIXA16 matWorld;
	// マトリックスの転置
	CDPConverter::SetMatrix( &matWorld , m_Actor );
	CMatrix mScale;
	mScale.SetScale( m_Scale );
	_d3d.SetWorldTrans( &( mScale * matWorld ) );
	m_lpXFile->Draw( _d3d.GetDevice() );

}