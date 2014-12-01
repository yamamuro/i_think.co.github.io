#pragma once 

#include "../GimmickBase/CGimmickBase.hpp"


class CStairs : public CGimmickBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CStairs
	//! \brief <b>�@�B�d�|���̊K�i�̊Ǘ��N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/16
	//! \since 2014/09/23
	//!
	/*=========================================================*/

public: 

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CStairs( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CStairs( void );


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

	static const int StairsNum = 8;		//!< �i�̐�

	NxVec3 m_Pos[StairsNum];			//!< �e�i�̍��W
	
	NxActor *m_Stairs[StairsNum];		//!< �e�i�̃A�N�^�[
	

	D3DMATERIAL9 *m_StairsMaterial;		//!< �}�e���A�����

	CXFile *m_lpProp;					//!< �K�i�̎x���̕���
	D3DMATERIAL9 *m_PropMaterial;		//!< �}�e���A�����

};