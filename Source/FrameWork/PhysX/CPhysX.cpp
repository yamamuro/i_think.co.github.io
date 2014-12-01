 #include "stdafx.hpp"

#include "Direct3D\CDirect3D.hpp"
#include "Cloth/NxCooking.h"
#include "Cloth/Stream.h"
#include "CPhysX.hpp"


/*=======================================*/
//! CPhysX�̃|�C���^�̒�`
/*=======================================*/
CPhysX* CPhysX::m_pInstance	= nullptr;

/*============================================*/
//! �R���W�����O���[�v�̃x�N�^�̒�`
/*============================================*/
std::vector< NxActor* > CPhysX::m_Data;


/*=======================================*/
//! �V�[���̍X�V�X�s�[�h
/*=======================================*/
const float CPhysX::SIMULATE_STEP = 1.0f / FRAME_RATE;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CPhysX::CPhysX( void )
	: m_pPhysicsSDK( nullptr )
	, m_lpVB( nullptr )
	, m_pScene( nullptr )
	, m_pDebugRender( nullptr )
{

#ifdef _DEBUG
	m_DrawPhysX = false;
#else
	m_DrawPhysX = false;
#endif
	
}

/*==================================*/
//! \brief ������
//! \return ���� = true 
//! \return ���s = fasle
/*==================================*/
bool CPhysX::Initialize( void )
{

	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	m_pPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION , NULL , NULL , desc , &errorCode );
	if( m_pPhysicsSDK == nullptr ){
		ErrorMessage( "\nPhysX��SDK�̏������Ɏ��s���܂���\n\n" );
		return false;
	}

	// �߂荞�ݗ�
	m_pPhysicsSDK->setParameter( NX_SKIN_WIDTH , 0.025f );
#ifdef _DEBUG
    m_pPhysicsSDK->setParameter( NX_VISUALIZATION_SCALE			, 1 );   
    m_pPhysicsSDK->setParameter( NX_VISUALIZE_COLLISION_SHAPES  , 1 );   
    m_pPhysicsSDK->setParameter( NX_VISUALIZE_ACTOR_AXES		, 2 );   
    m_pPhysicsSDK->setParameter( NX_VISUALIZE_CONTACT_POINT		, 1 );   
    m_pPhysicsSDK->setParameter( NX_VISUALIZE_CONTACT_NORMAL	, 1 );  
	m_pPhysicsSDK->setParameter( NX_VISUALIZE_JOINT_LIMITS		, 1 );
	m_pPhysicsSDK->setParameter( NX_VISUALIZE_JOINT_LOCAL_AXES	, 1 );

	m_pDebugRender = new CDebugRender();
#endif

	// �V�[���̐���
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3( 0.0f , -98.1f/*-9.81f*/ , 0.0f );			// �d�͉����x
	
	// �n�[�h�E�F�A�̗L�����m�F
	NxHWVersion HWVersion = m_pPhysicsSDK->getHWVersion();
	// �Ή����Ă���̂Ńt���O�����Ă�
	if( HWVersion != NX_HW_VERSION_NONE )
	{
		sceneDesc.simType = NX_SIMULATION_HW;
	}

	m_pScene = m_pPhysicsSDK->createScene( sceneDesc );
	if( m_pScene == nullptr ){
		ErrorMessage( "\nPhysX�̃V�[���̍쐬�Ɏ��s���܂���\n\n" );
		return false;
	}

	// NxScene�ɃC�x���g�ǉ�
	m_ContactCallBack = new ContactCallBack();
	m_pScene->setUserContactReport( m_ContactCallBack );

	m_JointNotify = new MyJointNotify();
	m_pScene->setUserNotify( m_JointNotify );


	
	// �f�t�H���g�̃}�e���A����ݒ�
	NxMaterial *defaultMaterial = m_pScene->getMaterialFromIndex( 0 );
	defaultMaterial->setRestitution( 0.0f );			// ���˕Ԃ�W���̎w��
	defaultMaterial->setStaticFriction( 8.9f );			// �Î~���C�W���̎w��
	defaultMaterial->setDynamicFriction( 9.9f );		// �����C�W���̎w��
	
	CreateDebugBase();

	InitActorCollisionGroups(); 
	
	DEBUG->PrintConsole( "PhysX�̏������ɐ������܂���...\n" , CDebug::ConPreside );

	return true;

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CPhysX::~CPhysX( void )
{
	Finalize();
}


/*==================================*/
//! \brief �I������
/*==================================*/
void CPhysX::Finalize( void )
{
	
	AllDeleteActor();

	if( m_pPhysicsSDK != nullptr )
	{
		if( m_pScene != nullptr )
		{

			// �}�e���A���̍폜
			int MatNum = m_pScene->getNbMaterials();
			for( int i=1 ; i<MatNum ; i++ )	// 0�Ԗڂ̓f�t�H���g�̃}�e���A���Ȃ̂ō폜���Ȃ�
			{
				NxMaterial *Material = m_pScene->getMaterialFromIndex( i );
				m_pScene->releaseMaterial( *Material );
			}

			SafeDelete( m_ContactCallBack );
			SafeDelete( m_JointNotify );
			m_pPhysicsSDK->releaseScene( *m_pScene );
			SafeDelete( VARIABLE_DATA( m_pDebugRender ) );
		}
		m_pScene = nullptr;
		NxReleasePhysicsSDK( m_pPhysicsSDK );
		m_pPhysicsSDK = nullptr;
	}
	
}


