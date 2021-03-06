#pragma once 

#include "../LaserBase/CLaserBase.hpp"

class CHoldingLaser : public CLaserBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CHoldingLaser
	//! \brief <b>HLを管理するクラス</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/26
	//! \since 2014/11/26
	//!
	/*=========================================================*/

public:

	/*===============================================*/
	//! \brief コンストラクタ
	/*===============================================*/
	CHoldingLaser( CPlayer *_pPlayer );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CHoldingLaser( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


private:

	/*==============================================*/
	//! \brief 左クリック時のアクション
	/*==============================================*/
	void LeftClickAction( void );

	/*==============================================*/
	//! \brief 右クリック時のアクション
	/*==============================================*/
	void RightClickAction( void );


	bool m_HittingFlag;		//!< オブジェクトをホールドしているか

};