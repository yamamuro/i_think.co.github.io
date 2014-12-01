// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//					            PARTICLE SYSTEM CLASS
//
//						  Written by Sam Thompson, 3-17-05
//						    Edited by Bob Schade, 5-1-06
// ===============================================================================

#ifndef PARTICLE_H
#define PARTICLE_H

#include "NxActor.h"
#include "NxSphereShape.h"
#include "NxSphereShapeDesc.h"
#include "NxScene.h"
#include <stdio.h>
#include "../Object/ObjectBase/CObjectBase.hpp"


class CParticleEmitter;

// Make a simple particle class
class CParticle
{
	public:
		CParticle( void );
		CParticle( NxActor* pToSet , NxVec3& vThermoDynamicAcceleration , LASER_COLOR _Color );
		void update( NxReal fDeltaTime );
		void setActor( NxActor* pToSet );
		inline NxActor* GetActor( void ){ return _pActor; }
		NxActor& getActorReference( void );
		inline short GetAlpha( void ){ return Alpha; }
		inline LASER_COLOR GetColor( void ){ return m_Color; }

		static inline void SetParticleEmitter( CParticleEmitter *_pParticleEmitter ){ m_pParticleEmitter = _pParticleEmitter; }

		inline float GetScale( void ){ return m_Scale; }
	private:
		NxActor* _pActor;
		NxVec3 _vThermoDynamicForce;
		short Alpha;	//!< エフェクトの透明度
		float m_Scale;	//!< 板ポリゴンの拡大率
		LASER_COLOR m_Color;	//!< エフェクトの色
		static CParticleEmitter *m_pParticleEmitter;	
};

#endif
