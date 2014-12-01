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
//! \brief コンストラクタ
/*===============================================*/
CAntiGravityLaser::CAntiGravityLaser( CPlayer *_pPlayer )
{
	m_pPlayer = _pPlayer;

	Name = "AGL";
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CAntiGravityLaser::~CAntiGravityLaser( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CAntiGravityLaser::Exec( void )
{

	// AGL作動
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = false;
		LeftClickAction();
	}
	// AGLのキャンセル
	if( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_HOLD )
	{
		m_LaserReverseFlag = true;
		RightClickAction();
	}
}


/*==============================================*/
//! \brief 左クリック時のアクション
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
	// パーティクルの更新
	LASER_COLOR Blue( 0.0f , 0.0f , 1.0f );
	ParticleUpdate( Blue , AddParticleFlag );
}

/*==============================================*/
//! \brief 右クリック時のアクション
/*==============================================*/
void CAntiGravityLaser::RightClickAction( void )
{
	// AntiGravityのキャンセル
	float Dis;
	if( HitCheck( &Dis ) )
	{
		m_pHitObject = OBJECT_MANAGER->GetActorToObject( m_pHitActor );
		if( m_pHitObject != nullptr )
			m_pHitObject->ResetAntiGravity();
	}
	// パーティクルの更新
	LASER_COLOR Blue( 0.0f , 0.0f , 1.0f );
	ParticleUpdate( Blue , AddParticleFlag );
}