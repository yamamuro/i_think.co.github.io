 #include "stdafx.hpp"

#include "Direct3D\CDirect3D.hpp"
#include "Cloth/NxCooking.h"
#include "Cloth/Stream.h"
#include "CPhysX.hpp"


/*=======================================*/
//! CPhysXのポインタの定義
/*=======================================*/
CPhysX* CPhysX::m_pInstance	= nullptr;

/*============================================*/
//! コリジョングループのベクタの定義
/*============================================*/
std::vector< NxActor* > CPhysX::m_Data;


/*=======================================*/
//! シーンの更新スピード
/*=======================================*/
const float CPhysX::SIMULATE_STEP = 1.0f / FRAME_RATE;


/*===============================================*/
//! \brief デフォルトコンストラクタ
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
//! \brief 初期化
//! \return 成功 = true 
//! \return 失敗 = fasle
/*==================================*/
bool CPhysX::Initialize( void )
{

	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	m_pPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION , NULL , NULL , desc , &errorCode );
	if( m_pPhysicsSDK == nullptr ){
		ErrorMessage( "\nPhysXのSDKの初期化に失敗しました\n\n" );
		return false;
	}

	// めり込み量
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

	// シーンの生成
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3( 0.0f , -98.1f/*-9.81f*/ , 0.0f );			// 重力加速度
	
	// ハードウェアの有無を確認
	NxHWVersion HWVersion = m_pPhysicsSDK->getHWVersion();
	// 対応しているのでフラグをたてる
	if( HWVersion != NX_HW_VERSION_NONE )
	{
		sceneDesc.simType = NX_SIMULATION_HW;
	}

	m_pScene = m_pPhysicsSDK->createScene( sceneDesc );
	if( m_pScene == nullptr ){
		ErrorMessage( "\nPhysXのシーンの作成に失敗しました\n\n" );
		return false;
	}

	// NxSceneにイベント追加
	m_ContactCallBack = new ContactCallBack();
	m_pScene->setUserContactReport( m_ContactCallBack );

	m_JointNotify = new MyJointNotify();
	m_pScene->setUserNotify( m_JointNotify );


	
	// デフォルトのマテリアルを設定
	NxMaterial *defaultMaterial = m_pScene->getMaterialFromIndex( 0 );
	defaultMaterial->setRestitution( 0.0f );			// 跳ね返り係数の指定
	defaultMaterial->setStaticFriction( 8.9f );			// 静止摩擦係数の指定
	defaultMaterial->setDynamicFriction( 9.9f );		// 動摩擦係数の指定
	
	CreateDebugBase();

	InitActorCollisionGroups(); 
	
	DEBUG->PrintConsole( "PhysXの初期化に成功しました...\n" , CDebug::ConPreside );

	return true;

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CPhysX::~CPhysX( void )
{
	Finalize();
}


