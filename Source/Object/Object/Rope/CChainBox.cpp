#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Box/CBox.hpp"
#include "CRope.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "CChainBox.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CChainBox::CChainBox( void )
{
	m_DrawFlag = true;
}

/*===================================*/
//! \brief �ʂ̏�����
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
//! \brief �f�X�g���N�^
/*=====================================*/
CChainBox::~CChainBox( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CChainBox::Exec( void )
{

	static bool AntiGravityFlag = false;
	// �W���C���g�I��
	if( m_pPhysX->GetJointNotify()->toBreak )
	{
		AntiGravityFlag = false;
		static_cast<CBox*>(m_pBox)->SetisJointFlag( false );
		m_pBox->GetActor()->clearBodyFlag( NX_BF_FROZEN_POS_X );
		m_pBox->GetActor()->clearBodyFlag( NX_BF_FROZEN_POS_Z );
		m_pBox->GetActor()->setMass( static_cast<CBox*>(m_pBox)->GetDefMass() );
	}
	// �W���C���g��
	else
	{
		static int Count = 0;
		if( m_pBox->GetAntiGravityFlag() ){ AntiGravityFlag = true; }
		// �����グ�ė��Ƃ����Ƃ�
		if( AntiGravityFlag == true && !m_pBox->GetAntiGravityFlag() )
		{
			// ��܂Ŏ����グ��Ƃ��̗͂ŃW���C���g���󂳂��̂ŏ����̊ԗ�������̂�҂�
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
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CChainBox::Draw( CDirect3D & _d3d )
{
}