#pragma once

/*===================================*/
//!< �O���錾
/*===================================*/
class CDirect3D;
class CTexture;

class CTextureManager
{

	/*===================================================*/
	//!
	//! \file
	//! \class CTextureManager
	//! \brief <b>�e�N�X�`���̊Ǘ����s��</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/11/07
	//! \since 2014/11/07
	//!
	/*===================================================*/

public:

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
	/*==============================================*/
	static CTextureManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CTextureManager();
		}
		return m_pInstance;
	}

	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CTextureManager( void );


	/*=============================================================*/
	//! \brief �e�N�X�`����ǂݍ���
	//! \param[in]	*_FileName		�ǂݍ��ރe�N�X�`���̖��O
	//! \return �ǂݍ��񂾃e�N�X�`���̃|�C���^
	/*=============================================================*/
	CTexture* Load( const std::string *_FileName );


	/*===========================================================*/
	//! \brief �w�肳�ꂽ�e�N�X�`�����폜����
	//! \param[in]	*_TargetTexture		�폜����e�N�X�`��
	/*===========================================================*/
	void DeleteTexture( CTexture *_TargetTexture );


	/*======================================================*/
	//! \brief �ǂݍ���ł���CTexture��S�ĉ������
	/*======================================================*/
	void AllClear( void );


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CTextureManager( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CTextureManager( const CTextureManager& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CTextureManager& operator = ( const CTextureManager& single );


	/*====================================================*/
	//! \brief �t�@�C������Ԃ�
	//! \param[in]	_FilePass		�t�@�C���p�X
	//! \param[in]	_Split			��؂蕶��
	/*====================================================*/
	std::string GetFileName( std::string _FilePass , std::string _Split = "/" );


	// �e�N�X�`���̕ۑ��ꏊ
	static const std::string TEXFILE;	//!< �e�N�X�`����ۑ����Ă���t�H���_�̃p�X

	//! \brief �C���X�^���X�̃|�C���^
	static CTextureManager *m_pInstance;	//!< CApplication�̃|�C���^

	CDirect3D *m_pd3d;						//!< CDirect3D�̃|�C���^

	std::map< std::string , CTexture* > m_TextureList;		//!< ���݊Ǘ����Ă���e�N�X�`���̃��X�g

	
};

/*=================================================*/
//! \def TEXTURE_MANAGER
//! \brief �R�[�����Ɏg�p����}�N��
/*=================================================*/
#define TEXTURE_MANAGER CTextureManager::GetInstance()