/*=================================================================*/
//! \brirf �A�N�^�[�̓������V�[����ŃV�~�����[�V��������
/*=================================================================*/
void CPhysX::Simulation( void )
{

	if( m_pScene == nullptr ){ return; }

	// PhysX ���ԃX�e�b�v�ݒ�
	m_pScene->simulate( SIMULATE_STEP );

	m_pScene->flushStream();
	m_pScene->fetchResults( NX_RIGID_BODY_FINISHED , true );

}


/*=====================================================================*/
//! \brief �R���W�����O���[�v�̃Z�b�g
//! \param[in]	*_Actor		�Z�b�g����A�N�^�[
//! \param[in]	_Group		�Z�b�g����O���[�vID
/*=====================================================================*/
void CPhysX::SetActorCollisionGroup( NxActor *_Actor , NxCollisionGroup _Group )
{
	if( _Actor != nullptr )
	{
		NxU32 nbShapes = _Actor->getNbShapes();
		NxShape*const* Shapes = _Actor->getShapes();
		             
		ACTOR_DATA *Tmp = static_cast<ACTOR_DATA*>( _Actor->userData );
		if( Tmp != nullptr )
		{
			Tmp->pObject->SetCollisionGroup( _Group );
		}
		while( nbShapes-- )
		{
			Shapes[nbShapes]->setGroup( _Group );
		}
		
	}
}


/*==========================================================*/
//! \brief �R���W�����O���[�v�̏�����
/*==========================================================*/
void CPhysX::InitActorCollisionGroups( void )
{
	// �R���W�������Ď�����
	m_pScene->setGroupCollisionFlag( CG_DEFAULT      , CG_HEAVY_OBJECT , true );
	m_pScene->setGroupCollisionFlag( CG_HEAVY_OBJECT , CG_STATIC       , true );
	m_pScene->setGroupCollisionFlag( CG_DEFAULT      , CG_STATIC       , true );


	m_pScene->setGroupCollisionFlag( CG_PARTICLE , CG_DEFAULT      , true );
	m_pScene->setGroupCollisionFlag( CG_PARTICLE , CG_HEAVY_OBJECT , true );
	m_pScene->setGroupCollisionFlag( CG_PARTICLE , CG_STATIC       , true );
	m_pScene->setGroupCollisionFlag( CG_PARTICLE , CG_PLAYER       , true );
	m_pScene->setGroupCollisionFlag( CG_PARTICLE , CG_THROUGH      , true );
	m_pScene->setGroupCollisionFlag( CG_PARTICLE , CG_PARTICLE     , true );
	

}

/*====================================================*/
//! \brief ���[�U���|�[�g�̏�����
/*====================================================*/
void CPhysX::InitContactReportsPerPair( NxActor *_Actor )
{
	
	if( m_Data.size() != 0 )
	{
		for( auto &x : m_Data )
		{
			// �Փˎ��̔����Ԃ�
			// ���������u�ԁ@�A�@�������Ă���@�A�@���ꂽ�@��3�^�C�v
			m_pScene->setActorPairFlags( *_Actor , *x , NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );
		}
	}
	m_Data.push_back( _Actor );
	
}


/*===============================================*/
//! \brief �A�N�^�[���폜����
//! \param[in]	*_pActor	�폜����A�N�^�[
//! \note ������
/*===============================================*/
void DeleteActor( NxActor* _pActor )
{



}

/*==============================================*/
//! \brief ���݂��Ă���A�N�^�[��S�č폜
/*==============================================*/
void CPhysX::AllDeleteActor( void )
{

	int actorCount;
	NxActor **ppActors;

	if( m_pPhysicsSDK == nullptr ){ return; }

	actorCount = m_pScene->getNbActors();
	ppActors   = m_pScene->getActors();

	// �A�N�^�[��1�ł����݂���Ȃ�S�č폜����
	if( actorCount >= 1 )
	{
		DEBUG->PrintConsole( "===Release UserData===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		while( actorCount-- )
		{
			NxActor* pTmp = *ppActors++;
			SafeDelete( VARIABLE_DATA( pTmp->userData ) );
			m_pScene->releaseActor( *pTmp );
		}
		DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );
	}

	m_Data.clear();

}

/*=========
�ȈՂ̃{�b�N�X�쐬
======*/
NxActor* CPhysX::CreateBox(const NxVec3& pos, const NxVec3& boxDim, const NxReal density)
{

	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set( boxDim.x , boxDim.y , boxDim.z );
	boxDesc.localPose.t = NxVec3( 0.0f , boxDim.y , 0.0f );

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack( &boxDesc );
	actorDesc.globalPose.t = pos;

	NxBodyDesc bodyDesc;
	if ( density )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = 0;
	}
	return m_pScene->createActor( actorDesc );	
}

