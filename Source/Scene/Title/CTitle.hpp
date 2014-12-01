#pragma once 

#include "../Scene/SceneBase/CSceneBase.hpp"


class CTitle : public CSceneBase
{

	/*===============================================*/
	//!
	//! \file
	//! \class CTitle
	//! \brief <b>タイトル画面を管理</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/10
	//!
	/*================================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CTitle( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CTitle( void );


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
	std::string GetName( void ){ return "Title"; }


private:

	
	CTexture *m_lpTitleTexture;			//!< ロゴマーク
	CTexture *m_lpPPushSpaceTexture;	//!< Push Spaceの描かれた文字テクスチャ

	CSprite* m_pSprite;					//!< スプライト

};