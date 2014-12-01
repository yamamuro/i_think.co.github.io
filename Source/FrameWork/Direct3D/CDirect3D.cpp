#include "stdafx.hpp"

#include <string>

#include<d3dx9.h>
#pragma comment( lib , "d3d9.lib"   )
#pragma comment( lib , "d3dx9.lib"  )
#pragma comment( lib , "dxguid.lib" )

#include "Wrapper//Matrix/CMatrix.hpp"
#include "CDirect3D.hpp"


/*==============================================*/
//! CDirect3Dのポインタの定義
/*==============================================*/
CDirect3D* CDirect3D::m_pInstance	= nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CDirect3D::CDirect3D( void )
	: m_lpD3D( nullptr )
	, m_lpD3DDevice( nullptr )
	, m_lpBackbuffer( nullptr )
	, m_adapter( 0 )
	, m_pFont( nullptr )
	, m_LightManager( nullptr )
{				
					
	m_CamRx		= 0.0f;
	m_CamRy		= 0.0f;
	m_CamRz		= 0.0f;
	m_CamZoom	= 0.0f;
	m_CamPos = D3DXVECTOR3( 0 , 0 , 0 );

	m_DrawDirectX = true;

}

/*======================================================================*/
//! \brief DirectXの初期化処理
//! \param[in]	_hWnd	ウィンドウハンドル
//! \param[in]	_fsc	フルスクリーンにするかどうかのフラグ
//! \param[in]	_scw	ウィンドウの幅
//! \param[in]	_sch	ウィンドウの高さ
//! \return	DirectXのデバイスを返す
/*======================================================================*/
const LPDIRECT3DDEVICE9 CDirect3D::Initialize( const HWND _hwnd , const bool _fsc , const int _scw , const int _sch )
{

	m_lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !m_lpD3D )
	{
		// オブジェクト作成失敗
		MessageBox( NULL , "Direct3D の作成に失敗しました。" , "ERROR" , MB_OK | MB_ICONSTOP );
		// 終了する
		return nullptr;
	}
	// 使用するアダプタ番号
	m_adapter = 0;

	// ウインドウの作成が完了したので、Direct3D を初期化する
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp , sizeof( D3DPRESENT_PARAMETERS ) );
	// Direct3D 初期化パラメータの設定
	if ( _fsc )
	{
		// フルスクリーン : ほとんどのアダプタでサポートされているフォーマットを使用
		//d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// ウインドウ : 現在の画面モードを使用
		D3DDISPLAYMODE disp;
		// 現在の画面モードを取得
		m_lpD3D->GetAdapterDisplayMode( m_adapter , &disp );
		d3dpp.BackBufferFormat = disp.Format;
	}
	
	// 表示領域サイズの設定
	d3dpp.BackBufferWidth  = _scw;
	d3dpp.BackBufferHeight = _sch;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if ( !_fsc )
	{
		// ウインドウモード
		d3dpp.Windowed = 1;
	}

	// Z バッファの自動作成
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//D3DFMT_D16;

	//ﾊﾞｯｸﾊﾞｯﾌｧをﾛｯｸ可能にする(GetDCも可能になる)
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//ﾘｾｯﾄ用に状態情報を保存
	m_param = d3dpp;

	// デバイスの作成 - T&L HAL
	if ( FAILED( m_lpD3D->CreateDevice( m_adapter , D3DDEVTYPE_HAL , _hwnd , D3DCREATE_HARDWARE_VERTEXPROCESSING , &d3dpp , &m_lpD3DDevice ) ) )
	{
		// 失敗したので HAL で試行
		if ( FAILED( m_lpD3D->CreateDevice( m_adapter , D3DDEVTYPE_HAL , _hwnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &m_lpD3DDevice ) ) )
		{
			// 失敗したので REF で試行
			if ( FAILED( m_lpD3D->CreateDevice( m_adapter , D3DDEVTYPE_REF , _hwnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &m_lpD3DDevice ) ) )
			{
				// 結局失敗した
				MessageBox( NULL , "DirectX9が初期化できません。\n未対応のパソコンと思われます。" , "ERROR" , MB_OK | MB_ICONSTOP );
				SafeRelease( VARIABLE_DATA( m_lpD3D ) );
				// 終了する
				return nullptr;
			}
		}
	}

	// レンダリング・ステートを設定
	// Zバッファ有効化->前後関係の計算を正確にしてくれる
	m_lpD3DDevice->SetRenderState( D3DRS_ZENABLE	  , D3DZB_TRUE );
	m_lpD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE	   );

	// アルファブレンディング有効化
	m_lpD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );

	// アルファブレンディング方法を設定
	m_lpD3DDevice->SetRenderState( D3DRS_SRCBLEND  , D3DBLEND_SRCALPHA    );
	m_lpD3DDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );

	// レンダリング時のアルファ値の計算方法の設定
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP   , D3DTOP_MODULATE );
	// テクスチャの色を使用
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	// 頂点の色を使用
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );
	// レンダリング時の色の計算方法の設定
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_COLOROP   , D3DTOP_MODULATE );

	//裏面カリング
	m_lpD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );

	// フィルタ設定
	m_lpD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
	m_lpD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	m_lpD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );

	// ライト
	m_lpD3DDevice->SetRenderState( D3DRS_LIGHTING , TRUE );

	m_lpD3DDevice->GetBackBuffer( 0 , 0 , D3DBACKBUFFER_TYPE_MONO , &m_lpBackbuffer );

	// 頂点フォーマットの設定
	m_lpD3DDevice->SetFVF( FVF_Vertex );

	// スプライトの作成
	m_Sprite = new CSprite();

	// ライトマネージャの作成
    m_LightManager = new CLightManager();

	// フォントの作成
	D3DXCreateFont( m_lpD3DDevice ,	// デバイス
		24 ,						// 文字の高さ
		0 ,							// 文字幅
		FW_NORMAL ,					// フォントの太さ
		1 ,							// MIPMAPのレベル
		FALSE ,						// イタリック
		DEFAULT_CHARSET ,			// 文字セット
		OUT_DEFAULT_PRECIS ,		// 出力精度
		DEFAULT_QUALITY ,			// 出力品質
		DEFAULT_PITCH | FF_SWISS ,	// フォントピッチとファミリ 
		"ＭＳ Ｐゴシック" ,			// フォント名
		&m_pFont );					// フォントデバイス
	

	m_CamRx		= 0.0f;
	m_CamRy		= 0.0f;
	m_CamZoom	= 0.0f;


	DEBUG->PrintConsole( "DirectXの初期化に成功しました...\n" , CDebug::ConPreside );

	// 成功した場合初期化したインターフェースが帰る
	return m_lpD3DDevice;
	
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CDirect3D::~CDirect3D( void )
{
	SafeRelease( VARIABLE_DATA( m_pFont ) );
	SafeDelete( VARIABLE_DATA( m_Sprite ) );
	SafeDelete( VARIABLE_DATA( m_LightManager ) );
	SafeRelease( VARIABLE_DATA( m_lpD3DDevice ) );
}