/*===========
//! �ȈՂ̃J�v�Z��
*/
NxActor* CPhysX::CreateCapsule( const NxVec3 &_Pos , const NxReal _Height , const NxReal _Radius , const NxReal _Density , BOOL flag , int _MaterialIndex )
{

	assert( m_pScene != 0 );

	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.height = _Height;
	capsuleDesc.radius = _Radius;
	capsuleDesc.localPose.t = NxVec3( 0.0f , _Radius + 0.5f * _Height , 0.0f );

	capsuleDesc.materialIndex = _MaterialIndex;

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack( &capsuleDesc );
	actorDesc.globalPose.t = _Pos;

	NxBodyDesc bodyDesc;
	if ( _Density )
	{

		if( !flag )
		{
			//bodyDesc.flags |= NX_BF_FROZEN_POS_X;
			//bodyDesc.flags |= NX_BF_FROZEN_POS_Z;
			bodyDesc.flags |= NX_BF_KINEMATIC;
			bodyDesc.flags |= NX_BF_FROZEN_ROT_Y;	
		}
		actorDesc.body = &bodyDesc;
		actorDesc.density = _Density;
	}
	else
	{
		actorDesc.body = 0;
	}

	return m_pScene->createActor( actorDesc );	

}

/*=======================================================================*/
//! \brief �A�N�^�[�̒ǉ�
//! \param[in]	*_pActorData			�쐬����A�N�^�[�̃f�[�^
//! \return ���� = �ǉ������A�N�^�[
//! \return ���s = nullptr
/*=======================================================================*/
NxActor* CPhysX::CreateActor( ACTOR_DATA *_pActorData )
{

	if( m_pScene == nullptr ){ return nullptr; }	

	ACTOR_DATA* data = new ACTOR_DATA;
	*data = *_pActorData;

	// �f�X�N���v�^	
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();							// ������
	bodyDesc.angularDamping	= data->AngularDamping;		// ��]�ɂ�鐊�ތW��


	NxActorDesc actorDesc;

	if( false ){}
	else if( data->Type == 'B' )			// �{�b�N�X�̍쐬
	{

		NxBoxShapeDesc boxDesc;
		boxDesc.setToDefault();		// ������
		boxDesc.dimensions = data->Dimension;
		boxDesc.skinWidth = data->SkinWidth;
		boxDesc.localPose.t     = data->LocalPosition;							// ���[�J�����W
		boxDesc.localPose.M     = data->LocalRotation;							// ���[�J���Ȏp��

		// �}�e���A���̒ǉ�
		if( data->MatDesc.isInit )
		{
			m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			boxDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}
		
		actorDesc.setToDefault();	// ������
		// ���I�ł����bodyDesc�@�ÓI�ł����NULL���i�[����
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// ���I�f�[�^���i�[
		actorDesc.density		= data->Density;								// ���ʖ��x�ikg/m^3�j
		actorDesc.globalPose.t  = data->Position;								// �V�[����ł̈ʒu(�O���[�o��)
		actorDesc.globalPose.M  = data->Rotation;								// �V�[����ł̉�]�p�x
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &boxDesc );									// �{�b�N�X���A�N�^�[�ɓo�^
		
	}
	else if( data->Type == 'S' )			// �X�t�B�A�̍쐬
	{
	
		NxSphereShapeDesc sphereDesc;
		sphereDesc.setToDefault();		// ������
		sphereDesc.radius = data->Radius;
		sphereDesc.skinWidth = data->SkinWidth;
		sphereDesc.localPose.t  = data->LocalPosition;							// ���[�J�����W
		sphereDesc.localPose.M  = data->LocalRotation;							// ���[�J���Ȏp��

		// �}�e���A���̒ǉ�
		if( data->MatDesc.isInit )
		{
			m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			sphereDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}

		actorDesc.setToDefault();	// ������
		// ���I�ł����bodyDesc�@�ÓI�ł����NULL���i�[����
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// ���I�f�[�^���i�[
		actorDesc.density		= data->Density;								// ���ʖ��x�ikg/m^3�j
		actorDesc.globalPose.t  = data->Position;								// �V�[����ł̈ʒu(�O���[�o��)
		actorDesc.globalPose.M  = data->Rotation;								// �V�[����ł̉�]�p�x
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &sphereDesc );								// �����A�N�^�[�ɓo�^

	}
	else if( data->Type == 'C' )			// �J�v�Z���̍쐬
	{

		NxCapsuleShapeDesc capsuleDesc;
		capsuleDesc.setToDefault();		// ������
		capsuleDesc.radius = data->Radius;
		capsuleDesc.height = data->Height;
		capsuleDesc.skinWidth = data->SkinWidth;
		capsuleDesc.localPose.t = data->LocalPosition;							// ���[�J�����W
		capsuleDesc.localPose.M = data->LocalRotation;							// ���[�J���Ȏp��
		
		// �}�e���A���̒ǉ�
		if( data->MatDesc.isInit )
		{
			m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			capsuleDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}

		actorDesc.setToDefault();	// ������
		// ���I�ł����bodyDesc�@�ÓI�ł����NULL���i�[����
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// ���I�f�[�^���i�[
		actorDesc.density		= data->Density;								// ���ʖ��x�ikg/m^3�j
		actorDesc.globalPose.t  = data->Position;								// �V�[����ł̈ʒu(�O���[�o��)
		actorDesc.globalPose.M  = data->Rotation;								// �V�[����ł̉�]�p�x
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &capsuleDesc );								// �J�v�Z�����A�N�^�[�ɓo�^

	}
	else if( data->Type == 'P' )
	{

		NxCapsuleShapeDesc capsuleDesc;
		capsuleDesc.setToDefault();		// ������
		capsuleDesc.radius = data->Radius;
		capsuleDesc.height = data->Height;
		capsuleDesc.skinWidth = 0.0f;
		capsuleDesc.localPose.t = data->LocalPosition;							// ���[�J�����W
		capsuleDesc.localPose.M = data->LocalRotation;							// ���[�J���Ȏp��

		// �}�e���A���̒ǉ�
		if( data->MatDesc.isInit )
		{
			NxMaterial *Mat;
			Mat = m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			capsuleDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}

		actorDesc.setToDefault();	// ������
		// ���I�ł����bodyDesc�@�ÓI�ł����NULL���i�[����
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// ���I�f�[�^���i�[
		actorDesc.flags			|= NX_BF_KINEMATIC;
		actorDesc.density		= data->Density;								// ���ʖ��x�ikg/m^3�j
		actorDesc.globalPose.t  = data->Position;								// �V�[����ł̈ʒu(�O���[�o��)
		actorDesc.globalPose.M  = data->Rotation;								// �V�[����ł̉�]�p�x
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &capsuleDesc );								// �J�v�Z�����A�N�^�[�ɓo�^

	}
	else if( data->Type == 'G' )
	{
		
		// �n�ʂ̃Z�b�g
		NxPlaneShapeDesc PlaneDesc;
		PlaneDesc.setToDefault();
		actorDesc.body = NULL;
		actorDesc.userData = data;
		actorDesc.shapes.pushBack( &PlaneDesc );
		
	}
	else
	{
		// �m�ۂ������̂́A���ǎg��Ȃ�����ACTOR_DATA�����
		SafeDelete( data );
		return nullptr;
	}

	// �����ŃA�N�^�[���V�[���ɓo�^
	NxActor *Actor = m_pScene->createActor( actorDesc );
	// �Փ˔�����Ď�����
	InitContactReportsPerPair( Actor );
	
	return Actor;
	
}


