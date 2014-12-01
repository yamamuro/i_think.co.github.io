#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"


class CRope : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CRope
	//!	\brief <b>���[�v�̊Ǘ��N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/29
	//! \since 2014/10/29
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CRope( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CRope( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	void Draw( CDirect3D &_d3d );


	/*==============================================*/
	//! \brief ���[�v�̃A�N�^�[��Ԃ�
	/*==============================================*/
	inline NxActor** GetRopeSegs( void ){ return m_RopeSegs; }


	static const int PartsNum = 23;


private:


	NxActor **m_RopeSegs;				//!< ���[�v�̃J�v�Z���A�N�^�[
	NxSphericalJoint **m_RopeLinks;		//!< ���[�v�̊֐�

	static int m_MaterialIndex;			//!< �}�e���A���̃C���f�b�N�X�ԍ�

};