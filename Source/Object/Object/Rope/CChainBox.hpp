#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"

class CBox;
class CRope;

class CChainBox : public CObjectBase
{

	/*===========================================================*/
	//!	
	//! \file
	//! \class CChainBox
	//!	\brief <b>鎖に繋がれたボックスの管理クラス</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/28
	//! \since 2014/11/28
	//!
	/*============================================================*/

public:
	
	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CChainBox( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CChainBox( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	/*=============================================*/
	void Draw( CDirect3D &_d3d );


private:

	CObjectBase *m_pBox;
	CObjectBase *m_pRope;

	NxRevoluteJoint *m_Joint;

};