/*=====================================================================*/
//! \brief ���[�v���쐬����
//! \param[out]		**_ropeSegs		���[�v�̃A�N�^�[�̃|�C���^
//! \param[out]		**_ropeLinks	���[�v�̃W���C���g�����̃|�C���^
//! \param[in]		_Pos			�z�u������W
//! \param[in]		_segHeight		1�̃A�N�^�[�̍���
//! \param[in]		_segRadius		1�̃A�N�^�[�̔��a
//! \param[in]		_nbSegments		�����̃A�N�^�[���q����
//! \param[in]		_density		1�̃A�N�^�[�̖��x
//! \param[in]		_MaterialIndex	�}�e���A���̃C���f�b�N�X�ԍ�
/*=====================================================================*/
void CPhysX::CreateRope( NxActor **_ropeSegs , NxSphericalJoint **_ropeLinks ,  const NxVec3& _Pos , const NxReal _segHeight , const NxReal _segRadius , const int _nbSegments , const NxReal _density , int _MaterialIndex )
{

	// ���[�v�̃p�[�c������Ă���
	for( int i=0 ; i<_nbSegments ; i++ )
	{
		_ropeSegs[i] = CreateCapsule( _Pos + NxVec3( 0.0f , i * ( 2 * _segRadius + _segHeight ) , 0.0f ) , _segHeight , _segRadius , _density , i == _nbSegments-1 ? FALSE : TRUE , _MaterialIndex );
	}

	// �q���Ă���
	for( int i=0 ; i<_nbSegments-1 ; i++ )
	{
		_ropeLinks[i] = CreateRopeSphericalJoint( _ropeSegs[i] , _ropeSegs[i+1] , _Pos + NxVec3( 0.0f , ( i + 1 ) * ( 2 * _segRadius + _segHeight ) , 0.0f ) , NxVec3( 0.0f , 1.0f , 0.0f ) ); 
	}

}


