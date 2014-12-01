#pragma once

class CKeyboard
{

	/*===================================================*/
	//!
	//! \file
	//! \class CKeyboard
	//! \brief <b>�L�[�{�[�h����̓��͂��Ǘ�</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/04
	//!
	/*===================================================*/

public:

	/*===============================*/
	//! \enum KeyTypes
	//! \brief �L�[�̎��
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
		KEY_MAX			//!< �ő�l

	};


	/*===============================*/
	//! \enum KeyStateTypes
	//! \brief �L�[�̏��
	/*===============================*/
	enum KeyStateTypes
	{
		KST_PUSH,	//!< �������u��
		KST_HOLD,	//!< ���������Ă���
		KST_PULL,	//!< �������u��
		KST_FREE	//!< ������Ă��Ȃ�
	};


	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
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
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CKeyboard( void );


	/*=======================================*/
	//! \brief �S�ẴL�[�̏�����
	/*=======================================*/
	void StateInitialize( void );


	/*======================================================*/
	//! \brief �g�p����S�ẴL�[�̏�Ԃ��X�V����
	/*======================================================*/
	void AllStateUpdata( void );


	/*===========================================================*/
	//! \brief �w�肵���L�[�̏�Ԃ�Ԃ�
	//! \param[in] _Id		���ׂ����L�[��ID(KeyTypes)
	//! \return �L�[�̏��(KeyStateTypes)
	/*===========================================================*/
	inline int GetState( const int _Id ){ return m_State[_Id]; }


	/*================================================================*/
	//! \bried �ȈՂ̃L�[���͔���
	//! \param[in]	_Id		���ׂ����L�[��VirtualKeyCode
	//! \return �����Ă���   = true
	//! \return �����Ă��Ȃ� = false
	/*================================================================*/
	bool GetSimplifyState( const int _VirtualKeyCode );


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CKeyboard( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CKeyboard( const CKeyboard& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CKeyboard& operator = ( const CKeyboard& single );


	/*==============================================================*/
	//! \brief 1�̃L�[�̏�Ԃ��X�V����
	//! \param[in,out] _bPushFlag	�L�[�������Ă��邩�̃t���O
	//! \param[in]		_pKeyId		�X�V����L�[��ID(KeyTypes)
	/*==============================================================*/
	void StateUpdata( const bool _bPushFlag , const int _pKeyId );


	//! \brief �C���X�^���X�̃|�C���^
	static CKeyboard *m_pInstance;			//!< CKeyboard�̃|�C���^

	int m_State[KEY_MAX];					//!< �L�[�̏�Ԃ��i�[����

};

/*==================================================*/
//! \def KEYBOARD
//! \brief �R�[�����Ɏg�p����}�N��
/*==================================================*/
#define KEYBOARD CKeyboard::GetInstance()

/*==================================================*/
//! \def KEY_STATE
//! \brief �L�[�������Ă���̂��Ԃ�
/*==================================================*/
#define KEY_STATE( _Key ) ( GetAsyncKeyState( _Key ) && 0x800  ) 

