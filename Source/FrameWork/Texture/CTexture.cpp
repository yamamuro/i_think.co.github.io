#include "stdafx.hpp"

#include "CTexture.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CTexture::CTexture( void )
	: m_lpTexture( nullptr )
	, m_lpBackup( nullptr )
{
}

/*====================================================================*/
//! \brief コンストラクタ
//! \param[in]		_lpDirect3D		DirectXのデバイス
//! \param[in]		_FilePass		テクスチャがあるファイルのパス
/*====================================================================*/
CTexture::CTexture( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass )
	: m_lpTexture( nullptr )
	, m_lpBackup( nullptr )
{
	Load( _lpDirect3D , _FilePass );
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CTexture::~CTexture( void )
{
	m_lpTexture = m_lpBackup;
	SafeRelease( VARIABLE_DATA( m_lpTexture ) );
	m_lpBackup = nullptr;
}


/*==============================================================================*/
//! \brief テクスチャを読み込む
//! \param[in]		_lpDirect3D		DirectXのデバイス
//! \param[in]		_FilePass		テクスチャがあるファイルのパス
/*==============================================================================*/
void CTexture::Load( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass )
{
	if( FAILED( D3DXCreateTextureFromFile( _lpDirect3D , _FilePass.c_str() , &m_lpTexture ) ) )
	{
		m_lpTexture = nullptr;
	}
	// バックアップを保存しておく
	m_lpBackup = m_lpTexture;
}