/*================================================================*/
//! \brief �A�N�^�[���W���C���g����
//! \param[in]	*_a0			�W���C���g����A�N�^�[
//! \param[in]	*_a1			�W���C���g����A�N�^�[
//! \param[in]	&_globalAnchor	�W���C���g������W
//! \param[in]	&_globalAxis	�W���C���g���鎲
/*================================================================*/
NxSphericalJoint* CPhysX::CreateRopeSphericalJoint( NxActor *_a0 , NxActor *_a1 , const NxVec3 &_globalAnchor , const NxVec3 &_globalAxis )
{

	NxSphericalJointDesc sphericalDesc;
	sphericalDesc.actor[0] = _a0;
	sphericalDesc.actor[1] = _a1;
	sphericalDesc.setGlobalAnchor( _globalAnchor );
	sphericalDesc.setGlobalAxis( _globalAxis );

	sphericalDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
	sphericalDesc.twistLimit.low.value = -(NxReal)1.0 * NxPi;//0.1
	sphericalDesc.twistLimit.high.value = (NxReal)1.0 * NxPi;//0.1

	sphericalDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
	NxSpringDesc ts;
	ts.spring = 10.0f;				// �΂ˌW��
	ts.damper = 100000000.0f;		// �_���p�[�W���@��(������)�ɖ߂낤�Ƃ����
	ts.targetValue = 111000.0f;		// ?
	sphericalDesc.twistSpring = ts;

	sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
	sphericalDesc.swingLimit.value = 3.0f;//0.3 * NxPi		// �U��񂵉^���̉��͈�

	sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
	NxSpringDesc ss;
	ss.spring = 10.0f;				// �΂ˌW��
	ss.damper = 100000000.0f;		// �_���p�[�W���@��(������)�ɖ߂낤�Ƃ����
	ss.targetValue = 111000.0f;		// ?
	sphericalDesc.swingSpring = ss;

	return (NxSphericalJoint*)m_pScene->createJoint( sphericalDesc );

}


/*======================================================================*/
//! \brief ���b�V������A�N�^�[���쐬����
//! \param[in]	_lpMesh		���b�V���f�[�^
//! \return ���� = �ǉ������A�N�^�[
//! \return ���s = nullptr
/*======================================================================*/
NxActor* CPhysX::CreateConvexActor( LPD3DXMESH _lpMesh )
{

	NxTriangleMeshDesc meshDesc;
	NxU32 flg = 0;
	LPBYTE pIndex = nullptr;
	LPBYTE pBits = nullptr;
	NxVec3* pVec = nullptr;
	 
	// ���b�V�����璸�_�f�[�^���쐬
	LPD3DXMESH lpClone;
	if( _lpMesh == nullptr ){ return nullptr; }
	_lpMesh->CloneMeshFVF( D3DXMESH_MANAGED | D3DXMESH_NPATCHES , D3DFVF_XYZ , DIRECTX->GetDevice() , &lpClone );


	LPDIRECT3DINDEXBUFFER9 pIndexBuffer = nullptr;
	_lpMesh->GetIndexBuffer( &pIndexBuffer );
	D3DINDEXBUFFER_DESC desc;
	pIndexBuffer->GetDesc( &desc );
	SafeRelease( pIndexBuffer );

	if( desc.Format == D3DFMT_INDEX16 )
	{
		flg |= NX_CF_16_BIT_INDICES;
	}
	meshDesc.flags = flg;

	// ���_���𒲂ׂ�
	meshDesc.numVertices      = lpClone->GetNumVertices();
	// 1�̒��_�̃T�C�Y
	meshDesc.pointStrideBytes = lpClone->GetNumBytesPerVertex();
	// �O�p�|���S����
	meshDesc.numTriangles     = lpClone->GetNumFaces();

	// �O�p�|���S���P���̃C���f�b�N�X�̃T�C�Y
	if( flg & NX_CF_16_BIT_INDICES )
	{
		meshDesc.triangleStrideBytes = 3 * sizeof( WORD );
	}
	else
	{
		meshDesc.triangleStrideBytes = 3 * sizeof( DWORD );
	}

	// ���_���i�[
	pVec = new NxVec3[meshDesc.numVertices];

	
	// ���_�o�b�t�@���璸�_���W���擾
	lpClone->LockVertexBuffer( 0 , (void**)&pBits );					// ���b�N�J�n
	CopyMemory( pVec, pBits, meshDesc.pointStrideBytes * meshDesc.numVertices );
	meshDesc.points = pVec;
	lpClone->UnlockVertexBuffer();										// �������I������̂ŃA�����b�N

	pIndex = new BYTE[ meshDesc.triangleStrideBytes * meshDesc.numTriangles ];

	//�C���f�b�N�X�o�b�t�@���璸�_���W���擾
	lpClone->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pBits );		// ���b�N�J�n
	CopyMemory( pIndex, pBits, meshDesc.triangleStrideBytes * meshDesc.numTriangles );
	meshDesc.triangles = pIndex;
	lpClone->UnlockIndexBuffer();										// �������I������̂ŃA�����b�N

	// �R���x�b�N�X�A�N�^�[�̍쐬�J�n
	NxInitCooking();

	MemoryWriteBuffer Buff;
	// �V���A����
	bool State = NxCookTriangleMesh( meshDesc, Buff );

	SafeDeleteArray( pVec , "ConvexMesh��Vertex" );
	SafeDeleteArray( pIndex , "ConvexMesh��Index" );

	if( State )
	{

		NxTriangleMesh *pMesh = nullptr;
		
		pMesh = m_pPhysicsSDK->createTriangleMesh( MemoryReadBuffer( Buff.data ) );
		if( pMesh == nullptr ){ return nullptr; }

		NxTriangleMeshShapeDesc meshShapeDesc;
		meshShapeDesc.meshData = pMesh;
		
		NxActorDesc actorDesc;
		assert( meshShapeDesc.isValid() );

		// �A�N�^�[�̃}�e���A����ݒ肷��
		NxMaterialDesc materialDesc;
		materialDesc.restitution		= 0.0f;
		materialDesc.dynamicFriction	= 0.0f;
		materialDesc.staticFriction		= 0.0f;
		int MaterialIndex				= m_pScene->createMaterial( materialDesc )->getMaterialIndex();

		meshShapeDesc.materialIndex = MaterialIndex;

		actorDesc.shapes.pushBack( &meshShapeDesc );

		NxBodyDesc bodyDesc;
		bodyDesc.setToDefault();
		bodyDesc.flags |= NX_BF_KINEMATIC;

		actorDesc.body		= &bodyDesc;
		actorDesc.density	= 700000000000000000000000000000.0f;
	   
		actorDesc.globalPose.t  = NxVec3( 0.0f , 0.0f , 0.0f );
		assert( actorDesc.isValid() );
		NxActor* Actor = m_pScene->createActor( actorDesc );
		assert( Actor );	

		// �R���x�b�N�X�A�N�^�[�̍쐬�I��
		NxCloseCooking();

		return Actor;

	}

	// �R���x�b�N�X�A�N�^�[�̍쐬�I��
	NxCloseCooking();

	return nullptr;

}