/*==================================*/
//! \brief 終了処理
/*==================================*/
void CPhysX::Finalize( void )
{
	
	AllDeleteActor();

	if( m_pPhysicsSDK != nullptr )
	{
		if( m_pScene != nullptr )
		{

			// マテリアルの削除
			int MatNum = m_pScene->getNbMaterials();
			for( int i=1 ; i<MatNum ; i++ )	// 0番目はデフォルトのマテリアルなので削除しない
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
//! \brirf アクターの動きをシーン上でシミュレーションする
/*=================================================================*/
void CPhysX::Simulation( void )
{

	if( m_pScene == nullptr ){ return; }

	// PhysX 時間ステップ設定
	m_pScene->simulate( SIMULATE_STEP );

	m_pScene->flushStream();
	m_pScene->fetchResults( NX_RIGID_BODY_FINISHED , true );

}


/*=====================================================================*/
//! \brief コリジョングループのセット
//! \param[in]	*_Actor		セットするアクター
//! \param[in]	_Group		セットするグループID
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
//! \brief コリジョングループの初期化
/*==========================================================*/
void CPhysX::InitActorCollisionGroups( void )
{
	// コリジョンを監視する
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
//! \brief ユーザレポートの初期化
/*====================================================*/
void CPhysX::InitContactReportsPerPair( NxActor *_Actor )
{
	
	if( m_Data.size() != 0 )
	{
		for( auto &x : m_Data )
		{
			// 衝突時の判定を返す
			// 当たった瞬間　、　当たっている　、　離れた　の3タイプ
			m_pScene->setActorPairFlags( *_Actor , *x , NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );
		}
	}
	m_Data.push_back( _Actor );
	
}


/*===============================================*/
//! \brief アクターを削除する
//! \param[in]	*_pActor	削除するアクター
//! \note 未実装
/*===============================================*/
void DeleteActor( NxActor* _pActor )
{



}

/*==============================================*/
//! \brief 存在しているアクターを全て削除
/*==============================================*/
void CPhysX::AllDeleteActor( void )
{

	int actorCount;
	NxActor **ppActors;

	if( m_pPhysicsSDK == nullptr ){ return; }

	actorCount = m_pScene->getNbActors();
	ppActors   = m_pScene->getActors();

	// アクターが1つでも存在するなら全て削除する
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
簡易のボックス作成
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
//! 簡易のカプセル
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
//! \brief アクターの追加
//! \param[in]	*_pActorData			作成するアクターのデータ
//! \return 成功 = 追加したアクター
//! \return 失敗 = nullptr
/*=======================================================================*/
NxActor* CPhysX::CreateActor( ACTOR_DATA *_pActorData )
{

	if( m_pScene == nullptr ){ return nullptr; }	

	ACTOR_DATA* data = new ACTOR_DATA;
	*data = *_pActorData;

	// デスクリプタ	
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();							// 初期化
	bodyDesc.angularDamping	= data->AngularDamping;		// 回転による衰退係数


	NxActorDesc actorDesc;

	if( false ){}
	else if( data->Type == 'B' )			// ボックスの作成
	{

		NxBoxShapeDesc boxDesc;
		boxDesc.setToDefault();		// 初期化
		boxDesc.dimensions = data->Dimension;
		boxDesc.skinWidth = data->SkinWidth;
		boxDesc.localPose.t     = data->LocalPosition;							// ローカル座標
		boxDesc.localPose.M     = data->LocalRotation;							// ローカルな姿勢

		// マテリアルの追加
		if( data->MatDesc.isInit )
		{
			m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			boxDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}
		
		actorDesc.setToDefault();	// 初期化
		// 動的であればbodyDesc　静的であればNULLを格納する
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// 動的データを格納
		actorDesc.density		= data->Density;								// 質量密度（kg/m^3）
		actorDesc.globalPose.t  = data->Position;								// シーン上での位置(グローバル)
		actorDesc.globalPose.M  = data->Rotation;								// シーン上での回転角度
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &boxDesc );									// ボックスをアクターに登録
		
	}
	else if( data->Type == 'S' )			// スフィアの作成
	{
	
		NxSphereShapeDesc sphereDesc;
		sphereDesc.setToDefault();		// 初期化
		sphereDesc.radius = data->Radius;
		sphereDesc.skinWidth = data->SkinWidth;
		sphereDesc.localPose.t  = data->LocalPosition;							// ローカル座標
		sphereDesc.localPose.M  = data->LocalRotation;							// ローカルな姿勢

		// マテリアルの追加
		if( data->MatDesc.isInit )
		{
			m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			sphereDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}

		actorDesc.setToDefault();	// 初期化
		// 動的であればbodyDesc　静的であればNULLを格納する
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// 動的データを格納
		actorDesc.density		= data->Density;								// 質量密度（kg/m^3）
		actorDesc.globalPose.t  = data->Position;								// シーン上での位置(グローバル)
		actorDesc.globalPose.M  = data->Rotation;								// シーン上での回転角度
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &sphereDesc );								// 球をアクターに登録

	}
	else if( data->Type == 'C' )			// カプセルの作成
	{

		NxCapsuleShapeDesc capsuleDesc;
		capsuleDesc.setToDefault();		// 初期化
		capsuleDesc.radius = data->Radius;
		capsuleDesc.height = data->Height;
		capsuleDesc.skinWidth = data->SkinWidth;
		capsuleDesc.localPose.t = data->LocalPosition;							// ローカル座標
		capsuleDesc.localPose.M = data->LocalRotation;							// ローカルな姿勢
		
		// マテリアルの追加
		if( data->MatDesc.isInit )
		{
			m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			capsuleDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}

		actorDesc.setToDefault();	// 初期化
		// 動的であればbodyDesc　静的であればNULLを格納する
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// 動的データを格納
		actorDesc.density		= data->Density;								// 質量密度（kg/m^3）
		actorDesc.globalPose.t  = data->Position;								// シーン上での位置(グローバル)
		actorDesc.globalPose.M  = data->Rotation;								// シーン上での回転角度
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &capsuleDesc );								// カプセルをアクターに登録

	}
	else if( data->Type == 'P' )
	{

		NxCapsuleShapeDesc capsuleDesc;
		capsuleDesc.setToDefault();		// 初期化
		capsuleDesc.radius = data->Radius;
		capsuleDesc.height = data->Height;
		capsuleDesc.skinWidth = 0.0f;
		capsuleDesc.localPose.t = data->LocalPosition;							// ローカル座標
		capsuleDesc.localPose.M = data->LocalRotation;							// ローカルな姿勢

		// マテリアルの追加
		if( data->MatDesc.isInit )
		{
			NxMaterial *Mat;
			Mat = m_pScene->createMaterial( data->MatDesc.MaterialDesc );
			capsuleDesc.materialIndex = m_pScene->getHighestMaterialIndex();
		}

		actorDesc.setToDefault();	// 初期化
		// 動的であればbodyDesc　静的であればNULLを格納する
		actorDesc.body			= ( data->FixFlag )?( NULL ):( &bodyDesc );		// 動的データを格納
		actorDesc.flags			|= NX_BF_KINEMATIC;
		actorDesc.density		= data->Density;								// 質量密度（kg/m^3）
		actorDesc.globalPose.t  = data->Position;								// シーン上での位置(グローバル)
		actorDesc.globalPose.M  = data->Rotation;								// シーン上での回転角度
		actorDesc.userData      = data;
		actorDesc.shapes.pushBack( &capsuleDesc );								// カプセルをアクターに登録

	}
	else if( data->Type == 'G' )
	{
		
		// 地面のセット
		NxPlaneShapeDesc PlaneDesc;
		PlaneDesc.setToDefault();
		actorDesc.body = NULL;
		actorDesc.userData = data;
		actorDesc.shapes.pushBack( &PlaneDesc );
		
	}
	else
	{
		// 確保したものの、結局使わなかったACTOR_DATAを解放
		SafeDelete( data );
		return nullptr;
	}

	// ここでアクターをシーンに登録
	NxActor *Actor = m_pScene->createActor( actorDesc );
	// 衝突判定を監視する
	InitContactReportsPerPair( Actor );
	
	return Actor;
	
}


/*=====================================================================*/
//! \brief ロープを作成する
//! \param[out]		**_ropeSegs		ロープのアクターのポインタ
//! \param[out]		**_ropeLinks	ロープのジョイント部分のポインタ
//! \param[in]		_Pos			配置する座標
//! \param[in]		_segHeight		1つのアクターの高さ
//! \param[in]		_segRadius		1つのアクターの半径
//! \param[in]		_nbSegments		いくつのアクターを繋ぐか
//! \param[in]		_density		1つのアクターの密度
//! \param[in]		_MaterialIndex	マテリアルのインデックス番号
/*=====================================================================*/
void CPhysX::CreateRope( NxActor **_ropeSegs , NxSphericalJoint **_ropeLinks ,  const NxVec3& _Pos , const NxReal _segHeight , const NxReal _segRadius , const int _nbSegments , const NxReal _density , int _MaterialIndex )
{

	// ロープのパーツを作っていく
	for( int i=0 ; i<_nbSegments ; i++ )
	{
		_ropeSegs[i] = CreateCapsule( _Pos + NxVec3( 0.0f , i * ( 2 * _segRadius + _segHeight ) , 0.0f ) , _segHeight , _segRadius , _density , i == _nbSegments-1 ? FALSE : TRUE , _MaterialIndex );
	}

	// 繋げていく
	for( int i=0 ; i<_nbSegments-1 ; i++ )
	{
		_ropeLinks[i] = CreateRopeSphericalJoint( _ropeSegs[i] , _ropeSegs[i+1] , _Pos + NxVec3( 0.0f , ( i + 1 ) * ( 2 * _segRadius + _segHeight ) , 0.0f ) , NxVec3( 0.0f , 1.0f , 0.0f ) ); 
	}

}


/*================================================================*/
//! \brief アクターをジョイントする
//! \param[in]	*_a0			ジョイントするアクター
//! \param[in]	*_a1			ジョイントするアクター
//! \param[in]	&_globalAnchor	ジョイントする座標
//! \param[in]	&_globalAxis	ジョイントする軸
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
	ts.spring = 10.0f;				// ばね係数
	ts.damper = 100000000.0f;		// ダンパー係数　元(直線状)に戻ろうとする力
	ts.targetValue = 111000.0f;		// ?
	sphericalDesc.twistSpring = ts;

	sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
	sphericalDesc.swingLimit.value = 3.0f;//0.3 * NxPi		// 振り回し運動の可動範囲

	sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
	NxSpringDesc ss;
	ss.spring = 10.0f;				// ばね係数
	ss.damper = 100000000.0f;		// ダンパー係数　元(直線状)に戻ろうとする力
	ss.targetValue = 111000.0f;		// ?
	sphericalDesc.swingSpring = ss;

	return (NxSphericalJoint*)m_pScene->createJoint( sphericalDesc );

}


/*======================================================================*/
//! \brief メッシュからアクターを作成する
//! \param[in]	_lpMesh		メッシュデータ
//! \return 成功 = 追加したアクター
//! \return 失敗 = nullptr
/*======================================================================*/
NxActor* CPhysX::CreateConvexActor( LPD3DXMESH _lpMesh )
{

	NxTriangleMeshDesc meshDesc;
	NxU32 flg = 0;
	LPBYTE pIndex = nullptr;
	LPBYTE pBits = nullptr;
	NxVec3* pVec = nullptr;
	 
	// メッシュから頂点データを作成
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

	// 頂点数を調べる
	meshDesc.numVertices      = lpClone->GetNumVertices();
	// 1つの頂点のサイズ
	meshDesc.pointStrideBytes = lpClone->GetNumBytesPerVertex();
	// 三角ポリゴン数
	meshDesc.numTriangles     = lpClone->GetNumFaces();

	// 三角ポリゴン１個分のインデックスのサイズ
	if( flg & NX_CF_16_BIT_INDICES )
	{
		meshDesc.triangleStrideBytes = 3 * sizeof( WORD );
	}
	else
	{
		meshDesc.triangleStrideBytes = 3 * sizeof( DWORD );
	}

	// 頂点を格納
	pVec = new NxVec3[meshDesc.numVertices];

	
	// 頂点バッファから頂点座標を取得
	lpClone->LockVertexBuffer( 0 , (void**)&pBits );					// ロック開始
	CopyMemory( pVec, pBits, meshDesc.pointStrideBytes * meshDesc.numVertices );
	meshDesc.points = pVec;
	lpClone->UnlockVertexBuffer();										// 処理が終わったのでアンロック

	pIndex = new BYTE[ meshDesc.triangleStrideBytes * meshDesc.numTriangles ];

	//インデックスバッファから頂点座標を取得
	lpClone->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pBits );		// ロック開始
	CopyMemory( pIndex, pBits, meshDesc.triangleStrideBytes * meshDesc.numTriangles );
	meshDesc.triangles = pIndex;
	lpClone->UnlockIndexBuffer();										// 処理が終わったのでアンロック

	// コンベックスアクターの作成開始
	NxInitCooking();

	MemoryWriteBuffer Buff;
	// シリアル化
	bool State = NxCookTriangleMesh( meshDesc, Buff );

	SafeDeleteArray( pVec , "ConvexMeshのVertex" );
	SafeDeleteArray( pIndex , "ConvexMeshのIndex" );

	if( State )
	{

		NxTriangleMesh *pMesh = nullptr;
		
		pMesh = m_pPhysicsSDK->createTriangleMesh( MemoryReadBuffer( Buff.data ) );
		if( pMesh == nullptr ){ return nullptr; }

		NxTriangleMeshShapeDesc meshShapeDesc;
		meshShapeDesc.meshData = pMesh;
		
		NxActorDesc actorDesc;
		assert( meshShapeDesc.isValid() );

		// アクターのマテリアルを設定する
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

		// コンベックスアクターの作成終了
		NxCloseCooking();

		return Actor;

	}

	// コンベックスアクターの作成終了
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
//! \brief 2つのアクターをジョイントする
//! \param[in]		ジョイントするアクター
//! \param[in]		ジョイントするアクター
//! \param[in]		ジョイントする座標
//! \param[in]		ジョイントする軸
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
//! \brief デバッグデータのセット
//! \return 成功 = S_OK
//! \return 失敗 = E_FAIL
/*==========================================*/
HRESULT CPhysX::CreateDebugBase( void )
{

	float b = 30.0f;		// マットの長さ
	float c = b/2;
	float d = 1.0f;			// ボックスの寸法
	float r = 1.0f;			// 球の半径  
	float r1 = r*1.41421356f/2;
	float h = r/2;

    // 頂点
    CUSTOMVERTEX Vertices[] =
    {
		// 座標軸

		// ボックス
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


		// 球
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


		// カプセル
        { -r ,  h ,  0 , },	// 43
        { -r , -h ,  0 , },
        {  r ,  h ,  0 , },	// 45
        {  r , -h ,  0 , },
        {  0 ,  h , -r , },	// 47
        {  0 , -h , -r , },
        {  0 ,  h ,  r , },	// 49
        {  0 , -h ,  r , },


		// 地面
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

    // 頂点バッファの生成
	//! \note sizeof手前の数値を変えると頂点バッファのサイズが変動する
	if( FAILED( DIRECTX->GetDevice()->CreateVertexBuffer( 100 * sizeof( CUSTOMVERTEX ),
                                                  0 , D3DFVF_CustomVertex ,
                                                  D3DPOOL_DEFAULT , &m_lpVB , NULL ) ) )
    {
        return E_FAIL;
    }

    // 頂点バッファへの転送
    VOID* pVertices;
    if( FAILED( m_lpVB->Lock( 0 , sizeof( Vertices ), ( void** )&pVertices , 0 ) ) )
        return E_FAIL;
    memcpy( pVertices , Vertices , sizeof( Vertices ) );
    m_lpVB->Unlock();


	return S_OK;

}


/*============================================================*/
//! \brief アクターをワイヤーフレームで描画する
/*============================================================*/
void CPhysX::RenderPhysX( void )
{

	if( m_DrawPhysX )
	{

		LPDIRECT3DDEVICE9 lpDevice = DIRECTX->GetDevice();

		D3DXMATRIXA16 matWorld;
	
		D3DXMatrixIdentity( &matWorld );
		// ワールドマトリックスをセット
		lpDevice->SetTransform( D3DTS_WORLD , &matWorld );
		lpDevice->SetStreamSource( 0, m_lpVB, 0, sizeof( CUSTOMVERTEX ) );
		lpDevice->SetFVF( D3DFVF_CustomVertex );

		DrawGround();

		// PhysXアクターの描画
		int nbActors = m_pScene->getNbActors();
		NxActor** actors = m_pScene->getActors();

		while( nbActors-- )
		{

			NxActor* actor = *actors++;
			if( !actor->userData ){ continue; }
		
			// マトリックスの転置
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
//! \brief PhysX上のボックスをワイヤーフレームで描画する
//!	\param[in]	*_Actor			描画するアクター
//!	\param[in]	&_MatWorld		セットする行列
/*============================================================*/
void CPhysX::DrawBox( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld )
{

	D3DXMATRIXA16 mat;
	ACTOR_DATA* mydata = (ACTOR_DATA*)_Actor->userData;
	NxVec3 v = mydata->Dimension;
	D3DXMatrixScaling( &mat , v.x , v.y , v.z );
	// ワールドへ乗算
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	// バッファへ転送
	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );

	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP,  0 , 3 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP,  4 , 3 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP,  8 , 3 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP, 12 , 3 );

}

/*============================================================*/
//! \brief PhysX上の球をワイヤーフレームで描画する
//!	\param[in]	*_Actor			描画するアクター
//!	\param[in]	&_MatWorld		セットする行列
/*============================================================*/
void CPhysX::DrawSphere( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld )
{

	D3DXMATRIXA16 mat;
	ACTOR_DATA* mydata = (ACTOR_DATA*)_Actor->userData;
	// 寸法の取得
	NxVec3 v = mydata->Dimension;
	float r = mydata->Radius;
	D3DXMatrixScaling( &mat , r , r , r );
	// ワールドへ乗算
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	// バッファへ転送
	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 16 , 8 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 25 , 8 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 34 , 8 );

}

/*============================================================*/
//! \brief PhysX上のカプセルをワイヤーフレームで描画する
//!	\param[in]	*_Actor			描画するアクター
//!	\param[in]	&_MatWorld		セットする行列
/*============================================================*/
void CPhysX::DrawCapsule( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld )
{

	D3DXMATRIXA16 mat, matold;
	matold = _MatWorld;
	ACTOR_DATA* mydata = (ACTOR_DATA*) _Actor->userData;
	float r = mydata->Radius;
	float h = mydata->Height;
	D3DXMatrixScaling( &mat , r , h , r );
	// ワールドへ乗算
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	// バッファへ転送
	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	// 胴部の描画
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
	// 上部の半球の描画
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 25 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 34 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 16 , 8 );

	_MatWorld = matold;
	D3DXMatrixTranslation( &mat , 0 , -h/2 , 0 );
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 
	D3DXMatrixScaling( &mat , r , r , r );
	D3DXMatrixMultiply( &_MatWorld , &mat , &_MatWorld ); 

	DIRECTX->GetDevice()->SetTransform( D3DTS_WORLD , &_MatWorld );
	// 下部の半球の描画
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 29 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 38 , 4 );
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 16 , 8 );

}

