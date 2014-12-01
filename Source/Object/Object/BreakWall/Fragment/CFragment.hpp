#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"


class CFragment : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CFragment
	//!	\brief <b>壊れる床の破片</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/22
	//! \since 2014/10/22
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief コンストラクタ
	/*===============================================*/
	CFragment( const int _Type , D3DXVECTOR3 _Pos );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CFragment( void );


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

	int m_Type;
	float m_Scale;


	static int m_MaterialIndex;

};