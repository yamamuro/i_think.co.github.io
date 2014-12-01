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
	//!	\brief <b>���Ɍq���ꂽ�{�b�N�X�̊Ǘ��N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/28
	//! \since 2014/11/28
	//!
	/*============================================================*/

public:
	
	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CChainBox( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CChainBox( void );


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

	CObjectBase *m_pBox;
	CObjectBase *m_pRope;

	NxRevoluteJoint *m_Joint;

};