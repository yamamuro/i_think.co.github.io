#include "stdafx.hpp"

#include "CMouse.hpp"


/*==============================================*/
// CMouseのポインタの定義
/*==============================================*/
CMouse* CMouse::m_pInstance = nullptr;



/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CMouse::CMouse( void )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CMouse::~CMouse( void )
{
}


/*============================================================*/
//! \brief マウスの初期化
//! \param[in]	_hWnd		ウィンドウハンドル
/*============================================================*/
void CMouse::Initialize( const HWND _hWnd )
{

	hWnd = _hWnd;

	for( auto& x : m_State )
	{
		x = MST_HOLD;
	}

	m_WheelMovement = 0;

}


/*=========================================*/
//! \brief マウスの状態を更新する
/*=========================================*/
void CMouse::AllStateUpdata( void )
{
	StateUpdata( MOUSE_STATE( VK_LBUTTON ) , B_LEFT  );
	StateUpdata( MOUSE_STATE( VK_RBUTTON ) , B_RIGHT );
}


/*=================================================================================*/
//! \brief 1つのマウスの状態を更新する
//! \param[in,out]  _bPushFlag	クリックしているかどうかのフラグ
//! \param[in]		_pButtonId	更新するマウスのボタンのIDを指定(MouseTypes)
/*=================================================================================*/
void CMouse::StateUpdata( const bool _bPushFlag , const int _pMouseId )
{
	if( _bPushFlag )
	{ 
		if( m_State[_pMouseId] == MST_FREE ){ m_State[_pMouseId] = MST_PUSH; }
		else{								  m_State[_pMouseId] = MST_HOLD; }
	}else{ 
		if( m_State[_pMouseId] == MST_HOLD ){ m_State[_pMouseId] = MST_PULL; }
		else{								  m_State[_pMouseId] = MST_FREE; }
	}
}
