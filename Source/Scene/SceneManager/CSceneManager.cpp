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
// CSceneManagerのポインタの定義
/*=================================================*/
CSceneManager* CSceneManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
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


	// テクスチャの読み込み
	// テクスチャマネージャを経由して読み込むと実行中にテクスチャを削除されてしまうのでCTextureから直接読み込む
	m_lpLoadingTexture = new CTexture;
	m_lpLoadingTexture->Load( m_pd3d->GetDevice() , std::string( "Resource/Texture/NowLoading.png" ) );

}


/*====================================*/
//! \brief 初期化
/*====================================*/
void CSceneManager::Initialize( void )
{
	ChangeScene( TITLE );
	//ChangeScene( GAME );
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CSceneManager::~CSceneManager( void )
{
	SafeDelete( VARIABLE_DATA(  m_NowScene ) );

	// テクスチャの解放
	SafeDelete( m_lpLoadingTexture );
}


/*===============================================*/
//! \brief シーンの変更を行う
//! \param[in]	_scene	変更するシーン
/*===============================================*/
void CSceneManager::ChangeScene( int _Scene )
{

	m_LoadScene = _Scene;
	
	CRandomize Rand;
	m_LoadingAnimation = Rand.GetRandomize( 0 , 0 );
	
	// カメラの設定
	m_pd3d->SetCamPos( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	m_pd3d->SetCamRot( D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) );

	if( _Scene != END )
	{
		// 読み込み中のフラグを立てる
		m_LoadingFlag = LOADING;

		// 読み込みスレッドの起動
		m_pLoadThread = new boost::thread(  &CSceneManager::SceneLoading  , this );
		// メインスレッド側ではロード中の画面を表示する
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
//! \brief ゲームの更新を行う
//! \return 成功 = true
//! \return 失敗 = false
/*=======================================*/
bool CSceneManager::Run( void )
{

	// ウィンドウがアクティブでないならば処理を抜ける
	if( GetActiveWindow() != CAPP->hWnd ){ return true; }
	
	// フレームカウンタのカウントアップ
	CAPP->FrameCountUp();

	// 使用するキーの状態を更新する
	m_pKeyboard->AllStateUpdata();
	// マウスの状態を更新する
	m_pMouse->AllStateUpdata();

	// エラーが発生していないかのチェック
	if( CAPP->GetErrorFlag() ){ return false; }

	// PhysXのワイヤーフレームを切り替える
	if( m_pKeyboard->GetState( CKeyboard::KEY_T ) == CKeyboard::KST_PUSH )
	{
		m_pPhysX->NotDrawPhysX();
	}
	// メッシュの表示を切り替える
	if( m_pKeyboard->GetState( CKeyboard::KEY_Y ) == CKeyboard::KST_PUSH )
	{
		m_pd3d->NotDrawDirectX();
	}


	// ゲームの更新
	if( m_NowScene != nullptr && !m_NowScene->Updata() )
	{
		// 画面の描画
		m_NowScene->Draw();
	}

	// アクターをシミュレートする
	m_pPhysX->Simulation();

	// 後始末
	Rearrange();

	return true;

}


/*=================================================*/
//! \brief マイフレームの最後に行う後始末
/*=================================================*/
void CSceneManager::Rearrange()
{
	// マウスホイールの移動量はウィンドウプロシージャから拾ってくるので
	// フレームの最後に初期化を行う
	m_pMouse->SetWheelMovement( 0 );
}


/*==========================================*/
//! \brief シーンを変更する
/*==========================================*/
void CSceneManager::SceneLoading( void )
{

	if( m_NowScene != nullptr )
	{
		DEBUG->PrintConsole( "===" + m_NowScene->GetName() + "を解放します===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );


		// XFileを解放する
		XFILE_MANAGER->AllClear();

		// テクスチャの解放
		TEXTURE_MANAGER->AllClear();

		// Waveを解放する
		//SOUND_MANAGER->ReleaseWaves();
		
		// 新しく読み込む前に現在のシーンを解放する
		SafeDelete( VARIABLE_DATA( m_NowScene ) );

		DEBUG->PrintConsole( "\n================================================================\n" , CDebug::ConFence );
		DEBUG->PrintConsole( "\n\n" );
	}

	DEBUG->PrintConsole( "\n\n\n\n\n" );
	DEBUG->PrintConsole( "シーンを" + m_SceneName[m_LoadScene] + "に切り替えます\n" , CDebug::ConPreside );
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
	DEBUG->PrintConsole( m_SceneName[m_LoadScene] + "にシーンを切り替えました\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "\n\n" , CDebug::ConPreside );

	// 読み込み終了
	m_LoadingFlag = LOAD_END;

}


/*===============================================*/
//! \brief シーン切り替え中の演出
/*===============================================*/
void CSceneManager::LoadingScreen( void )
{

	// スプライトの取得
	CSprite *pSprite = m_pd3d->GetCSprite();

	// テクスチャのサイズを取得//////////////////////////
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

		// 描画開始
		LPDIRECT3DDEVICE9 Direct3D = m_pd3d->Begin();
		m_pd3d->SetProj();
		m_pd3d->SetCamera();
		m_pd3d->GetDevice()->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );	
		
		// スプライト描画開始
		pSprite->Begin();
		
		// 乱数生成
		CRandomize Rand;

		// 移動行列
		CMatrix mTrans;

		// 移動量
		D3DXVECTOR3 BasePos = D3DXVECTOR3( 310.0f , 440.0f , 0.0f );
		D3DXVECTOR3 RandomPos;
		RandomPos = D3DXVECTOR3( (float)Rand.GetRandomize( -5.0f , 5.0f ) , (float)Rand.GetRandomize( -5.0f , 5.0f ) , 0.0f );
		
		// 移動行列に移動量をセット
		mTrans.SetTrans( BasePos + RandomPos );

		// 行列の適用
		CMatrix mWorld;
		mWorld = mTrans;
		m_pd3d->SetWorldTrans( &mWorld );

		// Now Loadingを表示
		pSprite->GetSprite()->Draw( m_lpLoadingTexture->GetTexture() ,
						&LoadingTextureRect,
						&D3DXVECTOR3( 0.0f, 0.0f , 0.0f ),
						NULL,
						D3DCOLOR_ARGB( 255 , 255-Color , 255-Color , 255-Color ) );

		// スプライト描画終了
		pSprite->End();

		// 描画終了
		if( !m_pd3d->End() ){ ErrorExit(); return; }

		Sleep( 16 );

	}


}


/*=========================================================*/
//! \brief 読み込み中にエラーが発生した際の処理
/*=========================================================*/
void CSceneManager::ErrorExit( void )
{
	m_pLoadThread->detach();
	PostQuitMessage( 0 );
	return;
}