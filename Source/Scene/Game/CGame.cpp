#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "CGame.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CGame::CGame( void )
	//: m_pBGM( nullptr )
{

	m_FilePass = "Resource/Load/Room/";

	m_RoomsDataPass[TUTORIAL_ROOM]	= m_FilePass + "TutorialRoom/TutorialRoom.csv";
	m_RoomsDataPass[PASSAGEWAY]		= m_FilePass + "Passageway/Passageway.csv";
	m_RoomsDataPass[ROOM1]			= m_FilePass + "Room1/Room1.csv";
	m_RoomsDataPass[ROOM2]			= m_FilePass + "Room2/Room2.csv";
	m_RoomsDataPass[ROOM3]			= m_FilePass + "Room3/Room3.csv";
	m_RoomsDataPass[ROOM4]			= m_FilePass + "Room4/Room4.csv";

	m_Name[TUTORIAL_ROOM] = "TutorialRoom";
	m_Name[PASSAGEWAY]	  = "Passageway";
	m_Name[ROOM1]		  = "Room1";
	m_Name[ROOM2]		  = "Room2";
	m_Name[ROOM3]		  = "Room3";
	m_Name[ROOM4]		  = "Room4";

	// �X�e�[�W�̃I�u�W�F�N�g��ǂݍ���
	ChangeRoom( TUTORIAL_ROOM );
	//ChangeRoom( PASSAGEWAY );

	//m_pBGM = SOUND_MANAGER->Load( "BGM.wav" , 1 );

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CGame::~CGame( void )
{
}


/*===========================================*/
//! \brief �X�V
//! \return �V�[���̕ύX = true
//! \return �p�� �@�@�@�@= false
/*===========================================*/
bool CGame::Updata( void )
{
	
	// �G�X�P�[�v�L�[�Ń^�C�g����ʂɖ߂�
	if( m_pKeyboard->GetState( CKeyboard::KEY_ESCAPE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::TITLE );
		return true;
	}

	// �X�e�[�W�ړ�
//#ifdef _DEBUG
	if( m_pKeyboard->GetState( CKeyboard::KEY_1 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( TUTORIAL_ROOM );
		return true;
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_2 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( PASSAGEWAY );
		return true;
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_3 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( ROOM1 );
		return true;
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_4 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( ROOM2 );
		return true;
	}

//#endif


	DEBUG->DebugKeyCood( m_RoomID );

	m_pObjectManager->Updata();

	//m_pBGM->RePlay();

	return false;

}


/*=================================*/
//! \brief ��ʕ\��
/*=================================*/
void CGame::Draw( void )
{

	// �`�揀��
	LPDIRECT3DDEVICE9 Direct3D = m_pd3d->Begin();
	m_pd3d->SetProj();
	m_pd3d->SetCamera();

	m_pObjectManager->Draw();

	// FPS��\������
	if( m_pDebug != nullptr ){ m_pDebug->DrawFPS(); }
	
	m_pd3d->End();

}


/*=================================*/
//! \brief ������؂�ւ���
/*=================================*/
void CGame::ChangeRoom( const int _ID )
{

	DEBUG->PrintConsole( "\n\n\n" );
	DEBUG->PrintConsole( "\n===�O�����̃f�[�^���===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );

	// ���C�g�̍폜
	m_pd3d->GetCLightManager()->AllDelete();

	// ���ݎ����Ă���I�u�W�F�N�g���폜����
	OBJECT_MANAGER->AllDeleteObject();

	// XFile���������
	XFILE_MANAGER->AllClear();

	// �e�N�X�`���̉��
	TEXTURE_MANAGER->AllClear();

	// Wave�̉��
	//SOUND_MANAGER->ReleaseWaves();

	// �A�N�^�[�̉��
	m_pPhysX->AllDeleteActor();
	// �W���C���g�j��t���O��������
	m_pPhysX->GetJointNotify()->toBreak = false;

	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( "\n\n" );

	// ����ID�̃Z�b�g
	SetRoomID( _ID );

	// �����̃p�X���Z�b�g
	SetRoomPass( m_RoomsDataPass[_ID] );


	// �����ɃX�e�[�W�؂�ւ��̃G�t�F�N�g������
	 
	DEBUG->PrintConsole( "\n\n\n" );
	DEBUG->PrintConsole( "\n===�X�e�[�W[" + m_Name[_ID] + "]��ǂݍ��݂܂�===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	// �I�u�W�F�N�g��ǂݍ���
	m_pObjectManager->LoadObject( GetRoomDataPass() );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( "\n\n" );


}