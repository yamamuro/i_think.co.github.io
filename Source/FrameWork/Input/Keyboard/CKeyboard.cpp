#include "stdafx.hpp"

#include <Windows.h>
#include "CKeyboard.hpp"


/*==============================================*/
//! CKeyboard�̃|�C���^�̒�`
/*==============================================*/
CKeyboard* CKeyboard::m_pInstance = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CKeyboard::CKeyboard( void )
{
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CKeyboard::~CKeyboard( void )
{
}


/*===============================*/
//! \brief �S�ẴL�[�̏�����
/*===============================*/
void CKeyboard::StateInitialize( void )
{
	for( auto& x : m_State )
	{
		x = KST_HOLD;
	}
}


/*==============================================*/
//! \brief �g�p����S�ẴL�[�̏�Ԃ��X�V����
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

	// �{�b�N�X�쐬
	StateUpdata( KEY_STATE( 'B' ) , KEY_B );
	StateUpdata( KEY_STATE( 'N' ) , KEY_N );
	StateUpdata( KEY_STATE( 'M' ) , KEY_M );

	// ��l�������]��
	StateUpdata( KEY_STATE( 'H' ) , KEY_H );
	StateUpdata( KEY_STATE( 'J' ) , KEY_J );
	StateUpdata( KEY_STATE( 'K' ) , KEY_K );

	// ���̑��f�o�b�O
	StateUpdata( KEY_STATE( 'U' ) , KEY_U );
	StateUpdata( KEY_STATE( 'I' ) , KEY_I );
	StateUpdata( KEY_STATE( 'O' ) , KEY_O );
	StateUpdata( KEY_STATE( 'P' ) , KEY_P );

	// ���[�v����
	StateUpdata( KEY_STATE( 'R' ) , KEY_R );
	StateUpdata( KEY_STATE( 'G' ) , KEY_G );
	StateUpdata( KEY_STATE( 'F' ) , KEY_F );
	StateUpdata( KEY_STATE( 'V' ) , KEY_V );

	// DirectX,PhysX�̕`��t���O���]
	StateUpdata( KEY_STATE( 'T' ) , KEY_T );
	StateUpdata( KEY_STATE( 'Y' ) , KEY_Y );

	// �Ď��J�����֐ߑ���
	StateUpdata( KEY_STATE( 'Z' ) , KEY_Z );
	StateUpdata( KEY_STATE( 'X' ) , KEY_X );
	
	// �X�e�[�W�؂�ւ�
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
//! \brief 1�̃L�[�̏�Ԃ��X�V����
//! \param[in] _bPushFlag	�L�[�������Ă��邩�̃t���O
//! \param[in] _pKeyId		�X�V����L�[��ID(KeyTypes)
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
//! \bried �ȈՂ̃L�[���͔���
//! \param[in]	_Id		���ׂ����L�[��VirtualKeyCode
//! \return �����Ă���   = true
//! \return �����Ă��Ȃ� = false
/*========================================================*/
bool CKeyboard::GetSimplifyState( const int _VirtualKeyCode )
{
	return KEY_STATE( _VirtualKeyCode ); 
}