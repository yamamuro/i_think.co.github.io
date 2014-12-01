#include "stdafx.hpp"

#include "Direct3D\CDirect3D.hpp"
#include "PhysX/CPhysX.hpp"
#include "../../Player/CPlayer.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Particle/CParticle.hpp"
#include "../Particle/CParticleEmitter.hpp"
#include  "XFile/CXFile.hpp"
#include "CAntiGravityLaser.hpp"


/*===============================================*/
//! \brief �R���X�g���N�^
/*===============================================*/
CAntiGravityLaser::CAntiGravityLaser( CPlayer *_pPlayer )
{
	m_pPlayer = _pPlayer;

	Name = "AGL";
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CAntiGravityLaser::~CAntiGravityLaser( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CAntiGravityLaser::Exec( void )
{

	// AGL�쓮
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = false;
		LeftClickAction();
	}
	// AGL�̃L�����Z��
	if( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = true;
		RightClickAction();
	}
}


/*==============================================*/
//! \brief ���N���b�N���̃A�N�V����
/*==============================================*/
void CAntiGravityLaser::LeftClickAction( void )
{
	float Dis;
	if( HitCheck( &Dis ) )
	{
		m_pHitObject = OBJECT_MANAGER->GetActorToObject( m_pHitActor );
		if( m_pHitObject != nullptr )
			m_pHitObject->SetLaserHitFlag( true );
	}
	// �p�[�e�B�N���̍X�V
	LASER_COLOR Blue( 0.0f , 0.0f , 1.0f );
	ParticleUpdate( Blue , AddParticleFlag );
}

/*==============================================*/
//! \brief �E�N���b�N���̃A�N�V����
/*==============================================*/
void CAntiGravityLaser::RightClickAction( void )
{
	// AntiGravity�̃L�����Z��
	float Dis;
	if( HitCheck( &Dis ) )
	{
		m_pHitObject = OBJECT_MANAGER->GetActorToObject( m_pHitActor );
		if( m_pHitObject != nullptr )
			m_pHitObject->ResetAntiGravity();
	}
	// �p�[�e�B�N���̍X�V
	LASER_COLOR Blue( 0.0f , 0.0f , 1.0f );
	ParticleUpdate( Blue , AddParticleFlag );
}