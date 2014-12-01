#include "stdafx.hpp"

#include "../GimmickBase/CGimmickBase.hpp"
#include "CLiftY.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CLiftY::CLiftY( void )
	: FreezeCount( FreezeTime )
	, m_MoveFlag( MoveUp )
{

	m_FilePass += "Object/Gimmick/Lift/Lift.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CLiftY::DiscreteInit( void )
{

	// �A�N�^�[�̍쐬
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();									// ������

	// Y�����ɂ̂ݓ����A��]���֎~����
	bodyDesc.flags |= NX_BF_FROZEN_POS_X;
	bodyDesc.flags |= NX_BF_FROZEN_POS_Z;
	bodyDesc.flags |= NX_BF_FROZEN_ROT;
	

	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();										// ������
	boxDesc.dimensions = NxVec3( 10.0f , 2.0f , 10.0f );
	
	NxActorDesc actorDesc;
	actorDesc.setToDefault();									// ������
	actorDesc.body			= &bodyDesc;						// ���I�f�[�^���i�[
	actorDesc.density		= 1000000000000000.0f;				// ���ʖ��x�ikg/m^3�j
	NxVec3 LocalPos( 0.0f , -2.0f , 0.0f );
	actorDesc.globalPose.t  = LocalPos + m_ActorData.Position;	// �V�[����ł̈ʒu(�O���[�o��)
	
	actorDesc.shapes.pushBack( &boxDesc );						// �{�b�N�X���V�[���ɓo�^

	m_Actor  = m_pPhysX->GetScene()->createActor( actorDesc );




	// �X�g�b�p�[�̃A�N�^�[
	boxDesc.setToDefault();										// ������
	boxDesc.dimensions = NxVec3( 10.0f , 0.5f , 10.0f );

	actorDesc.setToDefault();									// ������
	actorDesc.body			= nullptr;							// �ÓI�A�N�^�[
	actorDesc.globalPose.t  = NxVec3( 20000.0f, 0.0f , 0.0f );	// ��ʂ��痣�ꂽ�ꏊ�ɐݒu
	actorDesc.shapes.pushBack( &boxDesc );						// �{�b�N�X���V�[���ɓo�^

	m_Stopper = m_pPhysX->GetScene()->createActor( actorDesc );

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CLiftY::~CLiftY( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CLiftY::Exec( void )
{

	float Dist = ( m_MoveFlag == MoveUp ? 20.0f  : 2.0f );
	static const float Speed = 20.0f;


	if( FreezeCount < FreezeTime ){ FreezeCount++; return; }
	else
	{ 
		// �X�g�b�p�[����ʊO�ɔz�u
		m_Stopper->setGlobalPosition( NxVec3( 20000.0f, 0.0f , 0.0f ) );
	}

	// �i�s�����Ƀ��C���΂�
	if ( MoveCheck() < Dist )
	{
		// �J�E���^������
		FreezeCount = 0;

		if( m_MoveFlag == MoveUp )
			// ���t�g�̐^���ɐݒu�������~���̗�����h��
			m_Stopper->setGlobalPosition( m_Actor->getGlobalPosition() + NxVec3( 0.0f, -m_ActorData.Dimension.y/2 , 0.0f ) );

		// �i�s�����𔽓]
		m_MoveFlag = !m_MoveFlag;
	}
	

	if( m_MoveFlag == MoveUp )
		m_Actor->setLinearVelocity( NxVec3( 0.0f ,  Speed , 0.0f ) );
	else
		m_Actor->setLinearVelocity( NxVec3( 0.0f , -Speed , 0.0f ) );
	

	// ���W�X�V
	NxVec3 Pos = m_Actor->getGlobalPosition();
	m_vPos = CDPConverter::toDirectX( Pos );

}


/*=================================================*/
//! \brief �i�s�����Ƀ��C���΂��A������Ԃ�
/*=================================================*/
float CLiftY::MoveCheck( void )
{

	// ���C���΂��ꏊ
	NxVec3 orig = m_vPos;

	// ���C�̔�΂�����
	NxVec3 dir = NxVec3( 0.0f , m_MoveFlag ? -1.0f : 1.0f , 0.0f );

	NxRay ray( orig , dir );

	NxRaycastHit hit;
	NxReal dist;

	// ���C���΂�
	NxShape* closestShape = m_pPhysX->GetScene()->raycastClosestShape( ray , NX_ALL_SHAPES , hit , ( 1<<CPhysX::CG_DEFAULT ) | ( 1<<CPhysX::CG_STATIC ) );
	if( closestShape )
	{
		const NxVec3& worldImpact = hit.worldImpact;
		dist = hit.distance;
	} 
	else 
	{
		dist = 10000;
	}

	return (float)dist;

}