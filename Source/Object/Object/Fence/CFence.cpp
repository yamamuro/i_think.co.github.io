#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "CFence.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CFence::CFence( void )
{

	m_FilePass += "Object/Object/Fence/Fence.x";
	// メッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	// アクターデータ
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_FENCE;
	m_ActorData.Dimension = NxVec3( 40.0f , 0.0f , 40.0f );	// 大きさ
	m_ActorData.FixFlag = true;

	m_DrawFlag = true;

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CFence::DiscreteInit( void )
{
	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_THROUGH );
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CFence::~CFence( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CFence::Exec( void )
{
}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CFence::Draw( CDirect3D & _d3d )
{
	if( m_lpXFile == nullptr ){ return; }
	_d3d.GetDevice()->SetRenderState( D3DRS_LIGHTING , FALSE );
	_d3d.SetWorldTrans( &GetMatrix() );
	m_lpXFile->Draw( _d3d.GetDevice() );
	_d3d.GetDevice()->SetRenderState( D3DRS_LIGHTING , TRUE );
}