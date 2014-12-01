// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//					            PARTICLE SYSTEM CLASS
//
//						  Written by Sam Thompson, 3-17-05
//						    Edited by Bob Schade, 5-1-06
// ===============================================================================

#include "stdafx.hpp" 
#include "../Object/ObjectBase/CObjectBase.hpp"
#include "CParticleEmitter.hpp"
#include "CParticle.hpp"


CParticleEmitter* CParticle::m_pParticleEmitter = nullptr;

CParticle::CParticle( void )
	: Alpha( 254 )
	, m_Scale( 1.0f )
{
	setActor(NULL);
	_vThermoDynamicForce.zero();

	CRandomize Rand;
	m_Scale = (float)Rand.GetRandomize( 1.0f , 1.5 );
}

CParticle::CParticle( NxActor* pToSet , NxVec3& vThermoDynamicAcceleration , LASER_COLOR _Color )
	: Alpha( 254 )
	, m_Scale( 1.0f )
{
	setActor(pToSet);

	// F = M*a
	_vThermoDynamicForce = pToSet->getMass()*vThermoDynamicAcceleration;

	CRandomize Rand;
	m_Scale = (float)Rand.GetRandomize( 1.0f , 1.5 );

	m_Color = _Color;
}

void CParticle::setActor( NxActor* pToSet )
{
	_pActor = pToSet;
}

void CParticle::update( NxReal fDeltaTime )
{
	
	Alpha -= 4;

	// アクターを削除する
	if( Alpha < 0 )
	{
		Alpha = 0; 
		m_pParticleEmitter->removeParticle();
		return;
	}

	// Use our down-caster to get a sphere pointer (and make sure it is a sphere)
	NxSphereShape* pSphere = _pActor->getShapes()[0]->isSphere();
	const NxReal fMaxRadius = 0.01f;
	const NxReal fGrowthRate = 0.6f;
	if( pSphere )
	{
		// Grow the radius at growth rate fGrowthRate m/s, until it reaches fMaxRadius meters
		NxReal fCurrentRadius = pSphere->getRadius();
		if ( fCurrentRadius < fMaxRadius )
		{
			pSphere->setRadius( pSphere->getRadius() + fGrowthRate*fDeltaTime );
			if( pSphere->getRadius() > fMaxRadius )
				pSphere->setRadius( fMaxRadius );
		}
	}
}

NxActor& CParticle::getActorReference( void )
{
	return *_pActor;
}