/*============================================================*/
//! \brief PhysX上の地面をワイヤーフレームで描画する
//!	\param[in]	*_Actor			描画するアクター
//!	\param[in]	&_MatWorld		セットする行列
/*============================================================*/
void CPhysX::DrawGround( void )
{
	DIRECTX->GetDevice()->DrawPrimitive( D3DPT_LINESTRIP , 51 , 18 );
}




/*==================================================================================*/
//! \brief コールバック時に実行する関数
//! \param[in]	&pair		衝突した2つのアクター
//! \param[in]	events		衝突直後、衝突中、衝突終了のどれかが入る
/*==================================================================================*/
void ContactCallBack::onContactNotify( NxContactPair& pair, NxU32 events )
{
   // アクターで使用するユーザーデータ
   ACTOR_DATA* pUserData1 = nullptr;
   ACTOR_DATA* pUserData2 = nullptr;

   // アクターが削除されていたら処理を抜ける
   if( pair.isDeletedActor[0] || pair.isDeletedActor[1] ){ return; }

   switch( events )
   {
   // 衝突開始イベント通知
   case NX_NOTIFY_ON_START_TOUCH:

      // 衝突対象の２つのアクターのユーザーデータのポインタを取得する。
      pUserData1 = (ACTOR_DATA*)pair.actors[0]->userData;
      pUserData2 = (ACTOR_DATA*)pair.actors[1]->userData;

      // ユーザーデータにイベント情報を設定する
      pUserData1->ContactPairFlag = events;
      pUserData2->ContactPairFlag = events;

	  pUserData1->pContactActor = pUserData2->pObject->GetActor();
	  pUserData2->pContactActor = pUserData1->pObject->GetActor();

      break;

	// 衝突中
	case NX_NOTIFY_ON_TOUCH:

	   break;

   // 衝突終了イベント通知
   case NX_NOTIFY_ON_END_TOUCH:

      // 衝突対象の２つのアクターのユーザーデータのポインタを取得する。
      pUserData1 = (ACTOR_DATA*)pair.actors[0]->userData;
      pUserData2 = (ACTOR_DATA*)pair.actors[1]->userData;

      // ユーザーデータを 0 に初期化する
      pUserData1->ContactPairFlag = 0;
      pUserData2->ContactPairFlag = 0;

	  pUserData1->pContactActor = nullptr;
	  pUserData2->pContactActor = nullptr;

      break;
   }
}