NxRevoluteJoint* CPhysX::CreateRevoluteJoint( NxActor* a0 , NxActor* a1 , NxVec3 globalAnchor , NxVec3 globalAxis )
{
	NxRevoluteJointDesc revDesc;
	revDesc.actor[0] = a0;
	revDesc.actor[1] = a1;
	revDesc.setGlobalAnchor( globalAnchor );
	revDesc.setGlobalAxis( globalAxis );
	revDesc.jointFlags |= NX_JF_COLLISION_ENABLED;

	NxJoint* joint = m_pScene->createJoint( revDesc );

	NxReal maxForce  = 100000000000000000.0f;
	NxReal maxTorque = 100000000000000000.0f;
	joint->setBreakable(maxForce, maxTorque);
	return (NxRevoluteJoint*)joint;
}


/*============================================================*/
//! \brief 2�̃A�N�^�[���W���C���g����
//! \param[in]		�W���C���g����A�N�^�[
//! \param[in]		�W���C���g����A�N�^�[
//! \param[in]		�W���C���g������W
//! \param[in]		�W���C���g���鎲
/*============================================================*/
NxFixedJoint* CPhysX::CreateFixedJoint( NxActor *_a0 , NxActor *_a1 , NxVec3 _globalAnchor , NxVec3 _globalAxis )
{
	NxFixedJointDesc fixedDesc;

	fixedDesc.actor[0] = _a0;
	fixedDesc.actor[1] = _a1;
	fixedDesc.setGlobalAnchor( _globalAnchor );
	fixedDesc.setGlobalAxis( _globalAxis );

	return (NxFixedJoint*)m_pScene->createJoint( fixedDesc );
}




