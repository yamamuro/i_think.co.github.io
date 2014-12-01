#pragma once

#include "../GimmickBase/CGimmickBase.hpp"


class CLiftY : public CGimmickBase
{

	/*===========================================================*/
	//!
	//! \file
	//! \class CLiftY
	//! \brief <b>Y���ֈړ����郊�t�g�Ǘ��p�N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/10
	//!
	/*===========================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CLiftY( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CLiftY( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	/*=================================================*/
	//! \brief �i�s�����Ƀ��C���΂��A������Ԃ�
	/*=================================================*/
	float MoveCheck( void );


private:

	static const bool MoveUp   = false;		//!< m_MoveFlag�Ŏg�p	������ɐi�܂���ۂɎg�p
	static const bool MoveDown = true;		//!< m_MoveFlag�Ŏg�p	�������ɐi�܂���ۂɎg�p

	bool m_MoveFlag;						//!< ���݂ǂ���̕����ɐi��ł��邩�̃t���O

	int FreezeCount;						//!< �e�[�܂Ői�񂾎��̈ꎞ��~�̃J�E���^
	static const int FreezeTime = 200;		//!< ��~������t���[����

	NxActor *m_Stopper;						//!< �������~�����邽�߂̃X�g�b�p�[

};
