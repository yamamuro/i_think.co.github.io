#pragma once

#include "../Object/ObjectManager/CObjectManager.hpp"
#include "Direct3D/CDirect3D.hpp"
#include "PhysX/CPhysX.hpp"
#include "XFile/CXFile.hpp"

class CObjectBase
{

	/*===================================================*/
	//!
	//! \file
	//! \class CObjectBase
	//! \brief <b>�I�u�W�F�N�g�̊��N���X</b>
	//!
	//! \author �z�z�f�[�^+Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/10
	//!
	/*===================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CObjectBase( void );

	/*==========================================================================================*/
	//! \brief ������
	//! \param[in]	_Data		�I�u�W�F�N�g�̏������Ɏg�p����f�[�^
	/*==========================================================================================*/
	virtual void Init( CObjectManager::OBJECT_DATA _Data );
	
	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	virtual void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	virtual ~CObjectBase( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	virtual void Exec( void );


	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	virtual void Draw( CDirect3D &_d3d );


	/*==================================*/
	//! \brief �s���Ԃ�
	/*==================================*/
	CMatrix GetMatrix( void ); 


	/*==================================*/
	//! \brief ���W��Ԃ�
	/*==================================*/
	inline D3DXVECTOR3 GetPos( void ){ return m_vPos; }

	/*============================================*/
	//! \brief ���W���Z�b�g����
	//! \param[in]	_v		�Z�b�g������W
	/*============================================*/
	inline void SetPos( const D3DXVECTOR3 _v ){ m_vPos = _v; }


	/*=================================================*/
	//! \brief �A�N�^�[�̍��W���X�V����
	//! \param[in]	_v		�Z�b�g������W
	/*=================================================*/
	inline void SetActorPos( const NxVec3 _v ){ m_Actor->setGlobalPosition( _v ); }


	/*==================================*/
	//! \brief ���݂̊p�x��Ԃ�
	/*==================================*/
	inline D3DXVECTOR3 GetRot( void ){ return m_vRot; }


	/*============================================*/
	//! \brief �p�x���Z�b�g����
	//! \param[in]	_v		�Z�b�g����p�x
	/*============================================*/
	inline void SetRot( const D3DXVECTOR3 _v ){ m_vRot = _v; }

	/*============================================*/
	//! \brief �p�x��ǉ�����
	//! \param[in]	_v		�ǉ�����p�x
	/*============================================*/
	inline void AddRot( const D3DXVECTOR3 _v ){ m_vRot += _v; }


	/*=============================================================*/
	//! \brief �A�N�^�[���Z�b�g����
	//! \param[in]	*_Actor		�Z�b�g����A�N�^�[�̃|�C���^
	/*===========================================================s==*/
	inline void SetActor( NxActor* _Actor ){ m_Actor = _Actor; }

	/*===================================*/
	//! \brief �A�N�^�[��Ԃ�
	/*===================================*/
	inline NxActor* GetActor( void ){ return m_Actor; }


	/*========================================*/
	//! \brief �A�N�^�[�f�[�^��Ԃ�
	/*========================================*/
	ACTOR_DATA* GetActorData( void ){ return &m_ActorData; }


	/*=============================*/
	//! \brief XFile��Ԃ�
	/*=============================*/
	inline CXFile* GetXFile( void ){ return m_lpXFile; }


	/*===========================================*/
	//! \brief �I�u�W�F�N�g����Ԃ�
	/*===========================================*/
	inline std::string GetName( void ){ return Name; }


	/*==========================================*/
	//! \brief �`�����񂵂ɂ��邩�Ԃ�
	/*==========================================*/
	inline bool GetDefermentFlag( void ){ return m_DrawFlag; }


	/*==================================================*/
	//! \brief ���[�U�[�ɓ������Ă��邩���Z�b�g
	/*==================================================*/
	inline void SetLaserHitFlag( const bool _Flag ){ m_LaserHitFlag = _Flag; }


	/*================================================*/
	//! \brief ���V�����ǂ����Ԃ�
	//! \return ���V�� = true
	//! \retrun ���V���Ă��Ȃ� = false
	/*================================================*/
	inline bool GetAntiGravityFlag( void ){ return m_AntiGravity_Energy != 0 ? 1 : 0; }


	/*======================================*/
	//! \brief ���V�L�����Z��
	/*======================================*/
	inline virtual void ResetAntiGravity( void ){ m_AntiGravity_Energy = 0; }


	/*==================================================*/
	//! \brief �R���W�����O���[�v��Ԃ�
	/*==================================================*/
	inline int GetCollisionGroup( void ){ return m_CollisionGroup; }

	/*==================================================*/
	//! \brief �R���W�����O���[�v���Z�b�g����
	/*==================================================*/
	inline void SetCollisionGroup( int _Group ){ m_CollisionGroup = _Group; }


protected:

	/*===================================================*/
	//! \brief ���V���Ԃ̉��Z
	//! \param[in]	_Power	���Z����G�l���M�[��
	/*===================================================*/
	inline void AddAntiGravity_Energy( const int _Power )
	{ 
		if( m_LaserHitFlag )
		{
			m_AntiGravity_Energy += _Power;
			if( m_AntiGravity_Energy > MaxEnergy ){ m_AntiGravity_Energy = MaxEnergy; }
		}
	}

	/*=========================================================*/
	//! \brief �����G�l���M�[����
	//! \param[in]	_ReducePower	���炷�G�l���M�[��
	/*=========================================================*/
	inline void SubAntiGravity_Energy( const int _ReducePower )
	{
		m_AntiGravity_Energy -= _ReducePower;
		if( m_AntiGravity_Energy < 0 ){ m_AntiGravity_Energy = 0; }
	}


	std::string Name;				//!< �I�u�W�F�N�g�̖��O

	CDirect3D *m_pd3d;				//!< CDirect3D�̃|�C���^
	CPhysX *m_pPhysX;				//!< CPhysX�̃|�C���^

	std::string m_FilePass;			//!< �t�@�C���̑��΃p�X���w��
	CXFile *m_lpXFile;				//!< ���b�V���f�[�^
	ACTOR_DATA  m_ActorData;		//!< �A�N�^�[���\��������	
	NxActor *m_Actor;				//!< �A�N�^�[�{��
	NxMaterial *m_NxMaterial;		//!< �A�N�^�[�̃}�e���A���@�ǂ�ȍގ��Ȃ̂�
	
	D3DXVECTOR3 m_vPos;				//!< �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 m_vRot;				//!< �I�u�W�F�N�g�̊p�x
	D3DXVECTOR3 m_vScaling;			//!< �I�u�W�F�N�g�̊g�嗦

	bool m_DrawFlag;				//!< �`�����ɍs�����̃t���O

	static const int MaxEnergy = 1000;
	bool m_LaserHitFlag;			//!< ���[�U�[�ɓ������Ă��邩�̃t���O
	int m_AntiGravity_Energy;		//!< �A���`�O���r�e�B�[�̃G�l���M�[��

	D3DMATERIAL9 *m_Material;		//!< �}�e���A�������i�[

	int m_CollisionGroup;			//!< �R���W�����O���[�v

};