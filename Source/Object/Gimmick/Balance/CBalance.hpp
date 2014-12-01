#pragma once 

#include "../GimmickBase/CGimmickBase.hpp"


class CBalance : public CGimmickBase
{

	/*========================================================================*/
	//!
	//! \file
	//! \class CBalance
	//! \brief <b>�d���ɂ���č����̕ϓ�����V���̂悤�ȏ��̊Ǘ��N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/10
	//! \since 
	//!
	/*========================================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CBalance( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CBalance( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


private:

};
