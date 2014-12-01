#pragma once 

#include "CSound.hpp"

class CSoundManager
{

	/*=================================================================*/
	//!
	//! \file
	//! \class CSoundManager
	//! \brief <b>サウンドを管理するマネージャクラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/06
	//! \since 2014/10/06
	//!
	/*=================================================================*/

public:

	/*==============================================*/
	//! \brief インスタンスを返す
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
	//! \brief 初期化
	/*============================*/
	void Initialize( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CSoundManager( void );


	/*===============================================================*/
	//! \brief 読み込み
	//! \param[in]	_FilePass		Waveファイルのパス
	//! \param[in]	_BuffNum		複製するバッファ数
	//! \return 作成したCSoundのポインタ
	/*===============================================================*/
	CSound* Load( std::string _FilePass , int _BuffNum );


	/*===============================================*/
	// 読み込んでいるファイルを全て解放する
	/*===============================================*/
	void ReleaseWaves( void );


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CSoundManager( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CSoundManager( const CSoundManager& single );
    
	/*===============================================*/
	//! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CSoundManager& operator = ( const CSoundManager& single );


	//! \brief インスタンスのポインタ
	static CSoundManager* m_pInstance;					//!< CSoundManagerのポインタ

	std::map< std::string , CSound* > m_SoundList;		//!< 読み込んだサウンドのリスト

	LPDIRECTSOUND8 lpDSound;			//!< DirectSoundオブジェクト
	LPDIRECTSOUNDBUFFER lpSPrimary;		//!< プライマリバッファ
	long Vol;			//!< PC本体の初期ボリューム
	long Pan;			//!< PC本体の初期パン

};

/*==========================================*/
//! \def SOUND_MANAGER
//! \brief コール時に使用するマクロ
/*==========================================*/
#define SOUND_MANAGER CSoundManager::GetInstance()
