#include "stdafx.hpp"

#include "../Application/CApplication.hpp"


/*=========================================================*/
//!
//! \file
//! \brief <b>プログラムのエントリポイント</b>
//! \brief ウィンドウの作成を行う
//!
//! \author	Y.Yamamuro \n
//! \date  2014/09/03
//! \since 2014/09/08
//!
/*=========================================================*/


/*===================================================================================*/
//! \brief ウィンドウを作成する
//! \param[out]	*_FullScreenFlag	フルスクリーンで起動するのかのフラグ
//! \param[in]	_nWinMode			WinMainの引数を使用	ウィンドウの表示方法
//! \return true  = 成功
//! \return false = 失敗
/*===================================================================================*/
bool MakeWindow( bool *_FullScreenFlag , const int _nWinMode );


/*=====================================================================*/
//! \brief ウィンドウのクライアントのサイズを指定サイズにする。
//! \param[in]	_hWnd		ウィンドウハンドル
//! \param[in]	_Width		ウィンドウの幅
//! \param[in]	_Height		ウィンドウの高さ
/*=====================================================================*/
void SetClientSize( const HWND _hWnd , const int _Width , const int _Height );


/*================================================================*/
//! \brief プログラムのエントリポイント
//! \param[in]	_hInstance	インスタンスハンドル
//! \param[in]	_hPrevInst	使用しない　値はNULL
//! \param[in]	_lpszArgs	使用しない　コマンドライン引数
//! \param[in]	_nWinMode	ウィンドウの表示モード
/*================================================================*/
int WINAPI WinMain( HINSTANCE _hInstance , HINSTANCE _hPrevInst , LPSTR _lpszArgs , int _nWinMode )
{

	bool FullScreenFlag = false;
	CDebug	*m_pDebug = nullptr;

#ifdef _DEBUG
	// メモリリークの検出
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// デバッグ管理用のクラス
	m_pDebug = DEBUG;	
#endif


	// インスタンスをコピーしておく
	CAPP->hInst = _hInstance;
	CAPP->SetCDebug( m_pDebug );

	DEBUG->PrintConsole( "===" + GAME_TITLE + "の初期化を始めます===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( "\n\n" );

	// ウィンドウを作成する
	if( MakeWindow( &FullScreenFlag , _nWinMode ) == false )
	{
		std::string sErrorMessage = "ウィンドウの作成に失敗しました\n本作品はXP以降のWindowsOSでの動作を前提としています。\nもし上記を満たしており、エラーが発生した場合はご連絡お願いします";
		ErrorMessage( sErrorMessage );

		SafeExit();
		SafeDelete( VARIABLE_DATA( m_pDebug ) );
		return 1;
	}
	DEBUG->PrintConsole( "ウィンドウの作成に成功しました...\n" , CDebug::ConPreside );
	
	// アプリケーションの初期化
	if( CAPP->InitializeApplication( FullScreenFlag ) == false )
	{
		std::string sErrorMessage = "ゲームの初期化中にエラーが発生しました\n安全にゲームを終了します";
		ErrorMessage( sErrorMessage );

		SafeExit();
		SafeDelete( VARIABLE_DATA( m_pDebug ) );
		return 1;
	}

	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( GAME_TITLE + "の初期化に成功しました...\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "\n\n" );


	// ゲームループの開始
	CAPP->GameLoop();


	// 解放
	CAPP->Delete();


#ifdef _DEBUG
	// CDebugの解放は一番最後に行う
	SafeDelete( VARIABLE_DATA( m_pDebug ) );
#endif


	return 0; 

}


/*=========================================================================*/
//! \brief ウィンドウプロシージャ
//! \param[in]	_hWnd		ウィンドウハンドル
//! \param[in]	_Message	メッセージの識別子
//! \param[in]	_wParam		メッセージの最初のパラメータ
//! \param[in]	_lParam		メッセージの 2 番目のパラメータ
//! \return		メッセージ処理の結果
/*=========================================================================*/
LRESULT CALLBACK WindowProc( HWND _hWnd , UINT _Message , WPARAM _wParam , LPARAM _lParam )
{

	// メッセージによって処理を選択
	switch( _Message )
	{

	case WM_CLOSE:
		MessageBox( CAPP->GetHWND() , "エスケープキーを押して終了してください" , NULL , MB_OK );
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	case WM_SIZE:
		CAPP->SetWindowSize();
		break;

	case WM_MOUSEWHEEL:
		MOUSE->SetWheelMovement( (SHORT)HIWORD(_wParam)/WHEEL_DELTA );
		break;

	default:
		return DefWindowProc( _hWnd , _Message , _wParam , _lParam );
	}

	return 0;
}


/*=======================================================================================*/
//! \brief ウィンドウを作成する
//! \param[out]	*_FullScreenFlag	フルスクリーンで起動するのかのフラグ
//! \param[in]	_nWinMode			WinMainの引数を使用	ウィンドウの表示方法
//! \return true  = 成功
//! \return false = 失敗
/*=======================================================================================*/
bool MakeWindow( bool *_FullScreenFlag , const int _nWinMode )
{

	WNDCLASSEX wc;											// ウィンドウクラスの定義
	
	// ウィンドウクラスの定義
	wc.cbSize = sizeof(WNDCLASSEX);							// 構造体のサイズ
	wc.style = CS_DBLCLKS;									// スタイル
	wc.lpfnWndProc = WindowProc;							// ウインドウ関数
	wc.cbClsExtra = 0;										// エキストラクラス情報 
	wc.cbWndExtra = 0;										// エキストラウィンドウ情報
	wc.hInstance = CAPP->hInst;								// インスタンスハンドル
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// ラージアイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);				// スモールアイコン 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// マウスカーソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景色 
	wc.lpszMenuName = NULL;									// メインメニュー名
	wc.lpszClassName = WINDOW_NAME;							// ウィンドウクラス名

	// ウィンドウクラスの登録
	if ( !RegisterClassEx( &wc ) )
	{
		ErrorMessage( "ウィンドウの登録に失敗しました。\n" );
		return false;
	}

	// スクリーンの解像度を取得
	int ScreenSizeWidth  = GetSystemMetrics( SM_CXSCREEN );
	int ScreenSizeHeight = GetSystemMetrics( SM_CYSCREEN );
	POINT WindowPos;
	WindowPos.x = ( ScreenSizeWidth  - WINDOW_WIDTH  ) / 2;
	WindowPos.y = ( ScreenSizeHeight - WINDOW_HEIGHT ) / 2;

	// ウィンドウの作成
	CAPP->hWnd = CreateWindow(
		WINDOW_NAME,						// 作成するウィンドウ名
		GAME_TITLE.c_str(),					// ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,				// ウィンドウタイプを標準タイプに	
		WindowPos.x + WINDOW_POSITION_X ,	// ウィンドウの位置（Ｘ座標）
		WindowPos.y + WINDOW_POSITION_Y ,	// ウィンドウの位置（Ｙ座標）						
		WINDOW_WIDTH,						// ウィンドウの幅
		WINDOW_HEIGHT,						// ウィンドウの高さ			
		NULL,								// 親ウィンドウのハンドル
		NULL,								// メニューのハンドル
		CAPP->hInst,						// インスタンスハンドル 
		NULL);								// 追加情報 


	// ウィンドウの表示
#ifdef _DEBUG
	ShowWindow( CAPP->hWnd , bFullScreen ? ( SW_SHOW ) : ( _nWinMode ) );
#else
	int Select;
	Select = MESSAGE( "フルスクリーンで起動しますか？" );
	*_FullScreenFlag = Select == IDYES ? ( true ) : ( false );
	ShowWindow( CAPP->hWnd , Select == IDYES ? ( SW_SHOW ) : ( _nWinMode ) );
#endif

	// ウィンドウの更新
	UpdateWindow( CAPP->hWnd );

	return true;

}


/*=========================================================================*/
//! \brief ウィンドウのクライアントのサイズを指定サイズにする。
//! \param[in]	_hWnd		ウィンドウハンドル
//! \param[in]	_Width		ウィンドウの幅
//! \param[in]	_Height		ウィンドウの高さ
/*=========================================================================*/
void SetClientSize( const HWND _hWnd , const int _Width , const int _Height )
{

	RECT rcWnd, rcCli;

	GetWindowRect( _hWnd , &rcWnd );	// ウィンドウのRECT取得
	GetClientRect( _hWnd , &rcCli );	// クライアント領域のRECT取得

	// ウィンドウの余白を考えて、クライアントのサイズを指定サイズにする。
	MoveWindow( _hWnd ,
		rcWnd.left ,	// X座標
		rcWnd.top ,		// Y座標
		_Width  + ( rcWnd.right  - rcWnd.left ) - ( rcCli.right  - rcCli.left ),
		_Height + ( rcWnd.bottom - rcWnd.top  ) - ( rcCli.bottom - rcCli.top  ),
		TRUE );

}

