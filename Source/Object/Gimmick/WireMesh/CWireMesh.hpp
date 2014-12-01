#pragma once

#include "../GimmickBase/CGimmickBase.hpp"

/*===================================*/
//!< �O���錾
/*===================================*/
class CPlayer;


class CWireMesh : public CGimmickBase
{

	/*===========================================================*/
	//!
	//! \file
	//! \class CWireMesh
	//! \brief <b>���ԊǗ��p�N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/02
	//! \since 2014/10/07
	//! \since 2014/11/21 �M�~�b�N�ɕύX
	//!
	/*===========================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CWireMesh( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );

	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CWireMesh( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	void Draw( CDirect3D & _d3d );


private:

	static const int WireMeshNum = 10;		//!< �ݒu������Ԃ̐�

	//!< ���W���Q�Ƃ��邽�߂Ɏg�p
	CPlayer* m_pPlayer;						//!< �v���C���[�̃I�u�W�F�N�g

	D3DXVECTOR3 m_Pos[WireMeshNum];			//!< �e���Ԃ̍��W

	float m_Scale;							//!< ���b�V���̊g�k��

	CXFile *m_lpGap;						//!< ���Ԃ̌���

	NxActor *m_Actors[WireMeshNum];			//!< ���Ԃ̃A�N�^�[

};
