#pragma once 

class CDirect3D;
class CLaserBase;
class CAntiGravityLaser;
class CHoldingLaser;


class CLaserManager
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CLaserManager
	//! \brief <b>���[�U�[�e���Ǘ�����N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/26
	//! \since 2014/11/26
	//!
	/*=========================================================*/

public:

	/*===================================*/
	//! \enum LASER_GUN_TYPE
	//! \brief ���[�U�[�e�̎��
	/*===================================*/
	enum LASER_GUN_TYPE
	{
		ANTI_GRAVITY,					//!< �d�͂𕂗͂ɕϊ�
		HOLDING,						//!< �e�Ƃ̑��Έʒu�ŌŒ�
		ATTRACTION_AND_REPULSION,		//!< ���͂Ɛ˗͂ŕ��̂𓮂���
		MAX_LASER_GUN					//!< ���[�U�[�e�̎�ސ�
	};


	/*===============================================*/
	//! \brief �R���X�g���N�^
	/*===============================================*/
	CLaserManager( CPlayer *_pPlayer );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CLaserManager( void );


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

	/*===========================================*/
	//! \brief ���[�U�[�̐؂�ւ�����
	/*===========================================*/
	void ChangeLaserType( void );

	/*=================================================*/
	//! \brief ���[�U�[�̐F��ύX����
	/*=================================================*/
	void ChangeingLaserColor( void );


	CMouse *m_pMouse;							//!< �}�E�X�̃|�C���^

	int m_UsingLaserType;						//!< ���ݎg�p���Ă��郌�[�U�[�̃^�C�v
	CLaserBase *m_pUsingLaser;					//!< ���ݎg�p���Ă��郌�[�U�[�̃|�C���^

	CLaserBase *m_pLasers[MAX_LASER_GUN];		//!< �e���[�U�[�̃|�C���^���i�[		

	LASER_COLOR m_Color;								//!< ���ݎg�p���Ă��郌�[�U�[�̐F
	LASER_COLOR m_LaserGunColor[MAX_LASER_GUN];		//!< �e���[�U�[�e��\���F�f�[�^ 


};