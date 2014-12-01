#pragma once

#include "../GimmickBase/CGimmickBase.hpp"


class CLiftX : public CGimmickBase
{

	/*==============================================================*/
	//!
	//! \file
	//! \class CLiftX
	//! \brief <b>X軸へ移動するリフト管理用クラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/10
	//!
	/*==============================================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CLiftX( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CLiftX( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


	/*=================================================*/
	//! \brief 進行方向にレイを飛ばし、距離を返す
	/*=================================================*/
	float MoveCheck( void );


private:

static const bool MoveLeft  = false;	//!< m_MoveFlagで使用	左方向に進ませる際に使用
static const bool MoveRight = true;		//!< m_MoveFlagで使用	右方向に進ませる際に使用

	bool m_MoveFlag;					//!< 現在どちらの方向に進んでいるかのフラグ

};
