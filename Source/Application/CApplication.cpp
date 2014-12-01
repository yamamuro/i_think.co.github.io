#include "stdafx.hpp"

#include "Debug/CDebug.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "Texture/CTextureManager.hpp"
#include "XFile/CXFileManager.hpp"
#include "../Scene/SceneManager/CSceneManager.hpp"
#include "Direct3D/CDirect3D.hpp"
#include "CApplication.hpp"


/*==============================================*/
//! CApplication�̃|�C���^�̒�`
/*==============================================*/
CApplication* CApplication::m_pInstance = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CApplication::CApplication( void )
	: m_pDebug( nullptr )
	, m_pDirect3D( nullptr )
	, m_pXFileManager( nullptr )
	, m_pMouse( nullptr )
	, m_pObjectManager( nullptr )
	, m_pSceneManager( nullptr )
	, m_pPhysX( nullptr )
	, m_pSoundManager( nullptr )
	, FrameCounter( 0 )
	, FullScreenFlag( false )
	, m_ErrorFlag( false )

{

	// �E�B���h�E�T�C�Y�̏�����
	SetWindowSize();

	// �^�C�}�[�̐��x���グ��
	timeBeginPeriod( 1 );
	
}


/*==============================================================================*/
//! \brief �A�v���P�[�V�����̑S�̂̏�����
//! \param[in]	_FullScreenFlag		�t���X�N���[�����ǂ����̃t���O
//! \return	���� = true
//! \return ���s = false
/*==============================================================================*/
bool CApplication::InitializeApplication( const bool _FullScreenFlag )
{

	// �t���X�N���[�����ǂ����̊m�F
#ifdef _DEBUG
	FullScreenFlag = bFullScreen;
#else
	FullScreenFlag = _FullScreenFlag;
#endif

	m_pXFileManager = XFILE_MANAGER;

	m_pDirect3D = DIRECTX;
	if( m_pDirect3D->Initialize( hWnd , FullScreenFlag ) == nullptr )
	{
		ErrorMessage( "DirectX�̏������Ɏ��s���܂���\n" );
		SafeDelete( VARIABLE_DATA( m_pDirect3D ) );
		return false;
	}

	m_pPhysX = PHYSX;
	if( m_pPhysX->Initialize() == false )
	{
		ErrorMessage( "PhysX�̏������Ɏ��s���܂���\n" );
		SafeDelete( VARIABLE_DATA( m_pDirect3D ) );
		SafeDelete( VARIABLE_DATA( m_pPhysX ) );
		return false;
	}

	m_pKeyboard = KEYBOARD;
	m_pKeyboard->StateInitialize();

	m_pMouse	= MOUSE;
	m_pMouse->Initialize( CAPP->GetHWND() );

	m_pObjectManager = OBJECT_MANAGER;

	m_pTextureManager = TEXTURE_MANAGER;

	m_pSceneManager = SCENE_MANAGER;
	m_pSceneManager->Initialize();

	//m_pSoundManager = SOUND_MANAGER;
	//m_pSoundManager->Initialize();


	return true;

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CApplication::~CApplication( void )
{
}


/*================================================*/
//! \brief �A�v���P�[�V�����̉������
/*================================================*/
void CApplication::Delete( void )
{

	DEBUG->PrintConsole( "\n--------�Q�[���̏I��--------\n\n" , CDebug::ConPreside );

	// CDebug��Main�ŉ������

	SafeDelete( VARIABLE_DATA( m_pXFileManager ) );
	SafeDelete( VARIABLE_DATA( m_pDirect3D ) );
	SafeDelete( VARIABLE_DATA( m_pKeyboard ) );
	SafeDelete( VARIABLE_DATA( m_pMouse ) );
	//SafeDelete( VARIABLE_DATA( m_pSoundManager ) );
	SafeDelete( VARIABLE_DATA( m_pObjectManager ) );
	SafeDelete( VARIABLE_DATA( m_pTextureManager ) );
	SafeDelete( VARIABLE_DATA( m_pSceneManager ) );
	SafeDelete( VARIABLE_DATA( m_pPhysX ) );
	SafeDelete( VARIABLE_DATA( m_pInstance ) );
	
	// �^�C�}�[�̐��x�����ɖ߂�
	timeEndPeriod( 1 );

}


/*==========================================*/
//! \brief �Q�[�����[�v
//! \return	���[�v�p�� = true
//! \return ���[�v�I�� = false
/*==========================================*/
bool CApplication::GameLoop( void )
{

	// ���b�����ʍX�V������̂����i�[
	const int FrameRate = 1000 / FRAME_RATE;	

	// ���b�Z�[�W���[�v
	while (1)
	{
		// ���b�Z�[�W���������ꍇ�A���b�Z�[�W����������
		if ( PeekMessage( &msg , NULL , 0 , 0 , PM_NOREMOVE ) )
		{
			if ( !GetMessage( &msg , NULL , 0 , 0 ) ){ break; }
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{

			DWORD StartTime = timeGetTime();	// FPS����p���ݎ������o����

			// �Q�[���̍X�V���s��
			m_pSceneManager->Run();

			DWORD NowTime = timeGetTime();
			
			// �X���b�h�X���[�v
			if( NowTime - StartTime < FrameRate ){ Sleep( FrameRate - ( NowTime - StartTime ) ); }
		
		}
	}

	return true;

}

