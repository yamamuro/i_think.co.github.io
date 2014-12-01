#pragma once 

#include "../GimmickBase/CGimmickBase.hpp"


class CStairs : public CGimmickBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CStairs
	//! \brief <b>機械仕掛けの階段の管理クラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/16
	//! \since 2014/09/23
	//!
	/*=========================================================*/

public: 

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CStairs( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CStairs( void );


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

	static const int StairsNum = 8;		//!< 段の数

	NxVec3 m_Pos[StairsNum];			//!< 各段の座標
	
	NxActor *m_Stairs[StairsNum];		//!< 各段のアクター
	

	D3DMATERIAL9 *m_StairsMaterial;		//!< マテリアル情報

	CXFile *m_lpProp;					//!< 階段の支えの部分
	D3DMATERIAL9 *m_PropMaterial;		//!< マテリアル情報

};