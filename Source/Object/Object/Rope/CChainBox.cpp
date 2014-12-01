#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Box/CBox.hpp"
#include "CRope.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "CChainBox.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CChainBox::CChainBox( void )
{
	m_DrawFlag = true;
}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CChainBox::DiscreteInit( void )
{
 	m_pBox  = OBJECT_MANAGER->AddObject( "Box"  , m_vPos - D3DXVECTOR3( 0.0f, 4.0f , 0.0f ) );
	m_pRope = OBJECT_MANAGER->AddObject( "Rope" , m_vPos );

	static_cast<CBox*>(m_pBox)->SetisJointFlag( true );
	m_pBox->GetActor()->setMass( 3000000.0f );
	m_pBox->GetActor()->raiseBodyFlag( NX_BF_FROZEN_POS_X );
	m_pBox->GetActor()->raiseBodyFlag( NX_BF_FROZEN_POS_Z );
	m_Joint = m_pPhysX->CreateRevoluteJoint( static_cast<CRope*>(m_pRope)->GetRopeSegs()[0] , m_pBox->GetActor() , NxVec3( m_vPos ) , NxVec3( 0.0f, 1.0f , 0.0f ) );
	
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CChainBox::~CChainBox( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CChainBox::Exec( void )
{

	static bool AntiGravityFlag = false;
	// ジョイント終了
	if( m_pPhysX->GetJointNotify()->toBreak )
	{
		AntiGravityFlag = false;
		static_cast<CBox*>(m_pBox)->SetisJointFlag( false );
		m_pBox->GetActor()->clearBodyFlag( NX_BF_FROZEN_POS_X );
		m_pBox->GetActor()->clearBodyFlag( NX_BF_FROZEN_POS_Z );
		m_pBox->GetActor()->setMass( static_cast<CBox*>(m_pBox)->GetDefMass() );
	}
	// ジョイント中
	else
	{
		static int Count = 0;
		if( m_pBox->GetAntiGravityFlag() ){ AntiGravityFlag = true; }
		// 持ち上げて落としたとき
		if( AntiGravityFlag == true && !m_pBox->GetAntiGravityFlag() )
		{
			// 上まで持ち上げるとその力でジョイントが壊されるので少しの間落下するのを待つ
			Count++;
			static const int WaitFrame = 40;
			if( Count > WaitFrame )
			{
				Count = 0;
				m_Joint->setBreakable( 20000000.0f , 100000000000000000.0f );
			}
		}
	}
}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CChainBox::Draw( CDirect3D & _d3d )
{
}