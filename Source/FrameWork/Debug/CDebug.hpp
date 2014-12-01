#pragma once

#include "../Scene/Game/CGame.hpp" 

class CDebug
{

	/*============================================================*/
	//!
	//! \file
	//! \class CDebug
	//! \brief <b>デバッグ時使用する処理を管理</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/18
	//!
	/*============================================================*/

public:

	/*==============================================*/
	//! \enum STRING_COLOR
	//! \brief コンソールの文字色
	/*==============================================*/
	enum STRING_COLOR
	{

		STR_BLACK = 0,													//!< 文字を黒にする
		STR_RED   = ( FOREGROUND_RED   | FOREGROUND_INTENSITY ) ,		//!< 文字を赤にする
		STR_GREEN = ( FOREGROUND_GREEN | FOREGROUND_INTENSITY ) ,		//!< 文字を緑にする
		STR_BLUE  = ( FOREGROUND_BLUE  | FOREGROUND_INTENSITY ) ,		//!< 文字を青にする
		STR_WHITE = ( STR_RED | STR_GREEN | STR_BLUE ) ,				//!< 文字を白にする

		BACK_BLACK = 0,													//!< 背景を黒にする
		BACK_RED   = ( BACKGROUND_RED   | BACKGROUND_INTENSITY ) ,		//!< 背景を赤にする
		BACK_GREEN = ( BACKGROUND_GREEN | BACKGROUND_INTENSITY ) ,		//!< 背景を緑にする
		BACK_BLUE  = ( BACKGROUND_BLUE  | BACKGROUND_INTENSITY ) ,		//!< 背景を青にする
		BACK_WHITE = ( BACK_RED | BACK_GREEN | BACK_BLUE )				//!< 背景を白にする

	};

	// 頻繁に使用するコンソールの色の構成
	static const WORD ConPreside	= ( STR_GREEN );					//!< 進行に関する表示(初期化成功など)
	static const WORD ConFence	    = ( STR_GREEN | STR_BLUE  );		//!< 範囲指定する場合に使用
	static const WORD ConObjectMake = ( STR_RED   | STR_GREEN );		//!< オブジェクト作成時
	static const WORD ConFree		= ( STR_BLUE );						//!< メモリの解放
	static const WORD ConWarning	= ( STR_WHITE | BACK_RED   );		//!< 一時停止などの例外発生時

	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CDebug* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CDebug();
		}
		return m_pInstance;
	}


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CDebug( void );


	/*=======================================================*/
	//! \brief コンソールに文字列を出力する
	//! \param[in]	_String		出力する文字列
	//! \param[in]	_Color		色情報
	/*=======================================================*/
	void PrintConsole( const std::string _String , const WORD _Color = ( STR_WHITE | BACK_BLACK ) );


	/*=================================*/
	//! \brief FPSを表示する
	/*=================================*/
	void DrawFPS( void );


	/*==================================================*/
	//! \brief デバッグキーを使用する
	//! \param[in]	_Room	現在の部屋ID
	/*==================================================*/
	void DebugKeyCood( int _Room );


	/*=============================================*/
	//! \brief 構成がデバッグかどうか返す
	/*=============================================*/
	inline bool GetDebugFlag( void ){ return m_bDebugFlag; }


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CDebug( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CDebug( const CDebug& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CDebug& operator = ( const CDebug& single );


	//! \brief インスタンスのポインタ
	static CDebug *m_pInstance;						//!< CDebugのポインタ

	bool m_bDebugFlag;								//!< 構成がデバッグかどうかのフラグ
	HANDLE m_Console;								//!< コンソールを格納
	CONSOLE_SCREEN_BUFFER_INFO m_Backup;			//!< コンソール初期状態のデータ

	// FPS
	unsigned int m_BeforeTime;						//!< 前回の時間
	unsigned int m_FPSCounter;						//!< FPSカウント用
	unsigned int m_SaveFPS;							//!< FPS表示用


	D3DXVECTOR3 BoxMakePos[CGame::ROOM_MAX][3];		//!< デバッグで作成するボックスの座標
	NxVec3 PlayerPos[CGame::ROOM_MAX][3];		//!< デバッグで移動するプレイヤーの座標

};

/*=================================================*/
//! \def DEBUG
//! \brief コール時に使用するマクロ
/*=================================================*/
#define DEBUG CDebug::GetInstance()