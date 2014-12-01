#pragma once

/*===================================*/
//!< 前方宣言
/*===================================*/
class CObjectBase;
class CGimmickBase;
class CPlayer;
class CPhysX;


class CObjectManager
{

	/*========================================================*/
	//!
	//! \file
	//!	\class CObjectManager
	//! \brief <b>全てのオブジェクトの管理を行う</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/14
	//!
	/*========================================================*/

public:

	/*================================================*/
	//! \enum DATA_STRUCTURE
	//! \brief 外部ファイルのデータの格納順
	/*================================================*/
	enum DATA_STRUCTURE
	{

		OBJECT_NAME,		//!< オブジェクトの名前
		POS_X,				//!< 初期X座標
		POS_Y,				//!< 初期Y座標
		POS_Z,				//!< 初期Z座標
		ROT_X,				//!< X軸の初期回転角度
		ROT_Y,				//!< Y軸の初期回転角度
		ROT_Z,				//!< Z軸の初期回転角度
		SCALE_X,			//!< X軸の拡大率
		SCALE_Y,			//!< Y軸の拡大率
		SCALE_Z,			//!< Z軸の拡大率
		GIMMICK_ID,			//!< ギミックを動作させるギミックのID
		GIMMICK_RECEVER,	//!< どのギミックと連動して動作するのか
		DATA_STRUCTURE_MAX	//!< 最大値

	};

	/*==================================================================================*/
	//! \struct LOAD_OBJECT_DATA
	//! \brief <b>外部ファイルから読み込むデータを格納する文字列状態の構造体</b>
	/*==================================================================================*/
	typedef struct LOAD_OBJECT_DATA
	{
		std::string Str[DATA_STRUCTURE_MAX];		//!< データを格納する配列
	}LOAD_OBJECT_DATA;

	/*====================================================================*/
	//! \struct OBJECT_DATA
	//! \brief <b>読み込んだデータを実際に使えるデータにする</b>
	/*====================================================================*/
	typedef struct OBJECT_DATA
	{

		std::string Name;			//!< オブジェクト名
		D3DXVECTOR3 Pos;			//!< 初期座標
		D3DXVECTOR3 Rot;			//!< 回転角度
		D3DXVECTOR3 Scaling;		//!< 拡大率
		int GimmickID;				//!< ギミックのID
		int Connectoror;			//!< 接続するギミックのID

		/*==============================================================*/
		//! ￥brief 初期化
		//! \param[in]		_Data		初期化するためのデータ
		/*==============================================================*/
		bool Init( LOAD_OBJECT_DATA _Data )
		{

			bool InitFlag = true;

			try
			{

				Name = _Data.Str[CObjectManager::OBJECT_NAME];

				Pos = D3DXVECTOR3( std::stof( _Data.Str[CObjectManager::POS_X] )
								 , std::stof( _Data.Str[CObjectManager::POS_Y] )
								 , std::stof( _Data.Str[CObjectManager::POS_Z] ) );

				Rot = D3DXVECTOR3( std::stof( _Data.Str[CObjectManager::ROT_X] )
								 , std::stof( _Data.Str[CObjectManager::ROT_Y] )
								 , std::stof( _Data.Str[CObjectManager::ROT_Z] ) );

				Scaling = D3DXVECTOR3( std::stof( _Data.Str[CObjectManager::SCALE_X] )
									 , std::stof( _Data.Str[CObjectManager::SCALE_Y] )
									 , std::stof( _Data.Str[CObjectManager::SCALE_Z] ) );

				GimmickID = std::atoi( _Data.Str[CObjectManager::GIMMICK_ID].c_str() );
				Connectoror = std::atoi( _Data.Str[CObjectManager::GIMMICK_RECEVER].c_str() );

			}
			catch( std::exception )
			{ 
				InitFlag = false;
			}


			return InitFlag;

		}

	}OBJECT_DATA;


	/*======================================*/
	//! \enum OBJECT_LIST
	//! \brief 全オブジェクトリスト
	/*======================================*/
	enum OBJECT_LIST
	{
		
		// 初期値
		OBJ_NONE,					//!< エラーチェック用
		
		// プレイヤー
		OBJ_PLAYER,					//!< プレイヤー
		OBJ_PLAYER_BULLET,			//!< AGGの弾

		// オブジェクト
		OBJ_BOX,					//!< ボックス
		OBJ_CHAIN_BOX,				//!< 鎖に繋がれたボックス
		OBJ_FENCE,					//!< 侵入禁止のフェンス
		OBJ_BRIDGE,					//!< 橋
		OBJ_LIGHT,					//!< 蛍光灯
		OBJ_BREAK_WALL,				//!< 壊れる床
		
		// ギミック
		OBJ_SWITCH,					//!< 押すことでギミックを動作させるもの
		OBJ_WIREMESH,				//!< 金網
		OBJ_LIFT_X,					//!< 横に移動するリフト
		OBJ_LIFT_Y,					//!< 縦に移動するリフト
		OBJ_BALANCE,				//!< 天秤
		OBJ_STAIRS,					//!< スイッチで連動して動く階段
		OBJ_CONVEYER,				//!< ボックスを運ぶコンベア
		OBJ_SURVEILLANCE_CAMERA,	//!< 監視カメラ
		OBJ_CLUE_LEVER,				//!< ヒントレバー

