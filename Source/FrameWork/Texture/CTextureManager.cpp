#include "stdafx.hpp"

#include "CTexture.hpp"
#include "CTextureManager.hpp"

/*=======================================*/
// CTextureManagerのポインタの定義
/*=======================================*/
CTextureManager* CTextureManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CTextureManager::CTextureManager( void )
{
	m_pd3d = DIRECTX;
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CTextureManager::~CTextureManager( void )
{
	AllClear();
}


/*=============================================================*/
//! \brief テクスチャを読み込む
//! \param[in]	*_FileName		読み込むテクスチャの名前
/*=============================================================*/
CTexture* CTextureManager::Load( const std::string *_FileName ){

	std::string FilePass;
	FilePass = "Resource/Texture/" + *_FileName;
	

	// 既に読み込んでいないかmap内を走査
	std::map< std::string , CTexture* >::iterator it = m_TextureList.find( std::string( FilePass ) );
	if( it != m_TextureList.end() )
	{
		return it->second;
	}

	// 無い場合新たに読み込む
	CTexture *lpTexture = new CTexture( m_pd3d->GetDevice() , FilePass );
	
	// マップに追加する
	m_TextureList.insert( std::pair< std::string , CTexture* >( FilePass , lpTexture ) );

	return lpTexture;

}


/*===========================================================*/
//! \brief 指定されたテクスチャを削除する
//! \param[in]	*_TargetTexture		削除するテクスチャ
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
//! \brief 読み込んでいるCTextureを全て解放する
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
//! \brief ファイル名を返す
//! \param[in]	_FilePass		ファイルパス
//! \param[in]	_Split			区切り文字
/*====================================================*/
std::string CTextureManager::GetFileName( std::string _FilePass , std::string _Split )
{

	int SeekPos;
	std::string Name = _FilePass;

	while( 1 )
	{

		// 次の区切り文字がどこにあるのか走査、ない場合処理を抜ける
		if( ( SeekPos = Name.find( _Split ) ) == std::string::npos ){ break; }

		// 区切り文字を読み飛ばす
		Name = Name.substr( SeekPos + _Split.length() );

	}

	return Name;

}