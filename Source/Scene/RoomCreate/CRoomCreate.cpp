#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "CRoomCreate.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CRoomCreate::CRoomCreate( void )
{
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CRoomCreate::~CRoomCreate( void )
{
}


/*===========================================*/
//! \brief �X�V
//! \return �V�[���̕ύX = true
//! \return �p�� �@�@�@�@= false
/*===========================================*/
bool CRoomCreate::Updata( void )
{
	
	// �G�X�P�[�v�L�[�Ń^�C�g����ʂɖ߂�
	if( m_pKeyboard->GetState( CKeyboard::KEY_ESCAPE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::TITLE );
		return true;
	}


	m_pObjectManager->Updata();


	return false;

}


/*=================================*/
//! \brief ��ʕ\��
/*=================================*/
void CRoomCreate::Draw( void )
{

	// �`�揀��
	LPDIRECT3DDEVICE9 Direct3D = m_pd3d->Begin();
	m_pd3d->SetProj();
	m_pd3d->SetCamera();

	m_pObjectManager->Draw();

	// �f�o�b�O�`��
	if( m_pPhysX->GetDebugRender() != nullptr && m_pPhysX->GetPhysXDrawFlag() )
	{
		m_pPhysX->GetDebugRender()->RenderData( m_pd3d->GetDevice(), m_pPhysX->GetScene()->getDebugRenderable() );
	}	
	// FPS��\������
	if( m_pDebug != nullptr ){ m_pDebug->DrawFPS(); }
	
	m_pd3d->End();

}