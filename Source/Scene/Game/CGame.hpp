#pragma once

#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"


class CGame : public CSceneBase
{

	/*============================================================*/
	//!
	//! \file
	//! \class CGame
	//! \brief <b>ゲームシーンクラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/18
	//!
	/*============================================================*/

public:

	/*=================================*/
	//! \enum ROOM_NAME
	//! \brief 部屋識別用
	/*=================================*/
	enum ROOM_NAME
	{
		TUTORIAL_ROOM,		//!< チュートリアル部屋
		PASSAGEWAY,			//!< 全ての部屋に繋がる通路
		ROOM1,				//!< 部屋1
		ROOM2,				//!< 部屋2
		ROOM3,				//!< 部屋3
		ROOM4,				//!< 部屋4
		ROOM_MAX			//!< 部屋の最大数
	};


	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CGame( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CGame( void );


	/*===========================================*/
	//! \brief 更新
	//! \return シーンの変更 = true
	//! \return 継続 　　　　= false
	/*===========================================*/
	bool Updata( void );


	/*=================================*/
	//! \brief 画面表示
	/*=================================*/
	void Draw( void );


	/*=======================================*/
	//! \brief シーンの名前を返す
	/*=======================================*/
	std::string GetName( void ){ return "Game"; }


	/*======================================*/
	//! \brief 部屋のIDを返す
	/*======================================*/
	inline int GetRoomID( void ){ return m_RoomID; }


	/*======================================*/
	//! \brief 部屋のパスを返す
	/*======================================*/
	inline std::string GetRoomDataPass( void ){ return m_RoomDataPass; }


private:

	/*======================================*/
	//! \brief 部屋のIDをセット
	/*======================================*/
	inline void SetRoomID( const int _ID ){ m_RoomID = _ID; }		


	/*======================================*/
	//! \brief 部屋のパスをセット
	/*======================================*/
	inline void SetRoomPass( const  std::string _RoomDataPass ){ m_RoomDataPass = _RoomDataPass; }


	/*=================================*/
	//! \brief 部屋を切り替える
	/*=================================*/
	void ChangeRoom( const int _ID );

	
	std::string m_RoomsDataPass[ROOM_MAX];	//!< 部屋のパス
	std::string m_Name[ROOM_MAX];			//!< デバッグ表示に使用する部屋名

	std::string m_RoomDataPass;				//!< アクティブなルームデータへのパス
	int m_RoomID;							//!< アクティブな部屋識別ID
	
	CSound* m_pBGM;							//!< 部屋のBGM

};
