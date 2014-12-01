// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//					            PARTICLE SYSTEM CLASS
//
//						  Written by Sam Thompson, 3-17-05
//						    Edited by Bob Schade, 5-1-06
// ===============================================================================

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "NxActor.h"
#include "NxSphereShape.h"
#include "NxSphereShapeDesc.h"
#include "NxScene.h"
#include <stdio.h>
#include "../Object/ObjectBase/CObjectBase.hpp"

class CParticle;

const int ciMaxParticles = 128;					// 最大発生個数
const NxReal cfTimeBetweenParticles = 0.04f;	// 発生間隔

const NxCollisionGroup cParticleCollisionGroup = CPhysX::CG_PARTICLE;	// コリジョングループ

class CParticleEmitter 
{

	public:
		CParticleEmitter(NxScene* pSceneToSet, NxReal fStartingVelScale, NxVec3 vThermoDynamicAcceleration);
		~CParticleEmitter();
		void removeAllParticles();
		void update(NxReal fDeltaTime , NxVec3 Pos , LASER_COLOR _Color , bool Flag);
		void addParticle( NxVec3 Pos , LASER_COLOR _Color );
		void removeParticle();
		void DrawParticle( void );
	private:
		NxScene* _pScene;

		// \note データは循環キューによって管理されている
		CParticle* _aParticleArray[ciMaxParticles];		//!< パーティクルを格納

		int _iHeadIndex;		//!< 現在の先頭インデックス
		int _iTailIndex;		//!< 現在の末尾インデックス
		int _iParticleCount;	//!< いくつのパーティクルが存在するか
		NxReal _fStartingVelScale;	//!< 散らばり具合
		NxReal _fNewParticleTimer;
		NxVec3 _vThermoDynamicAcceleration;
		CDirect3D *m_pd3d;	
		CTexture *m_lpTexture;
};

#endif
