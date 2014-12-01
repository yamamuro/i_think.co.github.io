#include "stdafx.hpp"

#include "Debug/CDebug.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "Texture/CTextureManager.hpp"
#include "XFile/CXFileManager.hpp"
#include "../Scene/SceneManager/CSceneManager.hpp"
#include "Direct3D/CDirect3D.hpp"
#include "CApplication.hpp"


/*==============================================*/
//! CApplicationのポインタの定義
/*==============================================*/
CApplication* CApplication::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
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

	// ウィンドウサイズの初期化
	SetWindowSize();

	// タイマーの精度を上げる
	timeBeginPeriod( 1 );
	
}


/*==============================================================================*/
//! \brief アプリケーションの全体の初期化
//! \param[in]	_FullScreenFlag		フルスクリーンかどうかのフラグ
//! \return	成功 = true
//! \return 失敗 = false
/*==============================================================================*/
bool CApplication::InitializeApplication( const bool _FullScreenFlag )
{

	// フルスクリーンかどうかの確認
#ifdef _DEBUG
	FullScreenFlag = bFullScreen;
#else
	FullScreenFlag = _FullScreenFlag;
#endif

	m_pXFileManager = XFILE_MANAGER;

	m_pDirect3D = DIRECTX;
	if( m_pDirect3D->Initialize( hWnd , FullScreenFlag ) == nullptr )
	{
		ErrorMessage( "DirectXの初期化に失敗しました\n" );
		SafeDelete( VARIABLE_DATA( m_pDirect3D ) );
		return false;
	}

	m_pPhysX = PHYSX;
	if( m_pPhysX->Initialize() == false )
	{
		ErrorMessage( "PhysXの初期化に失敗しました\n" );
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
//! \brief デストラクタ
/*=====================================*/
CApplication::~CApplication( void )
{
}


/*================================================*/
//! \brief アプリケーションの解放処理
/*================================================*/
void CApplication::Delete( void )
{

	DEBUG->PrintConsole( "\n--------ゲームの終了--------\n\n" , CDebug::ConPreside );

	// CDebugはMainで解放する

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
	
	// タイマーの精度を元に戻す
	timeEndPeriod( 1 );

}


/*==========================================*/
//! \brief ゲームループ
//! \return	ループ継続 = true
//! \return ループ終了 = false
/*==========================================*/
bool CApplication::GameLoop( void )
{

	// 毎秒何回画面更新をするのかを格納
	const int FrameRate = 1000 / FRAME_RATE;	

	// メッセージループ
	while (1)
	{
		// メッセージがあった場合、メッセージを処理する
		if ( PeekMessage( &msg , NULL , 0 , 0 , PM_NOREMOVE ) )
		{
			if ( !GetMessage( &msg , NULL , 0 , 0 ) ){ break; }
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{

			DWORD StartTime = timeGetTime();	// FPS制御用現在時刻を覚える

			// ゲームの更新を行う
			m_pSceneManager->Run();

			DWORD NowTime = timeGetTime();
			
			// スレッドスリープ
			if( NowTime - StartTime < FrameRate ){ Sleep( FrameRate - ( NowTime - StartTime ) ); }
		
		}
	}

	return true;

}

