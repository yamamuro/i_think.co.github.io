#pragma once 

#include "CSound.hpp"

class CSoundManager
{

	/*=================================================================*/
	//!
	//! \file
	//! \class CSoundManager
	//! \brief <b>�T�E���h���Ǘ�����}�l�[�W���N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/06
	//! \since 2014/10/06
	//!
	/*=================================================================*/

public:

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
	/*==============================================*/
	static CSoundManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CSoundManager();
		}
		return m_pInstance;
	}

	/*============================*/
	//! \brief ������
	/*============================*/
	void Initialize( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CSoundManager( void );


	/*===============================================================*/
	//! \brief �ǂݍ���
	//! \param[in]	_FilePass		Wave�t�@�C���̃p�X
	//! \param[in]	_BuffNum		��������o�b�t�@��
	//! \return �쐬����CSound�̃|�C���^
	/*===============================================================*/
	CSound* Load( std::string _FilePass , int _BuffNum );


	/*===============================================*/
	// �ǂݍ���ł���t�@�C����S�ĉ������
	/*===============================================*/
	void ReleaseWaves( void );


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CSoundManager( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CSoundManager( const CSoundManager& single );
    
	/*===============================================*/
	//! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CSoundManager& operator = ( const CSoundManager& single );


	//! \brief �C���X�^���X�̃|�C���^
	static CSoundManager* m_pInstance;					//!< CSoundManager�̃|�C���^

	std::map< std::string , CSound* > m_SoundList;		//!< �ǂݍ��񂾃T�E���h�̃��X�g

	LPDIRECTSOUND8 lpDSound;			//!< DirectSound�I�u�W�F�N�g
	LPDIRECTSOUNDBUFFER lpSPrimary;		//!< �v���C�}���o�b�t�@
	long Vol;			//!< PC�{�̂̏����{�����[��
	long Pan;			//!< PC�{�̂̏����p��

};

/*==========================================*/
//! \def SOUND_MANAGER
//! \brief �R�[�����Ɏg�p����}�N��
/*==========================================*/
#define SOUND_MANAGER CSoundManager::GetInstance()
