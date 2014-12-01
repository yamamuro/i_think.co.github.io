#pragma once 

class CDirect3D;
class CLaserBase;
class CAntiGravityLaser;
class CHoldingLaser;


class CLaserManager
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CLaserManager
	//! \brief <b>レーザー銃を管理するクラス</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/26
	//! \since 2014/11/26
	//!
	/*=========================================================*/

public:

	/*===================================*/
	//! \enum LASER_GUN_TYPE
	//! \brief レーザー銃の種類
	/*===================================*/
	enum LASER_GUN_TYPE
	{
		ANTI_GRAVITY,					//!< 重力を浮力に変換
		HOLDING,						//!< 銃との相対位置で固定
		ATTRACTION_AND_REPULSION,		//!< 引力と斥力で物体を動かす
		MAX_LASER_GUN					//!< レーザー銃の種類数
	};


	/*===============================================*/
	//! \brief コンストラクタ
	/*===============================================*/
	CLaserManager( CPlayer *_pPlayer );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CLaserManager( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	/*=============================================*/
	void Draw( CDirect3D & _d3d );

private:

	/*===========================================*/
	//! \brief レーザーの切り替え処理
	/*===========================================*/
	void ChangeLaserType( void );

	/*=================================================*/
	//! \brief レーザーの色を変更する
	/*=================================================*/
	void ChangeingLaserColor( void );


	CMouse *m_pMouse;							//!< マウスのポインタ

	int m_UsingLaserType;						//!< 現在使用しているレーザーのタイプ
	CLaserBase *m_pUsingLaser;					//!< 現在使用しているレーザーのポインタ

	CLaserBase *m_pLasers[MAX_LASER_GUN];		//!< 各レーザーのポインタを格納		

	LASER_COLOR m_Color;								//!< 現在使用しているレーザーの色
	LASER_COLOR m_LaserGunColor[MAX_LASER_GUN];		//!< 各レーザー銃を表す色データ 


};