#pragma once

/*===================================*/
//!< �O���錾
/*===================================*/
class CPlayer;
class CLaser;
class CParticleEmitter;

#include "../../../ObjectBase/CObjectBase.hpp"
#include "../LaserManager/CLaserManager.hpp"


class CLaserBase : public CObjectBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CLaserBase
	//! \brief <b>���[�U�[�e�̊��N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/26
	//! \since 2014/11/26
	//!
	/*=========================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CLaserBase( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CLaserBase( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	virtual void Exec( void ) = 0;


	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	//! \param[in]	_Color	���[�U�[�̐F
	/*=============================================*/
	void Draw( CDirect3D & _d3d , LASER_COLOR _Color );


	static const bool AddParticleFlag = false;		//!< �p�[�e�B�N����ǉ��A�폜���鎞�̃t���O
	static const bool ParticleUpdateFlag = true;	//!< �p�[�e�B�N���̍X�V���Ɏg�p����t���O
	/*===================================================*/
	//! \brief �p�[�e�B�N���̍X�V
	//! \param[in]	_Color		�p�[�e�B�N���̐F
	//! \param[in]	_Flag		�X�V�̎��
	//! \param[in]	_deltaTime	�X�V���鎞��
	/*===================================================*/
	void ParticleUpdate( LASER_COLOR _Color , bool _Flag , NxReal _DeltaTime = 1/(float)FRAME_RATE );

	/*============================================*/
	//! \brief ���[�U�[�����̍X�V
	/*============================================*/
	void LaserUpdate( void );


	/*================================*/
	//! ���C�g�̃Z�b�^
	/*================================*/
	void SetLightFlag( BOOL _Flag ){ m_pd3d->GetCLightManager()->UpdateLight( m_Light , m_LightIndex , _Flag ); }


protected:

	/*===========================================================*/
	//! \brief ���C�̓����蔻��
	//! \param[out]	*Distance	�I�u�W�F�N�g�܂ł̋���
	//! \return �q�b�g = true
	//! \return �q�b�g���Ȃ����� = false
	/*===========================================================*/
	bool CLaserBase::HitCheck( float *Distance );


	/*==============================================*/
	//! \brief ���N���b�N���̃A�N�V����
	/*==============================================*/
	virtual void LeftClickAction( void ) = 0;

	/*==============================================*/
	//! \brief �E�N���b�N���̃A�N�V����
	/*==============================================*/
	virtual void RightClickAction( void ) = 0;


	NxActor *m_pHitActor;		//!< ���[�U�[�̓��������A�N�^�[�̃|�C���^
	CObjectBase *m_pHitObject;	//!< ���[�U�[�̓��������I�u�W�F�N�g�̃|�C���^

	static CPlayer *m_pPlayer;	//!< �v���C���[�̃|�C���^

	CMouse *m_pMouse;			//!< CMouse�̃|�C���^

	CRaycastLine RayPos;		//!< ���C�̏����ʒu�ƏI�_���i�[

	CXFile *m_lpAGLEffect;		//!< AGL�{�̂̋N�����G�t�F�N�g

	D3DLIGHT9 m_Light;			//!< ���[�U�[�̓��������n�_���Ƃ炷���C�g
	int m_LightIndex;			//!< ���C�g�̃C���f�b�N�X�ԍ�


	static CParticleEmitter *m_pParticleEmitter;	//!< ���������ꏊ�ɔ�юU��p�[�e�B�N���G�t�F�N�g
	CLaser *m_pLaser;			//!< ���[�U�[�̃|�C���^

	bool m_LaserReverseFlag;	//!< ���[�U�[���t������A�j���[�V�����ɂ��邩�̃t���O
	
};