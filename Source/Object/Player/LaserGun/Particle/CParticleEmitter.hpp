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

const int ciMaxParticles = 128;					// �ő唭����
const NxReal cfTimeBetweenParticles = 0.04f;	// �����Ԋu

const NxCollisionGroup cParticleCollisionGroup = CPhysX::CG_PARTICLE;	// �R���W�����O���[�v

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

		// \note �f�[�^�͏z�L���[�ɂ���ĊǗ�����Ă���
		CParticle* _aParticleArray[ciMaxParticles];		//!< �p�[�e�B�N�����i�[

		int _iHeadIndex;		//!< ���݂̐擪�C���f�b�N�X
		int _iTailIndex;		//!< ���݂̖����C���f�b�N�X
		int _iParticleCount;	//!< �����̃p�[�e�B�N�������݂��邩
		NxReal _fStartingVelScale;	//!< �U��΂�
		NxReal _fNewParticleTimer;
		NxVec3 _vThermoDynamicAcceleration;
		CDirect3D *m_pd3d;	
		CTexture *m_lpTexture;
};

#endif
