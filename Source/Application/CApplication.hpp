#pragma once

/*===================================*/
//!< �O���錾
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
	//!	\brief <b>�A�v���P�[�V�����̑S�̂��Ǘ�</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/18
	//!
	/*========================================================================*/

public:

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
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
	//! \brief �A�v���P�[�V�����̑S�̂̏�����
	//! \param[in]	_FullScreenFlag		�t���X�N���[�����ǂ����̃t���O
	//! \return	���� = true
	//! \return ���s = false
	/*==============================================================================*/
	bool InitializeApplication( const bool _FullScreenFlag );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CApplication( void );

	/*================================================*/
	//! \brief �A�v���P�[�V�����̉������
	/*================================================*/
	void Delete( void );

	
	/*=========================================================================*/
	//! \brief �E�B���h�E�v���V�[�W��
	//! \param[in]	_hWnd		�E�B���h�E�n���h��
	//! \param[in]	_Message	���b�Z�[�W�̎��ʎq
	//! \param[in]	_wParam		���b�Z�[�W�̍ŏ��̃p�����[�^
	//! \param[in]	_lParam		���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
	//! \return		���b�Z�[�W�����̌���
	/*=========================================================================*/
	LRESULT CALLBACK WindowProc( HWND _hWnd , UINT _Message , WPARAM _wParam , LPARAM _lParam );


	/*==========================================*/
	//! \brief �Q�[�����[�v
	//! \return	true  = ���[�v�p��
	//! \return false = ���[�v�I��
	/*==========================================*/
	bool GameLoop( void );

	/*=========================================================*/
	//! \brief ���݃C���X�^���X�𐶐����Ă��邩��Ԃ�
	//! \return �������Ă��� �@= true
	//! \return �������Ă��Ȃ� = false
	/*=========================================================*/
	inline static bool GetObjectExist( void ){ return m_pInstance == nullptr ? ( false ) : ( true ); }
	
	/*=================================================*/
	//! \brief �E�B���h�E�n���h����Ԃ�
	/*=================================================*/
	inline HWND GetHWND( void ){ return hWnd; } 


	/*=================================================*/
	//! \brief �E�B���h�E�T�C�Y���擾
	/*=================================================*/
	inline SIZE GetWindowSize( void ){ return m_WindowSize; }

	/*=================================================*/
	//! \brief �E�B���h�E�T�C�Y���Z�b�g
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
	//! \brief ���݂̃t���[������Ԃ�
	/*=================================================*/
	inline int GetFrameCounter( void ){ return FrameCounter; }

	/*=================================================*/
	//! \brief �t���[�����J�E���g�A�b�v����
	/*=================================================*/
	inline void FrameCountUp( void ){ FrameCounter++; }


	/*=================================================*/
	//! \brief CDebug��Ԃ�
	/*=================================================*/
	inline CDebug* GetCDebug( void ) const { return m_pDebug; }

	/*=================================================*/
	//! \brief Main�Ő錾����CDebug���i�[
	/*=================================================*/
	inline void SetCDebug( CDebug* _pDebug ){ m_pDebug = _pDebug; }


	/*=================================================*/
	//! \brief CDirect3D��Ԃ�
	/*=================================================*/
	inline CDirect3D* GetCDirect3D( void ) const { return m_pDirect3D; }


	/*=================================================*/
	//! \brief CKeyboard��Ԃ�
	/*=================================================*/
	inline CKeyboard* GetCKeyboard( void ) const { return m_pKeyboard; }


	/*=================================================*/
	//! \brief CMouse��Ԃ�
	/*=================================================*/
	inline CMouse* GetCMouse( void ) const { return m_pMouse; }


	/*=================================================*/
	//! \brief CObjectManager��Ԃ�
	/*=================================================*/
	inline CObjectManager* GetCObjectManager( void ) const { return m_pObjectManager; }
	

	/*=================================================*/
	//! \brief CTextureManager��Ԃ�
	/*=================================================*/
	inline CTextureManager* GetCTextureManager( void ) const { return m_pTextureManager; }

	/*=================================================*/
	//! \brief CSceneManager��Ԃ�
	/*=================================================*/
	inline CSceneManager* GetCSceneManager( void ) const { return m_pSceneManager; }


	/*=================================================*/
	//! \brief CPhysX��Ԃ�
	/*=================================================*/
	inline CPhysX* GetCPhysX( void ) const { return m_pPhysX; }


	/*=================================================*/
	//! \brief CSoundManager��Ԃ�
	/*=================================================*/
	inline CSoundManager* GetCSoundManager( void ) const { return m_pSoundManager; }


	/*=============================================*/
	//! \brief �G���[�������������Ԃ�
	/*=============================================*/
	inline bool GetErrorFlag( void ){ return m_ErrorFlag; }

	/*=============================================*/
	//! \brief �G���[���������������Z�b�g����
	/*=============================================*/
	inline void SetErrorFlag( bool _Flag = true ){ m_ErrorFlag = _Flag; }



	HWND hWnd;			//!< �E�B���h�E�n���h��
	HINSTANCE hInst;	//!< �C���X�^���X�n���h��
	MSG msg;            //!< ���b�Z�[�W�\����


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CApplication( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CApplication( const CApplication& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CApplication& operator = ( const CApplication& single );


	//! \brief �C���X�^���X�̃|�C���^
	static CApplication *m_pInstance;	//!< CApplication�̃|�C���^

	CDebug			*m_pDebug;			//!< �f�o�b�O�������s���N���X
	CDirect3D		*m_pDirect3D;		//!< DirectX�Ǘ��N���X
	CTextureManager *m_pTextureManager;	//!< �e�N�X�`���Ǘ��N���X
	CXFileManager	*m_pXFileManager;	//!< XFile�Ǘ��N���X
	CKeyboard		*m_pKeyboard;		//!< �L�[�{�[�h�̓��͊Ǘ��N���X
	CMouse			*m_pMouse;			//!< �}�E�X�̓��͊Ǘ��N���X
	CObjectManager	*m_pObjectManager;	//!< �I�u�W�F�N�g�Ǘ��N���X
	CSceneManager	*m_pSceneManager;	//!< �V�[���Ǘ��N���X
	CPhysX			*m_pPhysX;			//!< PhysX�Ǘ��N���X
	CSoundManager	*m_pSoundManager;	//!< �T�E���h�Ǘ��N���X

	unsigned long FrameCounter;			//!< �t���[���J�E���^
	bool FullScreenFlag;				//!< �t���X�N���[���ŋN�����邩�̃t���O

	SIZE m_WindowSize;					//!< �E�B���h�E�̕��A�������i�[
	
	bool m_ErrorFlag;					//!< �G���[�������������ǂ����̃t���O

};

/*=================================================*/
//! \def CAPP
//! \brief �R�[�����Ɏg�p����}�N��
/*=================================================*/
#define CAPP CApplication::GetInstance()
