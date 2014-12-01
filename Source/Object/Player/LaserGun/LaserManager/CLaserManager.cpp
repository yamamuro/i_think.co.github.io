#include "stdafx.hpp"

#include "../LaserBase/CLaserBase.hpp"
#include "../AntiGravity/CAntiGravityLaser.hpp"
#include "../Holding/CHoldingLaser.hpp"
#include "../AttractionAndRepulsionLaser/CAttractionAndRepulsionLaser.hpp"

#include "../Particle/CParticle.hpp"
#include "../Particle/CParticleEmitter.hpp"
#include "CLaserManager.hpp"


/*===============================================*/
//! \brief �R���X�g���N�^
/*===============================================*/
CLaserManager::CLaserManager( CPlayer *_pPlayer )
	: m_pMouse( MOUSE )
	, m_UsingLaserType( ANTI_GRAVITY )
	, m_pUsingLaser( nullptr )
{

	// �e���[�U�[�̃|�C���^���i�[
	m_pLasers[ANTI_GRAVITY]				= new CAntiGravityLaser( _pPlayer );
	m_pLasers[HOLDING]					= new CHoldingLaser( _pPlayer );
	m_pLasers[ATTRACTION_AND_REPULSION] = new CAttractionAndRepulsionLaser( _pPlayer );

	// �e���[�U�[�̐F
	m_LaserGunColor[ANTI_GRAVITY]			  = LASER_COLOR( 0.0f , 0.0f , 1.0f );
	m_LaserGunColor[HOLDING]				  = LASER_COLOR( 0.0f , 1.0f , 0.0f );
	m_LaserGunColor[ATTRACTION_AND_REPULSION] = LASER_COLOR( 0.0f , 0.0f , 0.0f );


	// �g�p���郌�[�U�[�̃|�C���^���i�[
	m_pUsingLaser = m_pLasers[m_UsingLaserType];
	// �g�p���郌�[�U�[�̐F���i�[
	m_Color = m_LaserGunColor[m_UsingLaserType];

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CLaserManager::~CLaserManager( void )
{
	for( auto &x : m_pLasers )
		SafeDelete( x );
}


/*================================*/
//! \brief �X�V
/*================================*/
void CLaserManager::Exec( void )
{

	// ���[�U�[�e�̐؂�ւ��̊m�F�A�؂�ւ�����
	ChangeLaserType();

	// ���ݎg�p���Ă��郌�[�U�[�e�̍X�V
	m_pUsingLaser->Exec();

	// �p�[�e�B�N�����ړ�������
	LASER_COLOR Dummy;
	m_pUsingLaser->ParticleUpdate( Dummy , CLaserBase::ParticleUpdateFlag );
}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CLaserManager::Draw( CDirect3D & _d3d )
{
	m_pUsingLaser->Draw( _d3d , m_Color );
}


/*===========================================*/
//! \brief ���[�U�[�̐؂�ւ�����
/*===========================================*/
void CLaserManager::ChangeLaserType( void )
{

	int MoveMent = m_pMouse->GetWheelMovement();

	// �}�E�X�̃z�C�[�����ړ��������m�F
	if( MoveMent )
	{
		m_pUsingLaser->SetLightFlag( FALSE );
		// ���[�U�[�e�̃^�C�v�؂�ւ�
		m_UsingLaserType = ( m_UsingLaserType + MoveMent ) % MAX_LASER_GUN;
		if( m_UsingLaserType < 0 ){ m_UsingLaserType += MAX_LASER_GUN; }
		// �Ή����郌�[�U�[�e���Z�b�g
		m_pUsingLaser = m_pLasers[m_UsingLaserType];

	}

	// ���[�U�[�e�̐F��؂�ւ��Ă���
	ChangeingLaserColor();

}


/*=================================================*/
//! \brief ���[�U�[�̐F��ύX����
/*=================================================*/
void CLaserManager::ChangeingLaserColor( void )
{

	static const int MaxCount = 30;				// �F�̕ύX�ɂ�����t���[����
	static int Count = 0;						// �F�̕ύX���Ɏg�p����J�E���^
	static int BeforeLaserType = ANTI_GRAVITY;	// �؂�ւ��O�̃��[�U�[�e�̃^�C�v
	static LASER_COLOR AddColor;				// �}�C�t���[���ǂꂾ���F�����Z���Ă�����

	// �g�p���Ă��郌�[�U�[�e���O�t���[���ƈႤ�����ׂ�
	if( BeforeLaserType != m_UsingLaserType )
	{ 
		
		Count = 0;
		m_Color = m_LaserGunColor[BeforeLaserType];

		AddColor = ( m_LaserGunColor[m_UsingLaserType] - m_LaserGunColor[BeforeLaserType] ) / (float)MaxCount;

		Count++;

		// ���[�U�[�^�C�v���X�V
		BeforeLaserType = m_UsingLaserType;
	}

	if( Count == 0 ){ return; }

	if( Count == MaxCount )
	{
		Count = 0;
		m_Color = m_LaserGunColor[m_UsingLaserType];
		return;
	}

	Count++;
	m_Color += AddColor;

}
