#include "stdafx.hpp"

#include "CTexture.hpp"
#include "CTextureManager.hpp"

/*=======================================*/
// CTextureManager�̃|�C���^�̒�`
/*=======================================*/
CTextureManager* CTextureManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CTextureManager::CTextureManager( void )
{
	m_pd3d = DIRECTX;
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CTextureManager::~CTextureManager( void )
{
	AllClear();
}


/*=============================================================*/
//! \brief �e�N�X�`����ǂݍ���
//! \param[in]	*_FileName		�ǂݍ��ރe�N�X�`���̖��O
/*=============================================================*/
CTexture* CTextureManager::Load( const std::string *_FileName ){

	std::string FilePass;
	FilePass = "Resource/Texture/" + *_FileName;
	

	// ���ɓǂݍ���ł��Ȃ���map���𑖍�
	std::map< std::string , CTexture* >::iterator it = m_TextureList.find( std::string( FilePass ) );
	if( it != m_TextureList.end() )
	{
		return it->second;
	}

	// �����ꍇ�V���ɓǂݍ���
	CTexture *lpTexture = new CTexture( m_pd3d->GetDevice() , FilePass );
	
	// �}�b�v�ɒǉ�����
	m_TextureList.insert( std::pair< std::string , CTexture* >( FilePass , lpTexture ) );

	return lpTexture;

}


/*===========================================================*/
//! \brief �w�肳�ꂽ�e�N�X�`�����폜����
//! \param[in]	*_TargetTexture		�폜����e�N�X�`��
/*===========================================================*/
void CTextureManager::DeleteTexture( CTexture *_TargetTexture )
{

	std::map< std::string, CTexture *>::iterator it = m_TextureList.begin();
	while( it != m_TextureList.end() )
	{
		if( _TargetTexture == it->second )
		{
			m_TextureList.erase( it );
			SafeDelete( _TargetTexture );
			return;
		}
	}

}


/*======================================================*/
//! \brief �ǂݍ���ł���CTexture��S�ĉ������
/*======================================================*/
void CTextureManager::AllClear( void )
{

	std::map< std::string, CTexture *>::iterator it;
	it = m_TextureList.begin();

	if( m_TextureList.size() != 0 )
	{
		DEBUG->PrintConsole( "===Release Texture===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		while( it != m_TextureList.end() )
		{
			std::string XfileName = GetFileName( it->first );
			SafeDelete( it->second , XfileName );
			it++;
		}
		m_TextureList.clear();
		DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );
	}

}


/*====================================================*/
//! \brief �t�@�C������Ԃ�
//! \param[in]	_FilePass		�t�@�C���p�X
//! \param[in]	_Split			��؂蕶��
/*====================================================*/
std::string CTextureManager::GetFileName( std::string _FilePass , std::string _Split )
{

	int SeekPos;
	std::string Name = _FilePass;

	while( 1 )
	{

		// ���̋�؂蕶�����ǂ��ɂ���̂������A�Ȃ��ꍇ�����𔲂���
		if( ( SeekPos = Name.find( _Split ) ) == std::string::npos ){ break; }

		// ��؂蕶����ǂݔ�΂�
		Name = Name.substr( SeekPos + _Split.length() );

	}

	return Name;

}