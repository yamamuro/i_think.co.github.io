#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "Fragment\CFragment.hpp"
#include "CBreakWall.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CBreakWall::CBreakWall( void )
	: m_BreakedFlag( false )
{

	m_BreakWallFilePass = m_FilePass + "Object/Object/BreakWall/BreakWall.x";
	// ���b�V���̓ǂݍ���
	m_lpBreakWallXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_BreakWallFilePass );


	m_BreakedWalldFilePass = m_FilePass + "Object/Object/BreakWall/BreakedWall.x";
	// ���b�V���̓ǂݍ���
	m_lpBreakedWallXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_BreakedWalldFilePass );


 	m_DrawFlag = true;

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CBreakWall::DiscreteInit( void )
{

	// �j�󂳂��O�̃A�N�^�[
	// �f�X�N���v�^
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();							// ������
	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();								// ������
	boxDesc.dimensions = NxVec3( 10.0f , 1.0f , 10.0f );
	boxDesc.localPose.t = NxVec3( 0.0f , -1.0f , 0.0f );
	NxActorDesc actorDesc;
	actorDesc.setToDefault();							// ������
	actorDesc.body = NULL;
	actorDesc.globalPose.t = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	actorDesc.shapes.pushBack( &boxDesc );				// �{�b�N�X���A�N�^�[�ɓo�^
	m_CollisionGroup = CPhysX::CG_STATIC;
	ACTOR_DATA *Tmp = new ACTOR_DATA;	// CPhysX�ŉ��
	*Tmp = m_ActorData;
	actorDesc.userData = Tmp;
	m_BreakWallActor = m_pPhysX->GetScene()->createActor( actorDesc );
	
	// �Փ˔�����Ď�����
	m_pPhysX->InitContactReportsPerPair( m_BreakWallActor );


	// �j�󂳂ꂽ��Ԃ̃A�N�^�[�̍쐬
	m_BreakedWallActor = m_pPhysX->CreateConvexActor( m_lpBreakedWallXFile->GetlpMesh() );
	m_BreakedWallActor->setGlobalPosition( NxVec3( 20000.0f , 20000.0f , 20000.0f ) );


	// ��юU��j�Ђ̍쐬
	CRandomize Rand;
	int i=0;
	for( auto &x : m_Fragment )
	{
		// �������痣�ꂽ�ꏊ�ɒ�~�������������Ă���
		x = new CFragment( Rand.GetRandomize( 1 , FragmentType ) , D3DXVECTOR3( 100000.0f , 0.0f , 3.0f*i++ ) );
		x->DiscreteInit();
		OBJECT_MANAGER->AddObject( x );
	}

	// �I�[�o�[���b�v���Ɏg�p����f�[�^
	m_worldBox.extents	= NxVec3( 0.5f , 2.0f , 0.5f );
	m_worldBox.center = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );

	m_pPhysX->SetActorCollisionGroup( m_BreakWallActor , CPhysX::CG_STATIC );

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CBreakWall::~CBreakWall( void )
{
	//!< \note CFrafment�̃����������CObjectManager�ōs��
}


/*================================*/
//! \brief �X�V
/*================================*/
void CBreakWall::Exec( void )
{

	// �X�C�b�`�̃v�b�V������X�V
	NxMat33 NxMat;
	NxQuat quat( 0.0f , NxVec3( 0 , 1 , 0 ) ); 
	NxMat.fromQuat( quat );
	m_worldBox.rot = NxMat;

	if ( m_BreakedFlag == false && m_pPhysX->GetScene()->checkOverlapOBB( m_worldBox , NX_DYNAMIC_SHAPES , 0xffffffff , NULL ) == true )
	{
		if( static_cast<ACTOR_DATA*>( m_BreakWallActor->userData )->ContactPairFlag &&
			static_cast<ACTOR_DATA*>( static_cast<ACTOR_DATA*>( m_BreakWallActor->userData )->pContactActor->userData )->pObject->GetCollisionGroup() == CPhysX::CG_HEAVY_OBJECT )
		{
			CRandomize Rand;
			m_BreakedWallActor->setGlobalPosition( CDPConverter::toPhysX( m_vPos ) );
			for( auto &x : m_Fragment )
			{
				x->GetActor()->setGlobalPosition( CDPConverter::toPhysX( m_vPos ) + NxVec3( (float)Rand.GetRandomize( -6.0 , 6.0 ), (float)Rand.GetRandomize( -3.0 , 3.0 ) , (float)Rand.GetRandomize( -6.0 , 6.0 ) ) );
				
				x->GetActor()->putToSleep();
				x->GetActor()->wakeUp();
			}

			m_BreakWallActor->setGlobalPosition( NxVec3( 20000.0f , 20000.0f , 20000.0f ) );

			m_BreakedFlag = true;
		}
	}


#ifdef _DEBUG
	if( KEYBOARD->GetState( CKeyboard::KEY_U ) == CKeyboard::KST_PUSH )
	{
		m_BreakedFlag = !m_BreakedFlag;

		if( !m_BreakedFlag )	// ����O�̏��
		{
			m_BreakWallActor->setGlobalPosition( CDPConverter::toPhysX( m_vPos ) );

			m_BreakedWallActor->setGlobalPosition( NxVec3( 20000.0f , 20000.0f , 20000.0f ) );
			for( auto &x : m_Fragment )
			{
				x->GetActor()->setGlobalPosition( NxVec3( 20000.0f , 20000.0f , 20000.0f ) );
				x->GetActor()->putToSleep();
			}
			
		}
		else	// ��ꂽ���
		{

			CRandomize Rand;
			m_BreakedWallActor->setGlobalPosition( CDPConverter::toPhysX( m_vPos ) );
			for( auto &x : m_Fragment )
			{
				x->GetActor()->setGlobalPosition( CDPConverter::toPhysX( m_vPos ) + NxVec3( (float)Rand.GetRandomize( -6.0 , 6.0 ), (float)Rand.GetRandomize( -3.0 , 3.0 ) , (float)Rand.GetRandomize( -6.0 , 6.0 ) ) );
				
				x->GetActor()->putToSleep();
				x->GetActor()->wakeUp();
			}

			m_BreakWallActor->setGlobalPosition( NxVec3( 20000.0f , 20000.0f , 20000.0f ) );

		}
	}
#endif
	

}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CBreakWall::Draw( CDirect3D & _d3d )
{

	if( m_lpBreakWallXFile == nullptr ||
		m_lpBreakedWallXFile == nullptr ){ return; }

	D3DXMATRIXA16 matWorld;
	if( !m_BreakedFlag )
	{
		// �}�g���b�N�X�̓]�u
		CDPConverter::SetMatrix( &matWorld , m_BreakWallActor );
		_d3d.SetWorldTrans( &matWorld );
		m_lpBreakWallXFile->Draw( _d3d.GetDevice() );
	}
	else
	{
		// �}�g���b�N�X�̓]�u
		CDPConverter::SetMatrix( &matWorld , m_BreakedWallActor );
		_d3d.SetWorldTrans( &matWorld );
		m_lpBreakedWallXFile->Draw( _d3d.GetDevice() );
	}

}