#pragma once

/*===================================*/
//!< 前方宣言
/*===================================*/
class CXFile;


class CXFileManager
{

	/*===========================================================*/
	//!
	//! \file
	//! \class CXFileManager
	//! \brief <b>CXFileの管理を行う</b>
	//! 
	//! \author 配布データ+Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/14
	//!
	/*===========================================================*/

public:

	/*==============================================*/
	//! \brief インスタンスを返す
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
	//! \brief デストラクタ
	/*=====================================*/
	~CXFileManager( void );


	/*===============================================================*/
	//! \brief CXFileの読み込み
	//! \param[in]		_lpDirect3D		DirectXのデバイス
	//! \param[in]		_FilePass		XFileがあるファイルのパス
	//! \return XFileのポインタ
	/*===============================================================*/
	CXFile * Load( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass );
	

	/*==================================================*/
	//! \brief データをmapの中から走査し返す
	//! \return 成功 = XFileのポインタ
	//! \return 失敗 = nullptr
	/*==================================================*/
	CXFile * Get( std::string _FilePass );

	/*======================================================*/
	//! \brief 読み込んでいるCXFileを全て解放する
	/*======================================================*/
	void AllClear( void );


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CXFileManager( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CXFileManager( const CXFileManager& single );

    /*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CXFileManager& operator = ( const CXFileManager& single );


	/*====================================================*/
	//! \brief ファイル名を返す
	//! \param[in]	_FilePass		ファイルパス
	//! \param[in]	_Split			区切り文字
	/*====================================================*/
	std::string GetFileName( std::string _FilePass , std::string _Split = "/" );


	std::map<std::string, CXFile *> m_xmap;		//!< XFileを格納

	//! \brief インスタンスのポインタ
	static CXFileManager *m_pInstance;		//!< CXFileManagerのポインタ


};

/*============================================*/
//! \def XFILE_MANAGER
//! \brief コール時に使用するマクロ 
/*============================================*/
#define XFILE_MANAGER CXFileManager::GetInstance()