/*=======================================================*/
//! \brief 描画開始処理
//! \brief 描画終了時にはEnd()をコールする
//! \return	DirectXのデバイスを返す
/*=======================================================*/
const LPDIRECT3DDEVICE9 CDirect3D::Begin( void )
{										
	m_lpD3DDevice->BeginScene();
	// バックバッファと Zバッファをクリア
	m_lpD3DDevice->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );					
	return m_lpD3DDevice;
}


/*=======================================================*/
//! \brief 描画終了処理
//! \brief 描画開始時にはBegin()をコールする
//! \return 成功 = true
//! \return 失敗(デバイスのロストなど) = false
/*=======================================================*/
bool CDirect3D::End( void )
{										
	m_lpD3DDevice->EndScene();

	
	// デバイスのロストを監視
	try
	{
		// バックバッファをプライマリバッファにコピー
 		m_lpD3DDevice->Present( NULL , NULL , NULL , NULL );
	}
	catch( std::exception ex )
	{
		m_lpD3DDevice->Reset( &m_param );
		CAPP->SetErrorFlag();
		ErrorMessage( "DirectXのデバイスがロストしました" );
		PostQuitMessage( 0 );
		return false;
	}

	return true;
}


/*============================================================*/
//! \brief 文字を画面に表示する
//! \param[in]	_str	表示する文字列
//! \param[in]	_x		表示する文字列のx座標
//! \param[in]	_y		表示する文字列のy座標
/*============================================================*/
void CDirect3D::DrawString( const std::string _str , const int _x , const int _y )
{
	RECT rc;
	rc.left   = _x;
	rc.top    = _y;
	rc.right  = WINDOW_WIDTH;
	rc.bottom = WINDOW_HEIGHT;
	m_pFont->DrawText( NULL , _str.c_str() , -1 , &rc , NULL , 0xFF88FF88 );

}


/*==========================================================*/
//! \brief 画面設定
//! \param[in]	_fovy	視野角
//! \param[in]	_asp	アスペクト比
//! \param[in]	_zn		Zバッファの0.0fの位置
//! \param[in]	_zf		Zバッファの1.0fの位置
/*==========================================================*/
void CDirect3D::SetProj( const float _fovy , const float _asp , const float _zn , const float _zf )
{
	D3DXMatrixPerspectiveFovLH( &m_mProj , D3DXToRadian( _fovy ) , _asp , _zn , _zf );
	m_lpD3DDevice->SetTransform( D3DTS_PROJECTION , &m_mProj ); 
}


/*================================*/
//! \brief カメラの更新
/*================================*/
void CDirect3D::SetCamera( void )
{
	CMatrix mPos, mZoom, mRot;
	mZoom.SetTrans( 0 , 0 , m_CamZoom );
	mPos.SetTrans( m_CamPos.x , m_CamPos.y , m_CamPos.z );
	mRot.SetRotate( m_CamRx , m_CamRy , m_CamRz );
	m_mViewSrc = mZoom * mRot * mPos;							// 視点行列の元

	m_mViewSrc = m_mViewSrc.GetInverse();						// 視点行列の作成(逆行列)			
	m_lpD3DDevice->SetTransform( D3DTS_VIEW , &m_mViewSrc );
}

