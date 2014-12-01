#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"
class CLaserManager;


/*==============================================*/
//!
//! \file
//! \class CPlayer
//! \brief <b>プレーヤーの管理</b>
//!
//! \author	Y.Yamamuro \n
//! \date  2014/09/03
//! \since 2041/09/12
//!
/*==============================================*/

class CPlayer : public CObjectBase
{
public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CPlayer( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CPlayer( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


	/*=======================================*/
	//! \brief マウスに関連した処理
	/*=======================================*/
	void MouseExec( void );


	/*=================================*/
	//! \brief 移動
	/*=================================*/
	void Move( void );

	
	/*============================================*/
	//! \brief 足場判定
	//! \return 接地中 = true
	//! \return 空中にいる = false
	/*============================================*/
	bool CPlayer::CheckFooting( void );


	/*=============================================*/
	//! \brief レーザー銃のポインタを返す
	/*=============================================*/
	inline CLaserManager* GetLaserGun( void ){ return m_pLaserGun; }


private:

	CDirect3D* m_pd3d;				//!< Direct3Dのインスタンスを格納
	CKeyboard* m_pKeyboard;			//!< CKeyboardのインスタンスを格納
	CMouse* m_pMouse;				//!< CMouseのインスタンスを格納
	POINT m_WindowCenterPos;		//!< ウィンドウの中心座標

	const float m_MoveVec;			//!< 移動量


	float m_TotalHeight;			//!< カプセルの半径+高さ+半径

	//CSound* m_pBGM;				//!< 主人公の歩く音

	CLaserManager *m_pLaserGun;		//!< レーザー銃のポインタ


};

