#pragma once

class CKeyboard
{

	/*===================================================*/
	//!
	//! \file
	//! \class CKeyboard
	//! \brief <b>キーボードからの入力を管理</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/04
	//!
	/*===================================================*/

public:

	/*===============================*/
	//! \enum KeyTypes
	//! \brief キーの種類
	/*===============================*/
	enum KeyTypes
	{

		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_RETURN,
		KEY_ESCAPE,
		KEY_SHIFT,
		KEY_CONTROL,
		KEY_SPACE,
		KEY_ALT,
		KEY_F4,
		KEY_MAX			//!< 最大値

	};


	/*===============================*/
	//! \enum KeyStateTypes
	//! \brief キーの状態
	/*===============================*/
	enum KeyStateTypes
	{
		KST_PUSH,	//!< 押した瞬間
		KST_HOLD,	//!< 押し続けている
		KST_PULL,	//!< 離した瞬間
		KST_FREE	//!< 押されていない
	};


	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CKeyboard* GetInstance()
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CKeyboard();
		}
		return m_pInstance;
	}


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CKeyboard( void );


	/*=======================================*/
	//! \brief 全てのキーの初期化
	/*=======================================*/
	void StateInitialize( void );


	/*======================================================*/
	//! \brief 使用する全てのキーの状態を更新する
	/*======================================================*/
	void AllStateUpdata( void );


	/*===========================================================*/
	//! \brief 指定したキーの状態を返す
	//! \param[in] _Id		調べたいキーのID(KeyTypes)
	//! \return キーの状態(KeyStateTypes)
	/*===========================================================*/
	inline int GetState( const int _Id ){ return m_State[_Id]; }


	/*================================================================*/
	//! \bried 簡易のキー入力判定
	//! \param[in]	_Id		調べたいキーのVirtualKeyCode
	//! \return 押している   = true
	//! \return 押していない = false
	/*================================================================*/
	bool GetSimplifyState( const int _VirtualKeyCode );


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CKeyboard( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CKeyboard( const CKeyboard& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CKeyboard& operator = ( const CKeyboard& single );


	/*==============================================================*/
	//! \brief 1つのキーの状態を更新する
	//! \param[in,out] _bPushFlag	キーを押しているかのフラグ
	//! \param[in]		_pKeyId		更新するキーのID(KeyTypes)
	/*==============================================================*/
	void StateUpdata( const bool _bPushFlag , const int _pKeyId );


	//! \brief インスタンスのポインタ
	static CKeyboard *m_pInstance;			//!< CKeyboardのポインタ

	int m_State[KEY_MAX];					//!< キーの状態を格納する

};

/*==================================================*/
//! \def KEYBOARD
//! \brief コール時に使用するマクロ
/*==================================================*/
#define KEYBOARD CKeyboard::GetInstance()

/*==================================================*/
//! \def KEY_STATE
//! \brief キーを押しているのか返す
/*==================================================*/
#define KEY_STATE( _Key ) ( GetAsyncKeyState( _Key ) && 0x800  ) 

