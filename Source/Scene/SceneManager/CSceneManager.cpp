#include "stdafx.hpp"

#include "../Application/CApplication.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "../Scene/Title/CTitle.hpp"
#include "../Scene/Game/CGame.hpp"
#include "../Scene/RoomCreate/CRoomCreate.hpp"
#include "Input/Keyboard/CKeyboard.hpp"
#include "Input/Mouse/CMouse.hpp"
#include "Direct3D/CDirect3D.hpp"
#include "PhysX/CPhysX.hpp"
#include "Sound/CSound.hpp"
#include <boost/thread.hpp>
#include "CSceneManager.hpp"


/*=================================================*/
// CSceneManager�̃|�C���^�̒�`
/*=================================================*/
CSceneManager* CSceneManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CSceneManager::CSceneManager( void )
	: m_pd3d( CAPP->GetCDirect3D() )
	, m_pPhysX( CAPP->GetCPhysX() )
	, m_pKeyboard( CAPP->GetCKeyboard() )
	, m_pMouse( CAPP->GetCMouse() )
	, m_NowScene( nullptr )
	, m_lpLoadingTexture( nullptr )
	, m_LoadingFlag( LOAD_END )
{

	m_SceneName[NONE]		 = "None";
	m_SceneName[TITLE]		 = "Title";
	m_SceneName[GAME]		 = "Game";
	m_SceneName[ROOM_CREATE] = "RoomCreate";
	m_SceneName[ENDING]		 = "Ending";
	m_SceneName[END]		 = "End";


	// �e�N�X�`���̓ǂݍ���
	// �e�N�X�`���}�l�[�W�����o�R���ēǂݍ��ނƎ��s���Ƀe�N�X�`�����폜����Ă��܂��̂�CTexture���璼�ړǂݍ���
	m_lpLoadingTexture = new CTexture;
	m_lpLoadingTexture->Load( m_pd3d->GetDevice() , std::string( "Resource/Texture/NowLoading.png" ) );

}


