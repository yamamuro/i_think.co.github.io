#include "stdafx.hpp"

#include "../../Player/CPlayer.hpp"
#include "CHoldingLaser.hpp"


/*===============================================*/
//! \brief コンストラクタ
/*===============================================*/
CHoldingLaser::CHoldingLaser( CPlayer *_pPlayer )
	: m_HittingFlag( false )
{
	m_pPlayer = _pPlayer;

	Name = "HL";
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CHoldingLaser::~CHoldingLaser( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CHoldingLaser::Exec( void )
{
	// HL作動
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_HOLD )
		LeftClickAction();
	if( m_pMouse->GetState( CMouse::B_LEFT ) == CMouse::MST_PULL )
	{
		m_HittingFlag = false;
		m_pHitActor->clearBodyFlag( NX_BF_FROZEN_POS );
		m_pHitActor = nullptr;
	}

	// HLのキャンセル
	//if( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_PUSH )
	//	RightClickAction();
}


/*==============================================*/
//! \brief 左クリック時のアクション
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
		// 持ち運ぶことが出来ないオブジェクトに当たっていた際にはフラグを下す
		m_HittingFlag = false;
	}

	

	// パーティクルの更新
	LASER_COLOR Green( 0.0f , 1.0f , 0.0f );
	ParticleUpdate( Green , AddParticleFlag );
	
}

/*==============================================*/
//! \brief 右クリック時のアクション
/*==============================================*/
void CHoldingLaser::RightClickAction( void )
{



}