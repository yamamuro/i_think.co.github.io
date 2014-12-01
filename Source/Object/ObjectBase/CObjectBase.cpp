#include "stdafx.hpp"

#include "Wrapper\Matrix\CMatrix.hpp"
#include "Direct3D\CDirect3D.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "CObjectBase.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CObjectBase::CObjectBase( void ) 
	: m_pPhysX( nullptr )
	, m_lpXFile( nullptr )
	, m_Actor( nullptr )
	, m_NxMaterial( nullptr )
	, m_DrawFlag( false )
	, m_LaserHitFlag( false )
	, m_AntiGravity_Energy( 0 )
	, m_Material( nullptr )
{

	m_vRot = D3DXVECTOR3( 0 , 0 , 0 );
	m_vPos = D3DXVECTOR3( 0 , 0 , 0 );

	m_ActorData.Type = '0';
	m_ActorData.ObjectName = CObjectManager::OBJ_NONE;
	m_ActorData.Dimension = NxVec3( 5 , 5 , 5 );
	m_ActorData.Radius = 5.0f;
	m_ActorData.Height;
	m_ActorData.FixFlag = false;
	m_ActorData.AngularDamping = 0.0f;
	m_ActorData.Density = 10000.0f;
	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	m_ActorData.MeshScale = NxVec3( 1.0f , 1.0f , 1.0f );

	m_FilePass = "Resource/XFile/";

	m_pd3d = CAPP->GetCDirect3D();
	m_pPhysX = CAPP->GetCPhysX();

}

/*==========================================================================================*/
//! \brief 初期化
//! \param[in]	_Data		オブジェクトの初期化に使用するデータ
/*==========================================================================================*/
void CObjectBase::Init( CObjectManager::OBJECT_DATA _Data )
{

	Name = _Data.Name;

	m_vPos = _Data.Pos;
	m_vRot = _Data.Rot;
	m_vScaling = _Data.Scaling;

	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	CDPConverter::SetRot( &m_ActorData.Rotation , &m_vRot );

	m_ActorData.pObject = this;

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CObjectBase::DiscreteInit( void )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CObjectBase::~CObjectBase( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CObjectBase::Exec( void )
{
}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CObjectBase::Draw( CDirect3D & _d3d )
{
	if( m_lpXFile == nullptr ){ return; }
	_d3d.SetWorldTrans( &GetMatrix() );
	m_lpXFile->Draw( _d3d.GetDevice() );
}


/*==================================*/
//! \brief 行列を返す
/*==================================*/
CMatrix CObjectBase::GetMatrix( void )
{
	CMatrix mPos, mRot;
	mPos.SetTrans(  m_vPos.x , m_vPos.y , m_vPos.z );
	mRot.SetRotate( m_vRot.x , m_vRot.y , m_vRot.z );

	return mRot*mPos;
}

