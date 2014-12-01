#pragma once 

/*===================================*/
//!< 前方宣言
/*===================================*/
class CSceneBase;
class CDirect3D;
class CPhysX;
class CKeyboard;
class CMouse;
class CSound;
class CTexture;


class CSceneManager
{

	/*====================================================*/
	//!
	//! \file
	//! \class CSceneManager
	//! \brief <b>シーンマネージャクラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/12
	//!
	/*====================================================*/

public:

	/*================================*/
	//! \enum SCENE
	//! \brief 各シーン
	/*================================*/
	enum SCENE_TYPE
	{
		NONE,			//!< 初期値
		TITLE,			//!< タイトル画面
		GAME,			//!< ゲームプレイ中	
		ROOM_CREATE,	//!< 部屋作成
		ENDING,			//!< エンディング
		END,			//!< ゲームの終了
		SCENE_MAX		//!< シーンの最大数
	};


	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CSceneManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CSceneManager();
		}
		return m_pInstance;
	}


	/*====================================*/
	//! \brief 初期化
	/*====================================*/
	void Initialize( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CSceneManager( void );


	/*===============================================*/
	//! \brief シーンの変更を行う
	//! \param[in]	_Scene	変更するシーン
	/*===============================================*/
	void ChangeScene( int _Scene );


	/*=======================================*/
	//! \brief ゲームの更新を行う
	//! \return 成功 = true
	//! \return 失敗 = false
	/*=======================================*/
	bool Run( void );

	
	/*=================================================*/
	//! \brief マイフレームの最後に行う後始末
	/*=================================================*/
	void Rearrange( void );


	/*==============================================*/
	//! \brief 現在のシーンを返す
	/*==============================================*/
	CSceneBase* GetScene( void ){ return m_NowScene; }


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CSceneManager( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CSceneManager( const CSceneManager& single );
   
	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CSceneManager& operator = ( const CSceneManager& single );


	/*==========================================*/
	//! \brief シーンを変更する
	/*==========================================*/
	void SceneLoading( void );


	/*===============================================*/
	//! \brief シーン切り替え中の演出
	/*===============================================*/
	void LoadingScreen( void );


	/*=========================================================*/
	//! \brief 読み込み中にエラーが発生した際の処理
	/*=========================================================*/
	void ErrorExit( void );


	std::string m_SceneName[SCENE_MAX];		//!< デバッグで使用する各シーンの名前

	CDirect3D *m_pd3d;						//!< CDirect3Dのポインタ
	CPhysX *m_pPhysX;						//!< CPhysXのポインタ

	CKeyboard *m_pKeyboard;					//!< CKeyboardのポインタ
	CMouse *m_pMouse;						//!< CMouseのポインタ

	//! \brief インスタンスのポインタ　
	static CSceneManager *m_pInstance;		//!< CSceneManagerのポインタ

	CSceneBase *m_NowScene;					//!< 現在のシーンを格納

	int m_LoadScene;						//!< 読み込むシーン
	int m_LoadingAnimation;					//!< 読み込み中に使用するアニメーション番号

	CTexture *m_lpLoadingTexture;			//!< 読み込み中の文字


	/*======================================================*/
	//! \enum LOAD_FLAG
	//! \brief マルチスレッディング時に使用するフラグ
	/*======================================================*/
	enum LOAD_FLAG
	{
		LOADING,		//!< 読み込み中
		LOAD_END		//!< 読み込み終了
	};
	LOAD_FLAG m_LoadingFlag;				//!< 読み込みが終了したかのフラグ

	boost::thread *m_pLoadThread;			//!< 読み込みを行うスレッド

};

/*====================================*/
//! \def SCENE_MANAGER
//! \brief コール時に使用する
/*====================================*/
#define SCENE_MANAGER CSceneManager::GetInstance()
