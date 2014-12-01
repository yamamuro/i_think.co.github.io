#pragma once 

#include "../LaserBase/CLaserBase.hpp"

class CAttractionAndRepulsionLaser : public CLaserBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CAttractionAndRepulsionLaser
	//! \brief <b>AARLを管理するクラス</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/27
	//! \since 2014/11/27
	//!
	/*=========================================================*/

public:

	/*===============================================*/
	//! \brief コンストラクタ
	/*===============================================*/
	CAttractionAndRepulsionLaser( CPlayer *_pPlayer );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CAttractionAndRepulsionLaser( void );


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


};