/*==========================================*/
//! \brief �f�o�b�O�f�[�^�̃Z�b�g
//! \return ���� = S_OK
//! \return ���s = E_FAIL
/*==========================================*/
HRESULT CPhysX::CreateDebugBase( void )
{

	float b = 30.0f;		// �}�b�g�̒���
	float c = b/2;
	float d = 1.0f;			// �{�b�N�X�̐��@
	float r = 1.0f;			// ���̔��a  
	float r1 = r*1.41421356f/2;
	float h = r/2;

    // ���_
    CUSTOMVERTEX Vertices[] =
    {
		// ���W��

		// �{�b�N�X
        {  -d , -d , -d , },	// 0
        {  -d , -d ,  d , },
        {  -d ,  d ,  d , },
        {  -d ,  d , -d , },

        {   d , -d , -d , },	// 4
        {  -d , -d , -d , },
        {  -d ,  d , -d , },
        {   d ,  d , -d , },

        {   d , -d ,  d , },	// 8
        {   d , -d , -d , },
        {   d ,  d , -d , },
        {   d ,  d ,  d , },

        {  -d , -d ,  d , },	// 12
        {   d , -d ,  d , },
        {   d ,  d ,  d , },
        {  -d ,  d ,  d , },


		// ��
        {   r ,   0 ,   0 , },	// 16
        {  r1 ,   0 ,  r1 , },
        {   0 ,   0 ,   r , },
        { -r1 ,   0 ,  r1 , },
        {  -r ,   0 ,   0 , },
        { -r1 ,   0 , -r1 , },
        {   0 ,   0 ,  -r , },
        {  r1 ,   0 , -r1 , },
        {   r ,   0 ,   0 , },

        {   r ,   0 ,   0 , },	// 25
        {  r1 ,  r1 ,   0 , },
        {   0 ,   r ,   0 , },
        { -r1 ,  r1 ,   0 , },
        {  -r ,   0 ,   0 , },	// 29
        { -r1 , -r1 ,   0 , },
        {   0 ,  -r ,   0 , },
        {  r1 , -r1 ,   0 , },
        {   r ,   0 ,   0 , },

        {   0 ,   0 ,   r , },	// 34
        {   0 ,  r1 ,  r1 , },
        {   0 ,   r ,   0 , },
        {   0 ,  r1 , -r1 , },
        {   0 ,   0 ,  -r , },	// 38
        {   0 , -r1 , -r1 , },
        {   0 ,  -r ,   0 , },
        {   0 , -r1 ,  r1 , },
        {   0 ,   0 ,   r , },


		// �J�v�Z��
        { -r ,  h ,  0 , },	// 43
        { -r , -h ,  0 , },
        {  r ,  h ,  0 , },	// 45
        {  r , -h ,  0 , },
        {  0 ,  h , -r , },	// 47
        {  0 , -h , -r , },
        {  0 ,  h ,  r , },	// 49
        {  0 , -h ,  r , },


		// �n��
		{  b , 0 ,  b , },		// 51
        {  b , 0 , -b , },
        {  c , 0 , -b , },		
		{  c , 0 ,  b , },
        {  0 , 0 ,  b , },
        {  0 , 0 , -b , },
		{ -c , 0 , -b , },
        { -c , 0 ,  b , },
        { -b , 0 ,  b , },
        { -b , 0 , -b , },
		{  b , 0 , -b , },
		{  b , 0 , -c , },
        { -b , 0 , -c , },
		{ -b , 0 ,  0 , },
		{  b , 0 ,  0 , },
		{  b , 0 ,  c , },
        { -b , 0 ,  c , },
        { -b , 0 ,  b , },
        {  b , 0 ,  b , },		// 69

    };

    // ���_�o�b�t�@�̐���
	//! \note sizeof��O�̐��l��ς���ƒ��_�o�b�t�@�̃T�C�Y���ϓ�����
	if( FAILED( DIRECTX->GetDevice()->CreateVertexBuffer( 100 * sizeof( CUSTOMVERTEX ),
                                                  0 , D3DFVF_CustomVertex ,
                                                  D3DPOOL_DEFAULT , &m_lpVB , NULL ) ) )
    {
        return E_FAIL;
    }

    // ���_�o�b�t�@�ւ̓]��
    VOID* pVertices;
    if( FAILED( m_lpVB->Lock( 0 , sizeof( Vertices ), ( void** )&pVertices , 0 ) ) )
        return E_FAIL;
    memcpy( pVertices , Vertices , sizeof( Vertices ) );
    m_lpVB->Unlock();


	return S_OK;

}


/*============================================================*/
//! \brief �A�N�^�[�����C���[�t���[���ŕ`�悷��
/*============================================================*/
void CPhysX::RenderPhysX( void )
{

	if( m_DrawPhysX )
	{

		LPDIRECT3DDEVICE9 lpDevice = DIRECTX->GetDevice();

		D3DXMATRIXA16 matWorld;
	
		D3DXMatrixIdentity( &matWorld );
		// ���[���h�}�g���b�N�X���Z�b�g
		lpDevice->SetTransform( D3DTS_WORLD , &matWorld );
		lpDevice->SetStreamSource( 0, m_lpVB, 0, sizeof( CUSTOMVERTEX ) );
		lpDevice->SetFVF( D3DFVF_CustomVertex );

		DrawGround();

		// PhysX�A�N�^�[�̕`��
		int nbActors = m_pScene->getNbActors();
		NxActor** actors = m_pScene->getActors();

		while( nbActors-- )
		{

			NxActor* actor = *actors++;
			if( !actor->userData ){ continue; }
		
			// �}�g���b�N�X�̓]�u
			CDPConverter::SetMatrix( &matWorld , actor );
			ACTOR_DATA* ActorData = (ACTOR_DATA*) actor->userData;

			char Type = ActorData->Type;
			switch( Type )
			{
			case 'B':
				DrawBox( actor , matWorld );
				break;
			case 'S':
				DrawSphere( actor , matWorld );
				break;
			case 'C':
				DrawCapsule( actor , matWorld );
				break;
			}
		}

	}

}

/*============================================================*/
//! \brief PhysX��̃{�b�N�X�����C���[�t���[���ŕ`�悷��
//!	\param[in]	*_Actor			�`�悷��A�N�^�[
//!	\param[in]	&_MatWorld		�Z�b�g����s��
/*============================================================*/
void CPhysX::DrawBox( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld )
{

	D3DXMATRIXA16 mat;
	ACTOR_DATA* mydata = (ACTOR_DATA*)_Actor->userData;
	NxVec3 v = mydata->Dimension;
	D3DXMatrixScaling( &mat , v.x , v.y , v.z );
	// ���[���h�֏�Z
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	// �o�b�t�@�֓]��
	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );

	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP,  0 , 3 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP,  4 , 3 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP,  8 , 3 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP, 12 , 3 );

}

