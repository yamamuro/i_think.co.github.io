#pragma once

/*===================================*/
//!< �O���錾
/*===================================*/
class CXFile;


class CXFileManager
{

	/*===========================================================*/
	//!
	//! \file
	//! \class CXFileManager
	//! \brief <b>CXFile�̊Ǘ����s��</b>
	//! 
	//! \author �z�z�f�[�^+Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/14
	//!
	/*===========================================================*/

public:

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
	/*==============================================*/
	static CXFileManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CXFileManager();
		}
		return m_pInstance;
	}

	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CXFileManager( void );


	/*===============================================================*/
	//! \brief CXFile�̓ǂݍ���
	//! \param[in]		_lpDirect3D		DirectX�̃f�o�C�X
	//! \param[in]		_FilePass		XFile������t�@�C���̃p�X
	//! \return XFile�̃|�C���^
	/*===============================================================*/
	CXFile * Load( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass );
	

	/*==================================================*/
	//! \brief �f�[�^��map�̒����瑖�����Ԃ�
	//! \return ���� = XFile�̃|�C���^
	//! \return ���s = nullptr
	/*==================================================*/
	CXFile * Get( std::string _FilePass );

	/*======================================================*/
	//! \brief �ǂݍ���ł���CXFile��S�ĉ������
	/*======================================================*/
	void AllClear( void );


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CXFileManager( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CXFileManager( const CXFileManager& single );

    /*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CXFileManager& operator = ( const CXFileManager& single );


	/*====================================================*/
	//! \brief �t�@�C������Ԃ�
	//! \param[in]	_FilePass		�t�@�C���p�X
	//! \param[in]	_Split			��؂蕶��
	/*====================================================*/
	std::string GetFileName( std::string _FilePass , std::string _Split = "/" );


	std::map<std::string, CXFile *> m_xmap;		//!< XFile���i�[

	//! \brief �C���X�^���X�̃|�C���^
	static CXFileManager *m_pInstance;		//!< CXFileManager�̃|�C���^


};

/*============================================*/
//! \def XFILE_MANAGER
//! \brief �R�[�����Ɏg�p����}�N�� 
/*============================================*/
#define XFILE_MANAGER CXFileManager::GetInstance()

