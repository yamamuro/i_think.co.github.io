#pragma once

/*===================================*/
//!< 前方宣言
/*===================================*/
class CObjectManager;
class CTextureManager;
class CXFileManager;
class CSceneManager;
class CDebug;
class CSoundManager;
class CKeyboard;
class CMouse;

#include "Direct3D\CDirect3D.hpp"


class CApplication
{

	/*========================================================================*/
	//!
	//! \file
	//! \class CApplication
	//!	\brief <b>アプリケーションの全体を管理</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/18
	//!
	/*========================================================================*/

public:

	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CApplication* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CApplication();
		}
		return m_pInstance;
	}

	/*==============================================================================*/
	//! \brief アプリケーションの全体の初期化
	//! \param[in]	_FullScreenFlag		フルスクリーンかどうかのフラグ
	//! \return	成功 = true
	//! \return 失敗 = false
	/*==============================================================================*/
	bool InitializeApplication( const bool _FullScreenFlag );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CApplication( void );

	/*================================================*/
	//! \brief アプリケーションの解放処理
	/*================================================*/
	void Delete( void );

	
	/*=========================================================================*/
	//! \brief ウィンドウプロシージャ
	//! \param[in]	_hWnd		ウィンドウハンドル
	//! \param[in]	_Message	メッセージの識別子
	//! \param[in]	_wParam		メッセージの最初のパラメータ
	//! \param[in]	_lParam		メッセージの 2 番目のパラメータ
	//! \return		メッセージ処理の結果
	/*=========================================================================*/
	LRESULT CALLBACK WindowProc( HWND _hWnd , UINT _Message , WPARAM _wParam , LPARAM _lParam );


	/*==========================================*/
	//! \brief ゲームループ
	//! \return	true  = ループ継続
	//! \return false = ループ終了
	/*==========================================*/
	bool GameLoop( void );

	/*=========================================================*/
	//! \brief 現在インスタンスを生成しているかを返す
	//! \return 生成している 　= true
	//! \return 生成していない = false
	/*=========================================================*/
	inline static bool GetObjectExist( void ){ return m_pInstance == nullptr ? ( false ) : ( true ); }
	
	/*=================================================*/
	//! \brief ウィンドウハンドルを返す
	/*=================================================*/
	inline HWND GetHWND( void ){ return hWnd; } 


	/*=================================================*/
	//! \brief ウィンドウサイズを取得
	/*=================================================*/
	inline SIZE GetWindowSize( void ){ return m_WindowSize; }

	/*=================================================*/
	//! \brief ウィンドウサイズをセット
	/*=================================================*/
	inline void SetWindowSize( void )
	{

		RECT Rect;
		GetClientRect( hWnd , &Rect );

		m_WindowSize.cx = Rect.right  - Rect.left;
		m_WindowSize.cy = Rect.bottom - Rect.top;

		m_pDirect3D->SetBackBufferSize( m_WindowSize );

	}


	/*=================================================*/
	//! \brief 現在のフレーム数を返す
	/*=================================================*/
	inline int GetFrameCounter( void ){ return FrameCounter; }

	/*=================================================*/
	//! \brief フレームをカウントアップする
	/*=================================================*/
	inline void FrameCountUp( void ){ FrameCounter++; }


	/*=================================================*/
	//! \brief CDebugを返す
	/*=================================================*/
	inline CDebug* GetCDebug( void ) const { return m_pDebug; }

	/*=================================================*/
	//! \brief Mainで宣言するCDebugを格納
	/*=================================================*/
	inline void SetCDebug( CDebug* _pDebug ){ m_pDebug = _pDebug; }


	/*=================================================*/
	//! \brief CDirect3Dを返す
	/*=================================================*/
	inline CDirect3D* GetCDirect3D( void ) const { return m_pDirect3D; }


	/*=================================================*/
	//! \brief CKeyboardを返す
	/*=================================================*/
	inline CKeyboard* GetCKeyboard( void ) const { return m_pKeyboard; }


	/*=================================================*/
	//! \brief CMouseを返す
	/*=================================================*/
	inline CMouse* GetCMouse( void ) const { return m_pMouse; }


	/*=================================================*/
	//! \brief CObjectManagerを返す
	/*=================================================*/
	inline CObjectManager* GetCObjectManager( void ) const { return m_pObjectManager; }
	

	/*=================================================*/
	//! \brief CTextureManagerを返す
	/*=================================================*/
	inline CTextureManager* GetCTextureManager( void ) const { return m_pTextureManager; }

	/*=================================================*/
	//! \brief CSceneManagerを返す
	/*=================================================*/
	inline CSceneManager* GetCSceneManager( void ) const { return m_pSceneManager; }


	/*=================================================*/
	//! \brief CPhysXを返す
	/*=================================================*/
	inline CPhysX* GetCPhysX( void ) const { return m_pPhysX; }


	/*=================================================*/
	//! \brief CSoundManagerを返す
	/*=================================================*/
	inline CSoundManager* GetCSoundManager( void ) const { return m_pSoundManager; }


	/*=============================================*/
	//! \brief エラーが発生したか返す
	/*=============================================*/
	inline bool GetErrorFlag( void ){ return m_ErrorFlag; }

	/*=============================================*/
	//! \brief エラーが発生したかをセットする
	/*=============================================*/
	inline void SetErrorFlag( bool _Flag = true ){ m_ErrorFlag = _Flag; }



	HWND hWnd;			//!< ウィンドウハンドル
	HINSTANCE hInst;	//!< インスタンスハンドル
	MSG msg;            //!< メッセージ構造体


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CApplication( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CApplication( const CApplication& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CApplication& operator = ( const CApplication& single );


	//! \brief インスタンスのポインタ
	static CApplication *m_pInstance;	//!< CApplicationのポインタ

	CDebug			*m_pDebug;			//!< デバッグ処理を行うクラス
	CDirect3D		*m_pDirect3D;		//!< DirectX管理クラス
	CTextureManager *m_pTextureManager;	//!< テクスチャ管理クラス
	CXFileManager	*m_pXFileManager;	//!< XFile管理クラス
	CKeyboard		*m_pKeyboard;		//!< キーボードの入力管理クラス
	CMouse			*m_pMouse;			//!< マウスの入力管理クラス
	CObjectManager	*m_pObjectManager;	//!< オブジェクト管理クラス
	CSceneManager	*m_pSceneManager;	//!< シーン管理クラス
	CPhysX			*m_pPhysX;			//!< PhysX管理クラス
	CSoundManager	*m_pSoundManager;	//!< サウンド管理クラス

	unsigned long FrameCounter;			//!< フレームカウンタ
	bool FullScreenFlag;				//!< フルスクリーンで起動するかのフラグ

	SIZE m_WindowSize;					//!< ウィンドウの幅、高さを格納
	
	bool m_ErrorFlag;					//!< エラーが発生したかどうかのフラグ

};

/*=================================================*/
//! \def CAPP
//! \brief コール時に使用するマクロ
/*=================================================*/
#define CAPP CApplication::GetInstance()
