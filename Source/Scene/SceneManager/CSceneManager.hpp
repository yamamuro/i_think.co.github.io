#pragma once 

/*===================================*/
//!< �O���錾
/*===================================*/
class CSceneBase;
class CDirect3D;
class CPhysX;
class CKeyboard;
class CMouse;
class CSound;
class CTexture;


class CSceneManager
{

	/*====================================================*/
	//!
	//! \file
	//! \class CSceneManager
	//! \brief <b>�V�[���}�l�[�W���N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/12
	//!
	/*====================================================*/

public:

	/*================================*/
	//! \enum SCENE
	//! \brief �e�V�[��
	/*================================*/
	enum SCENE_TYPE
	{
		NONE,			//!< �����l
		TITLE,			//!< �^�C�g�����
		GAME,			//!< �Q�[���v���C��	
		ROOM_CREATE,	//!< �����쐬
		ENDING,			//!< �G���f�B���O
		END,			//!< �Q�[���̏I��
		SCENE_MAX		//!< �V�[���̍ő吔
	};


	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
	/*==============================================*/
	static CSceneManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CSceneManager();
		}
		return m_pInstance;
	}


	/*====================================*/
	//! \brief ������
	/*====================================*/
	void Initialize( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CSceneManager( void );


	/*===============================================*/
	//! \brief �V�[���̕ύX���s��
	//! \param[in]	_Scene	�ύX����V�[��
	/*===============================================*/
	void ChangeScene( int _Scene );


	/*=======================================*/
	//! \brief �Q�[���̍X�V���s��
	//! \return ���� = true
	//! \return ���s = false
	/*=======================================*/
	bool Run( void );

	
	/*=================================================*/
	//! \brief �}�C�t���[���̍Ō�ɍs����n��
	/*=================================================*/
	void Rearrange( void );


	/*==============================================*/
	//! \brief ���݂̃V�[����Ԃ�
	/*==============================================*/
	CSceneBase* GetScene( void ){ return m_NowScene; }


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CSceneManager( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CSceneManager( const CSceneManager& single );
   
	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CSceneManager& operator = ( const CSceneManager& single );


	/*==========================================*/
	//! \brief �V�[����ύX����
	/*==========================================*/
	void SceneLoading( void );


	/*===============================================*/
	//! \brief �V�[���؂�ւ����̉��o
	/*===============================================*/
	void LoadingScreen( void );


	/*=========================================================*/
	//! \brief �ǂݍ��ݒ��ɃG���[�����������ۂ̏���
	/*=========================================================*/
	void ErrorExit( void );


	std::string m_SceneName[SCENE_MAX];		//!< �f�o�b�O�Ŏg�p����e�V�[���̖��O

	CDirect3D *m_pd3d;						//!< CDirect3D�̃|�C���^
	CPhysX *m_pPhysX;						//!< CPhysX�̃|�C���^

	CKeyboard *m_pKeyboard;					//!< CKeyboard�̃|�C���^
	CMouse *m_pMouse;						//!< CMouse�̃|�C���^

	//! \brief �C���X�^���X�̃|�C���^�@
	static CSceneManager *m_pInstance;		//!< CSceneManager�̃|�C���^

	CSceneBase *m_NowScene;					//!< ���݂̃V�[�����i�[

	int m_LoadScene;						//!< �ǂݍ��ރV�[��
	int m_LoadingAnimation;					//!< �ǂݍ��ݒ��Ɏg�p����A�j���[�V�����ԍ�

	CTexture *m_lpLoadingTexture;			//!< �ǂݍ��ݒ��̕���


	/*======================================================*/
	//! \enum LOAD_FLAG
	//! \brief �}���`�X���b�f�B���O���Ɏg�p����t���O
	/*======================================================*/
	enum LOAD_FLAG
	{
		LOADING,		//!< �ǂݍ��ݒ�
		LOAD_END		//!< �ǂݍ��ݏI��
	};
	LOAD_FLAG m_LoadingFlag;				//!< �ǂݍ��݂��I���������̃t���O

	boost::thread *m_pLoadThread;			//!< �ǂݍ��݂��s���X���b�h

};

/*====================================*/
//! \def SCENE_MANAGER
//! \brief �R�[�����Ɏg�p����
/*====================================*/
#define SCENE_MANAGER CSceneManager::GetInstance()
