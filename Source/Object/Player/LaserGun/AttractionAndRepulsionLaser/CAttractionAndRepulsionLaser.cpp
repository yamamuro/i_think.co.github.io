#include "stdafx.hpp"

#include "../Object/Player/Player/CPlayer.hpp"
#include "CAttractionAndRepulsionLaser.hpp"


/*===============================================*/
//! \brief �R���X�g���N�^
/*===============================================*/
CAttractionAndRepulsionLaser::CAttractionAndRepulsionLaser( CPlayer *_pPlayer )
{
	m_pPlayer = _pPlayer;

	Name = "AARL";
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CAttractionAndRepulsionLaser::~CAttractionAndRepulsionLaser( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CAttractionAndRepulsionLaser::Exec( void )
{
	// ���͂̍쓮
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = true;
		LeftClickAction();
	}

	// �˗͂̍쓮
	if( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = false;
		RightClickAction();
	}
}


/*==============================================*/
//! \brief ���N���b�N���̃A�N�V����
/*==============================================*/
void CAttractionAndRepulsionLaser::LeftClickAction( void )
{

	float Dis = 0.0f;
	if( HitCheck( &Dis ) && Dis > 20.0f )
	{

		m_pHitObject = OBJECT_MANAGER->GetActorToObject( m_pHitActor );
		if( m_pHitObject != nullptr && m_pHitObject->GetName() == "Box" )
		{

			D3DXVECTOR3 toPlayerVec = m_pPlayer->GetPos() - m_pHitObject->GetPos();
			toPlayerVec.y = 0.0f;
			D3DXVec3Normalize( &toPlayerVec , &toPlayerVec );
			m_pHitActor->setLinearVelocity( NxVec3( toPlayerVec ) * 40.0f );
		}

	}

	// �p�[�e�B�N���̍X�V
	LASER_COLOR Black( 0.0f , 0.0f , 0.0f );
	ParticleUpdate( Black , AddParticleFlag );

}

/*==============================================*/
//! \brief �E�N���b�N���̃A�N�V����
/*==============================================*/
void CAttractionAndRepulsionLaser::RightClickAction( void )
{
	
	float Dis = 0.0f;
	if( HitCheck( &Dis ) )
	{

		m_pHitObject = OBJECT_MANAGER->GetActorToObject( m_pHitActor );
		if( m_pHitObject != nullptr && m_pHitObject->GetName() == "Box" )
		{

			D3DXVECTOR3 toPlayerVec = m_pPlayer->GetPos() - m_pHitObject->GetPos();
			toPlayerVec.y = 0.0f;
			D3DXVec3Normalize( &toPlayerVec , &toPlayerVec );
			m_pHitActor->setLinearVelocity( NxVec3( -toPlayerVec ) * 40.0f );
		}

	}

	// �p�[�e�B�N���̍X�V
	LASER_COLOR Black( 0.0f , 0.0f , 0.0f );
	ParticleUpdate( Black , AddParticleFlag );

}