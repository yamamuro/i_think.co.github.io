#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"


class CFluorescentLight : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CFluorescentLight
	//!	\brief <b>蛍光灯</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/20
	//! \since 2014/10/29
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CFluorescentLight( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CFluorescentLight( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );

	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	/*=============================================*/
	void Draw( CDirect3D & _Direct3D );


private:

	/*================================================================*/
	//! \struct CUSTOMVERTEX
	//! \brief <b>板ポリゴンの頂点情報を管理</b>
	/*================================================================*/
	typedef struct CUSTOMVERTEX
	{
		D3DXVECTOR3 Pos;		//!< 頂点座標
		D3DXVECTOR2 Tex;		//!< テクスチャ座標
	}CUSTOMVERTEX;
	static const int CUSTOM_FVF = ( D3DFVF_XYZ | D3DFVF_TEX1 );		//!< 頂点データの構成


	D3DLIGHT9 m_Light;		//!< ライト構造体

	int m_LightIndex;		//!< ライトのインデックス番号

	CTexture *m_Texture[2];	//!< ライトのテクスチャ
	

};