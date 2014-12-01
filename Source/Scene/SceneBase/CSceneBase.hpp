#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Application/CApplication.hpp"


class CSceneBase
{

	/*================================================*/
	//!
	//! \file
	//! \class CSceneBase
	//! \brief <b>各シーンの基底クラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/10
	//! \since 2014/09/12
	//!
	/*================================================*/

public:


	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CSceneBase( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	virtual ~CSceneBase( void );


	/*=================================*/
	//! \brief 更新
	//! \return シーンの変更 = true
	//! \return 継続 　　　　= false
	/*=================================*/
	virtual bool Updata( void ) = 0;


	/*=================================*/
	//! \brief 画面表示
	/*=================================*/
	virtual void Draw( void ) = 0;


	/*=======================================*/
	//! \brief シーンの名前を返す
	/*=======================================*/
	virtual std::string GetName( void ) = 0;


protected:

	std::string m_FilePass;
	CDirect3D *m_pd3d;					//!< CDirect3Dのポインタ
	CPhysX *m_pPhysX;					//!< CPhysXのポインタ

	CKeyboard *m_pKeyboard;				//!< CKeyboardのポインタ
	CDebug *m_pDebug;					//!< FPSを表示するためにインスタンスを持っておく
	
	CObjectManager *m_pObjectManager;	//!< オブジェクトマネージャ
	CSceneManager *m_pSceneManager;		//!< シーンマネージャ

};