		// 敵
		OBJ_FOOL,					//!< 直線にしか進めない敵
		OBJ_TACKLE,					//!< 突撃してくる敵

		// 部屋
		OBJ_TUTORIAL_ROOM,			//!< チュートリアルステージ
		OBJ_PASSAGEWAY,				//!< 通路
		OBJ_ROOM1,					//!< 部屋1
		OBJ_ROOM2,					//!< 部屋2
		OBJ_ROOM3,					//!< 部屋3
		OBJ_ROOM4,					//!< 部屋4

		OBJ_MAX						//!< オブジェクトの最大数

	};


	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CObjectManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CObjectManager();
		}
		return m_pInstance;
	}

	
	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CObjectManager( void );


	/*============================*/
	//! \brief 更新
	/*============================*/
	void Updata( void );


	/*============================*/
	//! \brief 描画
	/*============================*/
	void Draw( void );


	/*=======================================================================================*/
	//! \brief オブジェクトを読み込む
	//! \param[in]		_FilePass		読み込むデータを保存しているcsvファイルのパス
	//! \return 成功 = true
	//! \return 失敗 = false
	/*=======================================================================================*/
	bool LoadObject( const std::string _FilePass );


	/*========================================================*/
	//! \brief オブジェクトを追加をする
	//! \param[in]	_Name	追加するオブジェクトの名前
	//! \param[in]	_Pos	追加する際の初期座標
	/*========================================================*/
	CObjectBase* AddObject( const std::string _Name , const D3DXVECTOR3 _Pos );

	/*========================================================*/
	//! \brief オブジェクトを追加をする
	//! \param[in]	*_Object	追加するオブジェクト
	/*========================================================*/
	void AddObject( CObjectBase *_Object );


	/*=======================================================*/
	//! \brief 確保している全オブジェクトを解放する
	/*=======================================================*/
	void AllDeleteObject( void );


	/*================================================*/
	//! \brief プレイヤーのポインタを返す
	/*================================================*/
	CPlayer* GetCPlayerObject( void ){ return m_pPlayer; }



	/*==========================================================*/
	// 指定したIDのギミックを探し、動作しているのか返す
	//! \param[in]	_Connector	探したいギミックのID
	//! \return 動作中 = true　
	//! \return 動作していない = false
	/*==========================================================*/
	bool GetWorkingFlag( const int _Connector );


	/*============================================================*/
	//! \brief 指定したオブジェクトが浮遊中か調べる
	//! \param[in]	*_Actor		探すオブジェクトのアクター
	//! \return 浮遊中 = true
	//! \return 浮遊していない = false 
	/*============================================================*/
	bool GetAntiGravityFlag( const NxActor *_Actor );


	/*============================================================*/
	//! \brief アクターからオブジェクトを見つける
	//! \param[in]	*_Actor		探すオブジェクトのアクター
	//! \return オブジェクトのポインタ
	/*============================================================*/
	CObjectBase* CObjectManager::GetActorToObject( const NxActor *_Actor );


	/*=======================================================*/
	//! \brief オブジェクトの浮遊可能時間を0にする
	/*=======================================================*/
	void ResetActiGravity( void );


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CObjectManager( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CObjectManager( const CObjectManager& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CObjectManager& operator = ( const CObjectManager& single );


	/*=================================================================================================*/
	//! \brief ここで全てのオブジェクトを生成する
	//! \brief 外部ファイルから読み込む
	//! \param[in]	_ObjectData		オブジェクトを構成するデータ
	//! \param[in]	_FilePass		エラー時に出力で使用する不具合のあるファイルのファイル名
	//! \return 生成したオブジェクト
	/*=================================================================================================*/
	CObjectBase* MakeObject( const OBJECT_DATA _ObjectData , const std::string _FilePass );


	//! \brief インスタンスのポインタ
	static CObjectManager *m_pInstance;		//!< CObjectManagerのポインタ

	//! \brief 各部屋毎に解放、読み込みを行う
	//! \note ここに部屋の構成に必要なオブジェクトが全て格納される
	static std::vector< CObjectBase*  > m_ObjectList;			//!< 読み込み時に作成されたオブジェクトを格納
	static std::vector< CGimmickBase* > m_GimmickList;			//!< 読み込み時に作成されたギミックを格納
	static std::vector< CObjectBase*  > m_AddObjectList;		//!< ゲームプレイ中に追加されたオブジェクトを格納

	CDirect3D *m_pD3D;				//!< CDirect3Dのポインタ
	CPhysX *m_pPhysX;				//!< CPhysXのポインタ
	int m_ErrorCheckCounter;		//!< 複数の部屋データを読み込もうとしていないかのチェック

	//! \brief 特別にプレイヤーのみポインタを持っておく
	CPlayer* m_pPlayer;				//!< プレイヤーのポインタ

};

/*==========================================*/
//! \def OBJECT_MANAGER
//! \brief コール時に使用するマクロ
/*==========================================*/
#define OBJECT_MANAGER CObjectManager::GetInstance()