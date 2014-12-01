#include "stdafx.hpp"

#include <Windows.h>
#include "CKeyboard.hpp"


/*==============================================*/
//! CKeyboardのポインタの定義
/*==============================================*/
CKeyboard* CKeyboard::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CKeyboard::CKeyboard( void )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CKeyboard::~CKeyboard( void )
{
}


/*===============================*/
//! \brief 全てのキーの初期化
/*===============================*/
void CKeyboard::StateInitialize( void )
{
	for( auto& x : m_State )
	{
		x = KST_HOLD;
	}
}


/*==============================================*/
//! \brief 使用する全てのキーの状態を更新する
/*==============================================*/
void CKeyboard::AllStateUpdata( void )
{

	StateUpdata( KEY_STATE( VK_ESCAPE ) , KEY_ESCAPE );
	StateUpdata( KEY_STATE( VK_RETURN ) , KEY_RETURN );
	StateUpdata( KEY_STATE( VK_SHIFT  ) , KEY_SHIFT  );
	StateUpdata( KEY_STATE( VK_SPACE  ) , KEY_SPACE  );
	StateUpdata( KEY_STATE( 'W' ) , KEY_W );
	StateUpdata( KEY_STATE( 'A' ) , KEY_A );
	StateUpdata( KEY_STATE( 'D' ) , KEY_D );
	StateUpdata( KEY_STATE( 'S' ) , KEY_S );


	

//#ifdef _DEBUG

	// ボックス作成
	StateUpdata( KEY_STATE( 'B' ) , KEY_B );
	StateUpdata( KEY_STATE( 'N' ) , KEY_N );
	StateUpdata( KEY_STATE( 'M' ) , KEY_M );

	// 主人公強制転移
	StateUpdata( KEY_STATE( 'H' ) , KEY_H );
	StateUpdata( KEY_STATE( 'J' ) , KEY_J );
	StateUpdata( KEY_STATE( 'K' ) , KEY_K );

	// その他デバッグ
	StateUpdata( KEY_STATE( 'U' ) , KEY_U );
	StateUpdata( KEY_STATE( 'I' ) , KEY_I );
	StateUpdata( KEY_STATE( 'O' ) , KEY_O );
	StateUpdata( KEY_STATE( 'P' ) , KEY_P );

	// ロープ操作
	StateUpdata( KEY_STATE( 'R' ) , KEY_R );
	StateUpdata( KEY_STATE( 'G' ) , KEY_G );
	StateUpdata( KEY_STATE( 'F' ) , KEY_F );
	StateUpdata( KEY_STATE( 'V' ) , KEY_V );

	// DirectX,PhysXの描画フラグ反転
	StateUpdata( KEY_STATE( 'T' ) , KEY_T );
	StateUpdata( KEY_STATE( 'Y' ) , KEY_Y );

	// 監視カメラ関節操作
	StateUpdata( KEY_STATE( 'Z' ) , KEY_Z );
	StateUpdata( KEY_STATE( 'X' ) , KEY_X );
	
	// ステージ切り替え
	StateUpdata( KEY_STATE( '0' ) , KEY_0 );
	StateUpdata( KEY_STATE( '1' ) , KEY_1 );
	StateUpdata( KEY_STATE( '2' ) , KEY_2 );
	StateUpdata( KEY_STATE( '3' ) , KEY_3 );
	StateUpdata( KEY_STATE( '4' ) , KEY_4 );
	StateUpdata( KEY_STATE( '5' ) , KEY_5 );
	StateUpdata( KEY_STATE( '6' ) , KEY_6 );
	StateUpdata( KEY_STATE( '7' ) , KEY_7 );
	StateUpdata( KEY_STATE( '8' ) , KEY_8 );
	StateUpdata( KEY_STATE( '9' ) , KEY_9 );

	StateUpdata( KEY_STATE( VK_RIGHT ) , KEY_RIGHT );
	StateUpdata( KEY_STATE( VK_LEFT ) , KEY_LEFT );

//#endif



}


/*======================================================*/
//! \brief 1つのキーの状態を更新する
//! \param[in] _bPushFlag	キーを押しているかのフラグ
//! \param[in] _pKeyId		更新するキーのID(KeyTypes)
/*======================================================*/
void CKeyboard::StateUpdata( const bool _bPushFlag , const int _pKeyId )
{
	if( _bPushFlag )
	{ 
		if( m_State[_pKeyId] == KST_FREE ){ m_State[_pKeyId] = KST_PUSH; }
		else{								m_State[_pKeyId] = KST_HOLD; }
	}else{ 
		if( m_State[_pKeyId] == KST_HOLD ){ m_State[_pKeyId] = KST_PULL; }
		else{								m_State[_pKeyId] = KST_FREE; }
	}
}


/*========================================================*/
//! \bried 簡易のキー入力判定
//! \param[in]	_Id		調べたいキーのVirtualKeyCode
//! \return 押している   = true
//! \return 押していない = false
/*========================================================*/
bool CKeyboard::GetSimplifyState( const int _VirtualKeyCode )
{
	return KEY_STATE( _VirtualKeyCode ); 
}