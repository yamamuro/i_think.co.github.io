#pragma once

#include "../Scene/Game/CGame.hpp" 

class CDebug
{

	/*============================================================*/
	//!
	//! \file
	//! \class CDebug
	//! \brief <b>�f�o�b�O���g�p���鏈�����Ǘ�</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/18
	//!
	/*============================================================*/

public:

	/*==============================================*/
	//! \enum STRING_COLOR
	//! \brief �R���\�[���̕����F
	/*==============================================*/
	enum STRING_COLOR
	{

		STR_BLACK = 0,													//!< ���������ɂ���
		STR_RED   = ( FOREGROUND_RED   | FOREGROUND_INTENSITY ) ,		//!< ������Ԃɂ���
		STR_GREEN = ( FOREGROUND_GREEN | FOREGROUND_INTENSITY ) ,		//!< ������΂ɂ���
		STR_BLUE  = ( FOREGROUND_BLUE  | FOREGROUND_INTENSITY ) ,		//!< ������ɂ���
		STR_WHITE = ( STR_RED | STR_GREEN | STR_BLUE ) ,				//!< �����𔒂ɂ���

		BACK_BLACK = 0,													//!< �w�i�����ɂ���
		BACK_RED   = ( BACKGROUND_RED   | BACKGROUND_INTENSITY ) ,		//!< �w�i��Ԃɂ���
		BACK_GREEN = ( BACKGROUND_GREEN | BACKGROUND_INTENSITY ) ,		//!< �w�i��΂ɂ���
		BACK_BLUE  = ( BACKGROUND_BLUE  | BACKGROUND_INTENSITY ) ,		//!< �w�i��ɂ���
		BACK_WHITE = ( BACK_RED | BACK_GREEN | BACK_BLUE )				//!< �w�i�𔒂ɂ���

	};

	// �p�ɂɎg�p����R���\�[���̐F�̍\��
	static const WORD ConPreside	= ( STR_GREEN );					//!< �i�s�Ɋւ���\��(�����������Ȃ�)
	static const WORD ConFence	    = ( STR_GREEN | STR_BLUE  );		//!< �͈͎w�肷��ꍇ�Ɏg�p
	static const WORD ConObjectMake = ( STR_RED   | STR_GREEN );		//!< �I�u�W�F�N�g�쐬��
	static const WORD ConFree		= ( STR_BLUE );						//!< �������̉��
	static const WORD ConWarning	= ( STR_WHITE | BACK_RED   );		//!< �ꎞ��~�Ȃǂ̗�O������

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
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
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CDebug( void );


	/*=======================================================*/
	//! \brief �R���\�[���ɕ�������o�͂���
	//! \param[in]	_String		�o�͂��镶����
	//! \param[in]	_Color		�F���
	/*=======================================================*/
	void PrintConsole( const std::string _String , const WORD _Color = ( STR_WHITE | BACK_BLACK ) );


	/*=================================*/
	//! \brief FPS��\������
	/*=================================*/
	void DrawFPS( void );


	/*==================================================*/
	//! \brief �f�o�b�O�L�[���g�p����
	//! \param[in]	_Room	���݂̕���ID
	/*==================================================*/
	void DebugKeyCood( int _Room );


	/*=============================================*/
	//! \brief �\�����f�o�b�O���ǂ����Ԃ�
	/*=============================================*/
	inline bool GetDebugFlag( void ){ return m_bDebugFlag; }


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CDebug( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CDebug( const CDebug& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CDebug& operator = ( const CDebug& single );


	//! \brief �C���X�^���X�̃|�C���^
	static CDebug *m_pInstance;						//!< CDebug�̃|�C���^

	bool m_bDebugFlag;								//!< �\�����f�o�b�O���ǂ����̃t���O
	HANDLE m_Console;								//!< �R���\�[�����i�[
	CONSOLE_SCREEN_BUFFER_INFO m_Backup;			//!< �R���\�[��������Ԃ̃f�[�^

	// FPS
	unsigned int m_BeforeTime;						//!< �O��̎���
	unsigned int m_FPSCounter;						//!< FPS�J�E���g�p
	unsigned int m_SaveFPS;							//!< FPS�\���p


	D3DXVECTOR3 BoxMakePos[CGame::ROOM_MAX][3];		//!< �f�o�b�O�ō쐬����{�b�N�X�̍��W
	NxVec3 PlayerPos[CGame::ROOM_MAX][3];		//!< �f�o�b�O�ňړ�����v���C���[�̍��W

};

/*=================================================*/
//! \def DEBUG
//! \brief �R�[�����Ɏg�p����}�N��
/*=================================================*/
#define DEBUG CDebug::GetInstance()