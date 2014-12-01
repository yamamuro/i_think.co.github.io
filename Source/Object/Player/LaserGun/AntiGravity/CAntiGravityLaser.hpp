#pragma once

/*===================================*/
//!< �O���錾
/*===================================*/
class CPlayer;
class CParticleEmitter;

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../LaserBase/CLaserBase.hpp"


class CAntiGravityLaser : public CLaserBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CAntiGravityLaser
	//! \brief <b>AGL���Ǘ�����N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/16
	//! \since 2014/09/27
	//!
	/*=========================================================*/

public:

	/*===============================================*/
	//! \brief �R���X�g���N�^
	/*===============================================*/
	CAntiGravityLaser( CPlayer *_pPlayer );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CAntiGravityLaser( void );


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