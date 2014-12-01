#pragma once

/*===================================*/
//!< 前方宣言
/*===================================*/
class CDirect3D;
class CTexture;

class CTextureManager
{

	/*===================================================*/
	//!
	//! \file
	//! \class CTextureManager
	//! \brief <b>テクスチャの管理を行う</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/11/07
	//! \since 2014/11/07
	//!
	/*===================================================*/

public:

	/*==============================================*/
	//! \brief インスタンスを返す
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
	//! \brief デストラクタ
	/*=====================================*/
	~CTextureManager( void );


	/*=============================================================*/
	//! \brief テクスチャを読み込む
	//! \param[in]	*_FileName		読み込むテクスチャの名前
	//! \return 読み込んだテクスチャのポインタ
	/*=============================================================*/
	CTexture* Load( const std::string *_FileName );


	/*===========================================================*/
	//! \brief 指定されたテクスチャを削除する
	//! \param[in]	*_TargetTexture		削除するテクスチャ
	/*===========================================================*/
	void DeleteTexture( CTexture *_TargetTexture );


	/*======================================================*/
	//! \brief 読み込んでいるCTextureを全て解放する
	/*======================================================*/
	void AllClear( void );


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CTextureManager( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CTextureManager( const CTextureManager& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CTextureManager& operator = ( const CTextureManager& single );


	/*====================================================*/
	//! \brief ファイル名を返す
	//! \param[in]	_FilePass		ファイルパス
	//! \param[in]	_Split			区切り文字
	/*====================================================*/
	std::string GetFileName( std::string _FilePass , std::string _Split = "/" );


	// テクスチャの保存場所
	static const std::string TEXFILE;	//!< テクスチャを保存しているフォルダのパス

	//! \brief インスタンスのポインタ
	static CTextureManager *m_pInstance;	//!< CApplicationのポインタ

	CDirect3D *m_pd3d;						//!< CDirect3Dのポインタ

	std::map< std::string , CTexture* > m_TextureList;		//!< 現在管理しているテクスチャのリスト

	
};

/*=================================================*/
//! \def TEXTURE_MANAGER
//! \brief コール時に使用するマクロ
/*=================================================*/
#define TEXTURE_MANAGER CTextureManager::GetInstance()
