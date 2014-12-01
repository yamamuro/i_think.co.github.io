#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "CRope.hpp"


// インデックス番号を格納する変数の定義
int CRope::m_MaterialIndex = 0;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CRope::CRope( void )
	: m_RopeSegs( nullptr )
	, m_RopeLinks( nullptr )
{

	m_FilePass += "Object/Object/Chain/Chain.x";
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	m_DrawFlag = true;
}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CRope::DiscreteInit( void )
{

	m_RopeSegs  = new NxActor*[PartsNum];
	m_RopeLinks = new NxSphericalJoint*[PartsNum-1];

	// マテリアルの作成
	if( m_MaterialIndex == 0 )
	{
		NxMaterialDesc materialDesc;
		materialDesc.setToDefault();
		materialDesc.restitution		= 0.1f;
		materialDesc.dynamicFriction	= 0.5f;
		materialDesc.staticFriction		= 0.5f;
		NxMaterial *Mat = m_pPhysX->GetScene()->createMaterial( materialDesc );
		m_MaterialIndex = Mat->getMaterialIndex();
	}

	// ロープの作成
	m_pPhysX->CreateRope( m_RopeSegs , m_RopeLinks , NxVec3( m_vPos ) , 0.7f , 0.3f , PartsNum , 1000000.0f , m_MaterialIndex );

	for( int i=0 ; i<PartsNum ; i++ )
		m_RopeSegs[i]->raiseBodyFlag( NX_BF_FROZEN_ROT_Y );
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CRope::~CRope( void )
{
	SafeDeleteArray( VARIABLE_DATA( m_RopeSegs  ) );
	SafeDeleteArray( VARIABLE_DATA( m_RopeLinks ) );
}


/*================================*/
//! \brief 更新
/*================================*/
void CRope::Exec( void )
{

	float power = 3.0f;
	if( KEYBOARD->GetState( CKeyboard::KEY_R ) == CKeyboard::KST_HOLD )
		m_RopeSegs[0]->setLinearVelocity( NxVec3(   0.0f , 0.0f ,  20.0f )*power );
	if( KEYBOARD->GetState( CKeyboard::KEY_G ) == CKeyboard::KST_HOLD )
		m_RopeSegs[0]->setLinearVelocity( NxVec3(   0.0f , 0.0f , -20.0f )*power );
	if( KEYBOARD->GetState( CKeyboard::KEY_F ) == CKeyboard::KST_HOLD )
		m_RopeSegs[0]->setLinearVelocity( NxVec3( -20.0f , 0.0f ,   0.0f )*power );
	if( KEYBOARD->GetState( CKeyboard::KEY_V ) == CKeyboard::KST_HOLD )
		m_RopeSegs[0]->setLinearVelocity( NxVec3(  20.0f , 0.0f ,   0.0f )*power );

	m_RopeSegs[PartsNum-1]->setLinearVelocity( NxVec3( 0.0f, 80.0f , 0.0f ) );

}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CRope::Draw( CDirect3D & _d3d )
{
	if( m_lpXFile == nullptr ){ return; }


	D3DXMATRIXA16 matWorld;

	for( int i=0 ; i<PartsNum ; i++ )
	{
		// マトリックスの転置
		CDPConverter::SetMatrix( &matWorld , m_RopeSegs[i] );
		CMatrix mRot;
		mRot.SetRotate( D3DXVECTOR3( 0.0f, 90.0f , 0.0f ) );
		_d3d.SetWorldTrans( i % 2 == 0 ? &( mRot * matWorld ) : &matWorld );
		m_lpXFile->Draw( _d3d.GetDevice() );
	}
}