#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "CFence.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CFence::CFence( void )
{

	m_FilePass += "Object/Object/Fence/Fence.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	// �A�N�^�[�f�[�^
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_FENCE;
	m_ActorData.Dimension = NxVec3( 40.0f , 0.0f , 40.0f );	// �傫��
	m_ActorData.FixFlag = true;

	m_DrawFlag = true;

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CFence::DiscreteInit( void )
{
	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_THROUGH );
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CFence::~CFence( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CFence::Exec( void )
{
}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CFence::Draw( CDirect3D & _d3d )
{
	if( m_lpXFile == nullptr ){ return; }
	_d3d.GetDevice()->SetRenderState( D3DRS_LIGHTING , FALSE );
	_d3d.SetWorldTrans( &GetMatrix() );
	m_lpXFile->Draw( _d3d.GetDevice() );
	_d3d.GetDevice()->SetRenderState( D3DRS_LIGHTING , TRUE );
}