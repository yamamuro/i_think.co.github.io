#pragma once 

#include "../LaserBase/CLaserBase.hpp"

class CAttractionAndRepulsionLaser : public CLaserBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CAttractionAndRepulsionLaser
	//! \brief <b>AARL���Ǘ�����N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/27
	//! \since 2014/11/27
	//!
	/*=========================================================*/

public:

	/*===============================================*/
	//! \brief �R���X�g���N�^
	/*===============================================*/
	CAttractionAndRepulsionLaser( CPlayer *_pPlayer );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CAttractionAndRepulsionLaser( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


private:

	/*==============================================*/
	//! \brief ���N���b�N���̃A�N�V����
	/*==============================================*/
	void LeftClickAction( void );

	/*==============================================*/
	//! \brief �E�N���b�N���̃A�N�V����
	/*==============================================*/
	void RightClickAction( void );


};