/*====================================*/
//! \brief ������
/*====================================*/
void CSceneManager::Initialize( void )
{
	ChangeScene( TITLE );
	//ChangeScene( GAME );
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CSceneManager::~CSceneManager( void )
{
	SafeDelete( VARIABLE_DATA(  m_NowScene ) );

	// �e�N�X�`���̉��
	SafeDelete( m_lpLoadingTexture );
}


/*===============================================*/
//! \brief �V�[���̕ύX���s��
//! \param[in]	_scene	�ύX����V�[��
/*===============================================*/
void CSceneManager::ChangeScene( int _Scene )
{

	m_LoadScene = _Scene;
	
	CRandomize Rand;
	m_LoadingAnimation = Rand.GetRandomize( 0 , 0 );
	
	// �J�����̐ݒ�
	m_pd3d->SetCamPos( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	m_pd3d->SetCamRot( D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) );

	if( _Scene != END )
	{
		// �ǂݍ��ݒ��̃t���O�𗧂Ă�
		m_LoadingFlag = LOADING;

		// �ǂݍ��݃X���b�h�̋N��
		m_pLoadThread = new boost::thread(  &CSceneManager::SceneLoading  , this );
		// ���C���X���b�h���ł̓��[�h���̉�ʂ�\������
		LoadingScreen();
		//SceneLoading();

		SafeDelete( m_pLoadThread );
	}
	else
	{
		SceneLoading();
	}
	MOUSE->SetCursor( FALSE );
}

/*=======================================*/
//! \brief �Q�[���̍X�V���s��
//! \return ���� = true
//! \return ���s = false
/*=======================================*/
bool CSceneManager::Run( void )
{

	// �E�B���h�E���A�N�e�B�u�łȂ��Ȃ�Ώ����𔲂���
	if( GetActiveWindow() != CAPP->hWnd ){ return true; }
	
	// �t���[���J�E���^�̃J�E���g�A�b�v
	CAPP->FrameCountUp();

	// �g�p����L�[�̏�Ԃ��X�V����
	m_pKeyboard->AllStateUpdata();
	// �}�E�X�̏�Ԃ��X�V����
	m_pMouse->AllStateUpdata();

	// �G���[���������Ă��Ȃ����̃`�F�b�N
	if( CAPP->GetErrorFlag() ){ return false; }

	// PhysX�̃��C���[�t���[����؂�ւ���
	if( m_pKeyboard->GetState( CKeyboard::KEY_T ) == CKeyboard::KST_PUSH )
	{
		m_pPhysX->NotDrawPhysX();
	}
	// ���b�V���̕\����؂�ւ���
	if( m_pKeyboard->GetState( CKeyboard::KEY_Y ) == CKeyboard::KST_PUSH )
	{
		m_pd3d->NotDrawDirectX();
	}


	// �Q�[���̍X�V
	if( m_NowScene != nullptr && !m_NowScene->Updata() )
	{
		// ��ʂ̕`��
		m_NowScene->Draw();
	}

	// �A�N�^�[���V�~�����[�g����
	m_pPhysX->Simulation();

	// ��n��
	Rearrange();

	return true;

}


/*=================================================*/
//! \brief �}�C�t���[���̍Ō�ɍs����n��
/*=================================================*/
void CSceneManager::Rearrange()
{
	// �}�E�X�z�C�[���̈ړ��ʂ̓E�B���h�E�v���V�[�W������E���Ă���̂�
	// �t���[���̍Ō�ɏ��������s��
	m_pMouse->SetWheelMovement( 0 );
}


/*==========================================*/
//! \brief �V�[����ύX����
/*==========================================*/
void CSceneManager::SceneLoading( void )
{

	if( m_NowScene != nullptr )
	{
		DEBUG->PrintConsole( "===" + m_NowScene->GetName() + "��������܂�===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );


		// XFile���������
		XFILE_MANAGER->AllClear();

		// �e�N�X�`���̉��
		TEXTURE_MANAGER->AllClear();

		// Wave���������
		//SOUND_MANAGER->ReleaseWaves();
		
		// �V�����ǂݍ��ޑO�Ɍ��݂̃V�[�����������
		SafeDelete( VARIABLE_DATA( m_NowScene ) );

		DEBUG->PrintConsole( "\n================================================================\n" , CDebug::ConFence );
		DEBUG->PrintConsole( "\n\n" );
	}

	DEBUG->PrintConsole( "\n\n\n\n\n" );
	DEBUG->PrintConsole( "�V�[����" + m_SceneName[m_LoadScene] + "�ɐ؂�ւ��܂�\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );
	switch( m_LoadScene )
	{
	case TITLE:
		m_NowScene = new CTitle();
		break;
	case GAME:
		m_NowScene = new CGame();
		break;
	case ROOM_CREATE:
		//m_NowScene = new CRoomCreate();
	case END:
		PostQuitMessage( 0 );
		break;
	}
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( m_SceneName[m_LoadScene] + "�ɃV�[����؂�ւ��܂���\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "\n\n" , CDebug::ConPreside );

	// �ǂݍ��ݏI��
	m_LoadingFlag = LOAD_END;

}


/*===============================================*/
//! \brief �V�[���؂�ւ����̉��o
/*===============================================*/
void CSceneManager::LoadingScreen( void )
{

	// �X�v���C�g�̎擾
	CSprite *pSprite = m_pd3d->GetCSprite();

	// �e�N�X�`���̃T�C�Y���擾//////////////////////////
	D3DSURFACE_DESC LoadingTextureDesc;
	if( FAILED( m_lpLoadingTexture->GetTexture()->GetLevelDesc( 0 , &LoadingTextureDesc ) ) )
	{
		LoadingTextureDesc.Width  = 0;
		LoadingTextureDesc.Height = 0;
	}
	////////////////////////////////////////////////////

	RECT LoadingTextureRect = { 0 , 0 , LoadingTextureDesc.Width , LoadingTextureDesc.Height };
	
	int Color = 0;
	while( m_LoadingFlag == LOADING )
	{
		Color += 3;
		if( Color > 255 ){ Color = 255; }

		// �`��J�n
		LPDIRECT3DDEVICE9 Direct3D = m_pd3d->Begin();
		m_pd3d->SetProj();
		m_pd3d->SetCamera();
		m_pd3d->GetDevice()->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );	
		
		// �X�v���C�g�`��J�n
		pSprite->Begin();
		
		// ��������
		CRandomize Rand;

		// �ړ��s��
		CMatrix mTrans;

		// �ړ���
		D3DXVECTOR3 BasePos = D3DXVECTOR3( 310.0f , 440.0f , 0.0f );
		D3DXVECTOR3 RandomPos;
		RandomPos = D3DXVECTOR3( (float)Rand.GetRandomize( -5.0f , 5.0f ) , (float)Rand.GetRandomize( -5.0f , 5.0f ) , 0.0f );
		
		// �ړ��s��Ɉړ��ʂ��Z�b�g
		mTrans.SetTrans( BasePos + RandomPos );

		// �s��̓K�p
		CMatrix mWorld;
		mWorld = mTrans;
		m_pd3d->SetWorldTrans( &mWorld );

		// Now Loading��\��
		pSprite->GetSprite()->Draw( m_lpLoadingTexture->GetTexture() ,
						&LoadingTextureRect,
						&D3DXVECTOR3( 0.0f, 0.0f , 0.0f ),
						NULL,
						D3DCOLOR_ARGB( 255 , 255-Color , 255-Color , 255-Color ) );

		// �X�v���C�g�`��I��
		pSprite->End();

		// �`��I��
		if( !m_pd3d->End() ){ ErrorExit(); return; }

		Sleep( 16 );

	}


}


/*=========================================================*/
//! \brief �ǂݍ��ݒ��ɃG���[�����������ۂ̏���
/*=========================================================*/
void CSceneManager::ErrorExit( void )
{
	m_pLoadThread->detach();
	PostQuitMessage( 0 );
	return;
}