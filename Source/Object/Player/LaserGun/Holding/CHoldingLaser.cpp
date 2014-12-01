#include "stdafx.hpp"

#include "../../Player/CPlayer.hpp"
#include "CHoldingLaser.hpp"


/*===============================================*/
//! \brief �R���X�g���N�^
/*===============================================*/
CHoldingLaser::CHoldingLaser( CPlayer *_pPlayer )
	: m_HittingFlag( false )
{
	m_pPlayer = _pPlayer;

	Name = "HL";
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CHoldingLaser::~CHoldingLaser( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CHoldingLaser::Exec( void )
{
	// HL�쓮
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_HOLD )
		LeftClickAction();
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_PULL )
	{
		m_HittingFlag = false;
		m_pHitActor->clearBodyFlag( NX_BF_FROZEN_POS );
		m_pHitActor = nullptr;
	}

	// HL�̃L�����Z��
	//if( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_PUSH )
	//	RightClickAction();
}


/*==============================================*/
//! \brief ���N���b�N���̃A�N�V����
/*==============================================*/
void CHoldingLaser::LeftClickAction( void )
{

	float Dis;
	static float toObjectDis;
	static NxActor *m_HoldingActor;
	
	if( HitCheck( &Dis ) && m_HittingFlag == false /*&& Dis < 60*/ )
	{
		m_HittingFlag = true;
		toObjectDis = Dis;
		m_HoldingActor = m_pHitActor;
		m_pHitObject = OBJECT_MANAGER->GetActorToObject( m_pHitActor );
	}


	if( m_HoldingActor == m_pHitActor && m_pHitObject != nullptr && m_pHitObject->GetName() == "Box" )
	{
		CMatrix mTrans;
		CMatrix mParent = m_pPlayer->GetMatrix();
		CMatrix mWorld;

		mTrans.SetTrans( 0.0f , 0.0f , toObjectDis );

		mWorld = mTrans * mParent;

		m_pHitActor->setGlobalPosition( CDPConverter::toPhysX( mWorld.GetPos() ) );
		
		m_pHitActor->raiseBodyFlag( NX_BF_FROZEN_POS );
	}
	else
	{
		// �����^�Ԃ��Ƃ��o���Ȃ��I�u�W�F�N�g�ɓ������Ă����ۂɂ̓t���O������
		m_HittingFlag = false;
	}

	

	// �p�[�e�B�N���̍X�V
	LASER_COLOR Green( 0.0f , 1.0f , 0.0f );
	ParticleUpdate( Green , AddParticleFlag );
	
}

/*==============================================*/
//! \brief �E�N���b�N���̃A�N�V����
/*==============================================*/
void CHoldingLaser::RightClickAction( void )
{



}