#pragma once

#include "../Scene/Game/CGame.hpp"
#include "../Object/ObjectBase/CObjectBase.hpp"


class CRoomManager : public CObjectBase
{

	/*=========================================================================================*/
	//! 
	//! \file
	//! \class CRoomManager
	//! \brief <b>部屋の管理クラス</b>
	//! 
	//! \author Y.Yamamuro \n
	//! \date  2014/09/28
	//! \since 2014/10/02
	//! \since 2014/11/21 部屋の作成方法をコンベックスメッシュを使用した作成方法に変更
	//! 
	/*=========================================================================================*/

public:

	
	/*=====================================================*/
	//! \brief コンストラクタ
	//! \param[in]	_Name	読み込む部屋の名前
	/*=====================================================*/
	CRoomManager( const int _Name );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CRoomManager( void );


protected:

	std::string m_RoomXFilePass[CGame::ROOM_MAX];	//!< 部屋のメッシュが格納されている場所へのパス

	D3DLIGHT9 m_Light;								//!< 部屋のディレクショナルライト

};