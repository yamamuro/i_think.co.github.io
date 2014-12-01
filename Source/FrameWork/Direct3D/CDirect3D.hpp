#pragma once

/*===================================*/
//!< 前方宣言
/*===================================*/
class CSprite;

#include "CLightManager.hpp"

#include "Wrapper/Matrix/CMatrix.hpp"
#include "System.hpp"


class CDirect3D
{

	/*==========================================================*/
	//!
	//! \file
	//! \class CDirect3D
	//! \brief <b>DirectXの管理</b>
	//!
	//! \author 配布データ+Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/04
	//!
	/*==========================================================*/

public:

	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CDirect3D* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CDirect3D();
		}
		return m_pInstance;
	}

	/*======================================================================*/
	//! \brief DirectXの初期化処理
	//! \param[in]	_hWnd	ウィンドウハンドル
	//! \param[in]	_fsc	フルスクリーンにするかどうかのフラグ
	//! \param[in]	_scw	ウィンドウの幅
	//! \param[in]	_sch	ウィンドウの高さ
	//! \return	DirectXのデバイスを返す
	/*======================================================================*/
	const LPDIRECT3DDEVICE9 Initialize( const HWND _hwnd , const bool _fsc = false , const int _scw = WINDOW_WIDTH , const int _sch = WINDOW_HEIGHT );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CDirect3D( void );


	/*=======================================================*/
	//! \brief 描画開始処理
	//! \brief 描画終了時にはEnd()をコールする
	//! \return	DirectXのデバイスを返す
	/*=======================================================*/
	const LPDIRECT3DDEVICE9 Begin( void );	


	/*============================*/
	// Zバッファをクリア
	/*============================*/
	inline void ClearZBuffer( void ){ m_lpD3DDevice->Clear( 0 , NULL , D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 ); }


	/*=======================================================*/
	//! \brief 描画終了処理
	//! \brief 描画開始時にはBegin()をコールする
	//! \return 成功 = true
	//! \return 失敗(デバイスのロストなど) = false
	/*=======================================================*/
	bool End( void );	


	/*============================================================*/
	//! \brief 文字を画面に表示する
	//! \param[in]	_str	表示する文字列
	//! \param[in]	_x		表示する文字列のx座標
	//! \param[in]	_y		表示する文字列のy座標
	/*============================================================*/
	void DrawString( const std::string _str , const int _x , const int _y );


	/*==========================================================*/
	//! \brief 画面設定
	//! \param[in]	_fovy	視野角
	//! \param[in]	_asp	アスペクト比
	//! \param[in]	_zn		Zバッファの0.0fの位置
	//! \param[in]	_zf		Zバッファの1.0fの位置
	/*==========================================================*/
	void SetProj( const float _fovy = 45.0f , const float _asp = 4.0f / 3.0f , const float _zn = 1.0f , const float _zf = 1000.0f );


	/*================================*/
	//! \brief カメラの更新
	/*================================*/
	void SetCamera( void );


	/*===============================================*/
	//! \brief 行列をセットする
	//! \param[in] *_lpmwt	セットする行列
	/*===============================================*/
	inline void SetWorldTrans( const D3DXMATRIX *_lpmwt ){ m_lpD3DDevice->SetTransform( D3DTS_WORLD , _lpmwt ); }
	

	/*=================================*/
	//! \brief D3DDeviceを返す
	/*=================================*/
	inline LPDIRECT3DDEVICE9 GetDevice( void ){ return m_lpD3DDevice; }


	/*=======================================================*/
	//! \brief カメラを指定座標にセットする
	//! \param[in]	_v	カメラをセットする座標
	/*=======================================================*/
	inline void SetCamPos( const D3DXVECTOR3 _v ){ m_CamPos = _v; }

	/*=========================================*/
	//! \brief カメラの座標を返す
	/*=========================================*/
	inline D3DXVECTOR3 GetCamPos( void ){ return m_CamPos; }


	/*=============================================================*/
	//! \brief カメラのズーム、引きをセットする
	//! \param[in]	_Zoom	カメラがどれだけ近寄るか
	/*=============================================================*/
	inline void SetZoom( const float _Zoom ){ m_CamZoom = _Zoom; }


	/*=======================================================*/
	//! \brief カメラを指定した方向へ向ける
	//! \param[in]	_v		カメラの回転角
	/*=======================================================*/
	inline void SetCamRot( const D3DXVECTOR3 _v ){ m_CamRx = _v.x; m_CamRy = _v.y; m_CamRz = _v.z; }

	/*=========================================*/
	//! \brief カメラの角度を返す
	/*=========================================*/
	inline D3DXVECTOR3 GetCamRot( void ){ return D3DXVECTOR3( m_CamRx , m_CamRy , m_CamRz ); }


	/*==============================================================*/
	//! \brief メッシュを描画するかのフラグを反転する
	/*==============================================================*/
	inline void NotDrawDirectX( void )
	{
#ifdef _DEBUG
		m_DrawDirectX = !m_DrawDirectX;
#else
		m_DrawDirectX = true;
#endif
	}


	/*=========================================*/
	//! \brief 描画フラグを返す
	/*=========================================*/
	inline bool GetDrawFlag( void ){ return m_DrawDirectX; }


	/*=================================================*/
	//! \brief CSpriteを返す
	/*=================================================*/
	inline CSprite* GetCSprite( void ) const { return m_Sprite; }


	/*=================================================*/
	//! \brief CLightManagerを返す
	/*=================================================*/
	inline CLightManager* GetCLightManager( void ) const { return m_LightManager; }


	/*================================================================================*/
	//! \brief ウィンドウのサイズが変更されたときバッファサイズを変更する
	//! \param[in]	_WindowSize		ウィンドウのサイズ
	/*================================================================================*/
	inline void SetBackBufferSize( const SIZE _WindowSize )
	{
		//m_param.BackBufferWidth = _WindowSize.cx;
		//m_param.BackBufferHeight = _WindowSize.cy;
	}


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CDirect3D( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CDirect3D( const CDirect3D& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CDirect3D& operator = ( const CDirect3D& single );


	//! \brief インスタンスのポインタ
	static CDirect3D *m_pInstance;				//!< DirectXのポインタ

	// 3D用の変数宣言
	LPDIRECT3D9				m_lpD3D;			//!< Direct3Dインターフェイス
	LPDIRECT3DDEVICE9		m_lpD3DDevice;		//!< Direct3DDeviceインターフェイス
	LPDIRECT3DSURFACE9		m_lpBackbuffer;		//!< バックバッファ
	int						m_adapter;			//!< アダプタ番号を格納
	D3DPRESENT_PARAMETERS	m_param;			//!< プレゼンテーションパラメータ
	LPD3DXFONT				m_pFont;			//!< フォント表示用
	CSprite					*m_Sprite;			//!< スプライトクラスのポインタ
	CLightManager			*m_LightManager;	//!< ライトを管理

	// カメラ用の変数宣言
	float m_CamRx;								//!< 注視点からのカメラの回転Y軸
	float m_CamRy;								//!< 注視点からのカメラの回転Y軸
	float m_CamZoom;							//!< 注視点とカメラの距離
	float m_CamRz;								//!< カメラの頭上の角度
	D3DXVECTOR3 m_CamPos;						//!< 注視点の位置
    CMatrix m_mViewSrc;							//!< 注視点の行列
	CMatrix m_mProj;							//!< 左手座標系パースペクティブ射影行列

	bool m_DrawDirectX;							//!< DirectXの描画フラグ

};

/*=========================================*/
//! \def DIRECTX
//! \brief コール時に使用する
/*=========================================*/
#define DIRECTX CDirect3D::GetInstance()