/*============================================================*/
//! \brief PhysX��̋������C���[�t���[���ŕ`�悷��
//!	\param[in]	*_Actor			�`�悷��A�N�^�[
//!	\param[in]	&_MatWorld		�Z�b�g����s��
/*============================================================*/
void CPhysX::DrawSphere( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld )
{

	D3DXMATRIXA16 mat;
	ACTOR_DATA* mydata = (ACTOR_DATA*)_Actor->userData;
	// ���@�̎擾
	NxVec3 v = mydata->Dimension;
	float r = mydata->Radius;
	D3DXMatrixScaling( &mat , r , r , r );
	// ���[���h�֏�Z
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	// �o�b�t�@�֓]��
	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 16 , 8 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 25 , 8 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 34 , 8 );

}

/*============================================================*/
//! \brief PhysX��̃J�v�Z�������C���[�t���[���ŕ`�悷��
//!	\param[in]	*_Actor			�`�悷��A�N�^�[
//!	\param[in]	&_MatWorld		�Z�b�g����s��
/*============================================================*/
void CPhysX::DrawCapsule( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld )
{

	D3DXMATRIXA16 mat, matold;
	matold = _MatWorld;
	ACTOR_DATA* mydata = (ACTOR_DATA*) _Actor->userData;
	float r = mydata->Radius;
	float h = mydata->Height;
	D3DXMatrixScaling( &mat , r , h , r );
	// ���[���h�֏�Z
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	// �o�b�t�@�֓]��
	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	// �����̕`��
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 43 , 1 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 45 , 1 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 47 , 1 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 49 , 1 );
	//
	_MatWorld = matold;
	D3DXMatrixTranslation( &mat , 0 , h/2 , 0 );
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	D3DXMatrixScaling( &mat , r , r , r );
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 

	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	// �㕔�̔����̕`��
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 25 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 34 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 16 , 8 );

	_MatWorld = matold;
	D3DXMatrixTranslation( &mat , 0 , -h/2 , 0 );
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	D3DXMatrixScaling( &mat , r , r , r );
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 

	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	// �����̔����̕`��
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 29 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 38 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 16 , 8 );

}

/*============================================================*/
//! \brief PhysX��̒n�ʂ����C���[�t���[���ŕ`�悷��
//!	\param[in]	*_Actor			�`�悷��A�N�^�[
//!	\param[in]	&_MatWorld		�Z�b�g����s��
/*============================================================*/
void CPhysX::DrawGround( void )
{
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 51 , 18 );
}




/*==================================================================================*/
//! \brief �R�[���o�b�N���Ɏ��s����֐�
//! \param[in]	&pair		�Փ˂���2�̃A�N�^�[
//! \param[in]	events		�Փ˒���A�Փ˒��A�ՓˏI���̂ǂꂩ������
/*==================================================================================*/
void ContactCallBack::onContactNotify( NxContactPair& pair, NxU32 events )
{
   // �A�N�^�[�Ŏg�p���郆�[�U�[�f�[�^
   ACTOR_DATA* pUserData1 = nullptr;
   ACTOR_DATA* pUserData2 = nullptr;

   // �A�N�^�[���폜����Ă����珈���𔲂���
   if( pair.isDeletedActor[0] || pair.isDeletedActor[1] ){ return; }

   switch( events )
   {
   // �ՓˊJ�n�C�x���g�ʒm
   case NX_NOTIFY_ON_START_TOUCH:

      // �ՓˑΏۂ̂Q�̃A�N�^�[�̃��[�U�[�f�[�^�̃|�C���^���擾����B
      pUserData1 = (ACTOR_DATA*)pair.actors[0]->userData;
      pUserData2 = (ACTOR_DATA*)pair.actors[1]->userData;

      // ���[�U�[�f�[�^�ɃC�x���g����ݒ肷��
      pUserData1->ContactPairFlag = events;
      pUserData2->ContactPairFlag = events;

	  pUserData1->pContactActor = pUserData2->pObject->GetActor();
	  pUserData2->pContactActor = pUserData1->pObject->GetActor();

      break;

	// �Փ˒�
	case NX_NOTIFY_ON_TOUCH:

	   break;

   // �ՓˏI���C�x���g�ʒm
   case NX_NOTIFY_ON_END_TOUCH:

      // �ՓˑΏۂ̂Q�̃A�N�^�[�̃��[�U�[�f�[�^�̃|�C���^���擾����B
      pUserData1 = (ACTOR_DATA*)pair.actors[0]->userData;
      pUserData2 = (ACTOR_DATA*)pair.actors[1]->userData;

      // ���[�U�[�f�[�^�� 0 �ɏ���������
      pUserData1->ContactPairFlag = 0;
      pUserData2->ContactPairFlag = 0;

	  pUserData1->pContactActor = nullptr;
	  pUserData2->pContactActor = nullptr;

      break;
   }
}