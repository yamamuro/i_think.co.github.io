#include "stdafx.hpp"

#include "../Object/Player/Player/CPlayer.hpp"
#include "CAttractionAndRepulsionLaser.hpp"


/*===============================================*/
//! \brief コンストラクタ
/*===============================================*/
CAttractionAndRepulsionLaser::CAttractionAndRepulsionLaser( CPlayer *_pPlayer )
{
	m_pPlayer = _pPlayer;

	Name = "AARL";
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CAttractionAndRepulsionLaser::~CAttractionAndRepulsionLaser( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CAttractionAndRepulsionLaser::Exec( void )
{
	// 引力の作動
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = true;
		LeftClickAction();
	}

	// 斥力の作動
	if( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = false;
		RightClickAction();
	}
}


/*==============================================*/
//! \brief 左クリック時のアクション
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

	// パーティクルの更新
	LASER_COLOR Black( 0.0f , 0.0f , 0.0f );
	ParticleUpdate( Black , AddParticleFlag );

}

/*==============================================*/
//! \brief 右クリック時のアクション
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

	// パーティクルの更新
	LASER_COLOR Black( 0.0f , 0.0f , 0.0f );
	ParticleUpdate( Black , AddParticleFlag );

}