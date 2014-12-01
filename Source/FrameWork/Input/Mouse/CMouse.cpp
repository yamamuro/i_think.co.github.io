#include "stdafx.hpp"

#include "CMouse.hpp"


/*==============================================*/
// CMouse�̃|�C���^�̒�`
/*==============================================*/
CMouse* CMouse::m_pInstance = nullptr;



/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CMouse::CMouse( void )
{
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CMouse::~CMouse( void )
{
}


/*============================================================*/
//! \brief �}�E�X�̏�����
//! \param[in]	_hWnd		�E�B���h�E�n���h��
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
//! \brief �}�E�X�̏�Ԃ��X�V����
/*=========================================*/
void CMouse::AllStateUpdata( void )
{
	StateUpdata( MOUSE_STATE( VK_LBUTTON ) , B_LEFT  );
	StateUpdata( MOUSE_STATE( VK_RBUTTON ) , B_RIGHT );
}


/*=================================================================================*/
//! \brief 1�̃}�E�X�̏�Ԃ��X�V����
//! \param[in,out]  _bPushFlag	�N���b�N���Ă��邩�ǂ����̃t���O
//! \param[in]		_pButtonId	�X�V����}�E�X�̃{�^����ID���w��(MouseTypes)
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
