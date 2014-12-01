#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"


class CFragment : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CFragment
	//!	\brief <b>���鏰�̔j��</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/22
	//! \since 2014/10/22
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief �R���X�g���N�^
	/*===============================================*/
	CFragment( const int _Type , D3DXVECTOR3 _Pos );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CFragment( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	void Draw( CDirect3D &_d3d );


private:

	int m_Type;
	float m_Scale;


	static int m_MaterialIndex;

};