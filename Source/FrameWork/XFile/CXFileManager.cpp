#include "stdafx.hpp"

#include "CXFileManager.hpp"
#include "CXFile.hpp"


/*=======================================*/
// CXFileManagerのポインタの定義
/*=======================================*/
CXFileManager* CXFileManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CXFileManager::CXFileManager( void )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CXFileManager::~CXFileManager( void )
{
	AllClear();
}


/*===============================================================*/
//! \brief CXFileの読み込み
//! \param[in]		_lpDirect3D		DirectXのデバイス
//! \param[in]		_FilePass		XFileがあるファイルのパス
//! \return XFileのポインタ
/*===============================================================*/
CXFile * CXFileManager::Load( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass )
{

	// 既に読み込んでいないかmap内を走査
	if ( m_xmap.find( _FilePass ) != m_xmap.end() )
	{
		return m_xmap[_FilePass];
	}

	// 無い場合新たに読み込む
	CXFile* lpx = new CXFile( _lpDirect3D , &_FilePass );
	m_xmap.insert( std::pair< std::string, CXFile *>( _FilePass , lpx ) );
	if( lpx == nullptr )
	{ 
		std::string sErrorMessage = _FilePass + "の読み込みに失敗しました\n";
		ErrorMessage( sErrorMessage );
	}
	return lpx;
	
}


/*==================================================*/
//! \brief データをmapの中から走査し返す
//! \return 成功 = XFileのポインタ
//! \return 失敗 = nullptr
/*==================================================*/
CXFile * CXFileManager::Get( std::string _FilePass )
{
	if( m_xmap.find( _FilePass ) != m_xmap.end() )
	{
		return m_xmap[_FilePass];
	}
	return nullptr;
}


/*======================================================*/
//! \brief 読み込んでいるCXFileを全て解放する
/*======================================================*/
void CXFileManager::AllClear( void )
{

	std::map< std::string, CXFile *>::iterator it;
	it = m_xmap.begin();

	if( m_xmap.size() != 0 )
	{
		DEBUG->PrintConsole( "===Release XFile===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		while( it != m_xmap.end() )
		{
			std::string XfileName = GetFileName( it->first );
			SafeDelete( it->second , XfileName );
			it++;
		}
		m_xmap.clear();
		DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );
	}

}


/*====================================================*/
//! \brief ファイル名を返す
//! \param[in]	_FilePass		ファイルパス
//! \param[in]	_Split			区切り文字
/*====================================================*/
std::string CXFileManager::GetFileName( std::string _FilePass , std::string _Split )
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