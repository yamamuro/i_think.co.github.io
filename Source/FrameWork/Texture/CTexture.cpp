#include "stdafx.hpp"

#include "CTexture.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CTexture::CTexture( void )
	: m_lpTexture( nullptr )
	, m_lpBackup( nullptr )
{
}

/*====================================================================*/
//! \brief �R���X�g���N�^
//! \param[in]		_lpDirect3D		DirectX�̃f�o�C�X
//! \param[in]		_FilePass		�e�N�X�`��������t�@�C���̃p�X
/*====================================================================*/
CTexture::CTexture( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass )
	: m_lpTexture( nullptr )
	, m_lpBackup( nullptr )
{
	Load( _lpDirect3D , _FilePass );
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CTexture::~CTexture( void )
{
	m_lpTexture = m_lpBackup;
	SafeRelease( VARIABLE_DATA( m_lpTexture ) );
	m_lpBackup = nullptr;
}


/*==============================================================================*/
//! \brief �e�N�X�`����ǂݍ���
//! \param[in]		_lpDirect3D		DirectX�̃f�o�C�X
//! \param[in]		_FilePass		�e�N�X�`��������t�@�C���̃p�X
/*==============================================================================*/
void CTexture::Load( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass )
{
	if( FAILED( D3DXCreateTextureFromFile( _lpDirect3D , _FilePass.c_str() , &m_lpTexture ) ) )
	{
		m_lpTexture = nullptr;
	}
	// �o�b�N�A�b�v��ۑ����Ă���
	m_lpBackup = m_lpTexture;
}



