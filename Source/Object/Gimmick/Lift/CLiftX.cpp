#include "stdafx.hpp"

#include "../Application/CApplication.hpp"
#include "../GimmickBase/CGimmickBase.hpp"
#include "CLiftX.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CLiftX::CLiftX( void )
	: m_MoveFlag( MoveLeft )
{

	m_FilePass += "Object/Gimmick/Lift/Lift.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CLiftX::DiscreteInit( void )
{

	// �A�N�^�[�̍쐬
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();									// ������

	// X�����ɂ̂ݓ����A��]���֎~����
	bodyDesc.flags |= NX_BF_FROZEN_POS_Y;
	bodyDesc.flags |= NX_BF_FROZEN_POS_Z;
	bodyDesc.flags |= NX_BF_FROZEN_ROT;
	
	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();										// ������
	boxDesc.dimensions = NxVec3( 10.0f , 2.3f , 10.0f );
	
	NxActorDesc actorDesc;
	actorDesc.setToDefault();									// ������
	actorDesc.body			= &bodyDesc;						// ���I�f�[�^���i�[
	actorDesc.density		= 1000000000000000.0f;					// ���ʖ��x�ikg/m^3�j
	NxVec3 LocalPos( 0.0f , -2.0f , 0.0f );
	actorDesc.globalPose.t  = LocalPos + m_ActorData.Position;	// �V�[����ł̈ʒu(�O���[�o��)
	
	actorDesc.shapes.pushBack( &boxDesc );						// �{�b�N�X���V�[���ɓo�^

	m_Actor  = m_pPhysX->GetScene()->createActor( actorDesc );

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CLiftX::~CLiftX( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CLiftX::Exec( void )
{

	// �f�o�b�O
	if( KEYBOARD->GetState( CKeyboard::KEY_O ) == CKeyboard::KST_PUSH )
	{
		OBJECT_MANAGER->AddObject( "Box" , D3DXVECTOR3( m_vPos.x , m_vPos.y + 2.0f , m_vPos.z ) );
	}

	static const float Dist = 12.0f;
	static const float Speed = 10.0f;

	// �i�s�����Ƀ��C���΂�
	if ( MoveCheck() < Dist )
		m_MoveFlag = !m_MoveFlag;
	
	if( m_MoveFlag == MoveLeft )
		m_Actor->setLinearVelocity( NxVec3( -Speed , 0.0f , 0.0f ) );
	else
		m_Actor->setLinearVelocity( NxVec3(  Speed , 0.0f , 0.0f ) );

	NxVec3 Pos = m_Actor->getGlobalPosition();
	m_vPos = CDPConverter::toDirectX( Pos );

}


/*=================================================*/
//! \brief �i�s�����Ƀ��C���΂��A������Ԃ�
/*=================================================*/
float CLiftX::MoveCheck( void )
{

	// ���C���΂��ꏊ
	NxVec3 orig = m_vPos;

	// ���C�̔�΂�����
	NxVec3 dir = NxVec3( m_MoveFlag == MoveLeft ? -1.0f : 1.0f , 0.0f , 0.0f );
	NxRay ray( orig , dir );

	NxRaycastHit hit;
	NxReal dist;

	// ���C���΂�
	NxShape* closestShape = m_pPhysX->GetScene()->raycastClosestShape( ray , NX_ALL_SHAPES , hit );
	if(closestShape)
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
