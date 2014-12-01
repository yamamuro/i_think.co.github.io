#include "stdafx.hpp"

#include "../GimmickBase/CGimmickBase.hpp"
#include "CLiftY.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CLiftY::CLiftY( void )
	: FreezeCount( FreezeTime )
	, m_MoveFlag( MoveUp )
{

	m_FilePass += "Object/Gimmick/Lift/Lift.x";
	// メッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CLiftY::DiscreteInit( void )
{

	// アクターの作成
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();									// 初期化

	// Y方向にのみ動き、回転も禁止する
	bodyDesc.flags |= NX_BF_FROZEN_POS_X;
	bodyDesc.flags |= NX_BF_FROZEN_POS_Z;
	bodyDesc.flags |= NX_BF_FROZEN_ROT;
	

	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();										// 初期化
	boxDesc.dimensions = NxVec3( 10.0f , 2.0f , 10.0f );
	
	NxActorDesc actorDesc;
	actorDesc.setToDefault();									// 初期化
	actorDesc.body			= &bodyDesc;						// 動的データを格納
	actorDesc.density		= 1000000000000000.0f;				// 質量密度（kg/m^3）
	NxVec3 LocalPos( 0.0f , -2.0f , 0.0f );
	actorDesc.globalPose.t  = LocalPos + m_ActorData.Position;	// シーン上での位置(グローバル)
	
	actorDesc.shapes.pushBack( &boxDesc );						// ボックスをシーンに登録

	m_Actor  = m_pPhysX->GetScene()->createActor( actorDesc );




	// ストッパーのアクター
	boxDesc.setToDefault();										// 初期化
	boxDesc.dimensions = NxVec3( 10.0f , 0.5f , 10.0f );

	actorDesc.setToDefault();									// 初期化
	actorDesc.body			= nullptr;							// 静的アクター
	actorDesc.globalPose.t  = NxVec3( 20000.0f, 0.0f , 0.0f );	// 画面から離れた場所に設置
	actorDesc.shapes.pushBack( &boxDesc );						// ボックスをシーンに登録

	m_Stopper = m_pPhysX->GetScene()->createActor( actorDesc );

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLiftY::~CLiftY( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CLiftY::Exec( void )
{

	float Dist = ( m_MoveFlag == MoveUp ? 20.0f  : 2.0f );
	static const float Speed = 20.0f;


	if( FreezeCount < FreezeTime ){ FreezeCount++; return; }
	else
	{ 
		// ストッパーを画面外に配置
		m_Stopper->setGlobalPosition( NxVec3( 20000.0f, 0.0f , 0.0f ) );
	}

	// 進行方向にレイを飛ばす
	if ( MoveCheck() < Dist )
	{
		// カウンタ初期化
		FreezeCount = 0;

		if( m_MoveFlag == MoveUp )
			// リフトの真下に設置し動作停止中の落下を防ぐ
			m_Stopper->setGlobalPosition( m_Actor->getGlobalPosition() + NxVec3( 0.0f, -m_ActorData.Dimension.y/2 , 0.0f ) );

		// 進行方向を反転
		m_MoveFlag = !m_MoveFlag;
	}
	

	if( m_MoveFlag == MoveUp )
		m_Actor->setLinearVelocity( NxVec3( 0.0f ,  Speed , 0.0f ) );
	else
		m_Actor->setLinearVelocity( NxVec3( 0.0f , -Speed , 0.0f ) );
	

	// 座標更新
	NxVec3 Pos = m_Actor->getGlobalPosition();
	m_vPos = CDPConverter::toDirectX( Pos );

}


/*=================================================*/
//! \brief 進行方向にレイを飛ばし、距離を返す
/*=================================================*/
float CLiftY::MoveCheck( void )
{

	// レイを飛ばす場所
	NxVec3 orig = m_vPos;

	// レイの飛ばす方向
	NxVec3 dir = NxVec3( 0.0f , m_MoveFlag ? -1.0f : 1.0f , 0.0f );

	NxRay ray( orig , dir );

	NxRaycastHit hit;
	NxReal dist;

	// レイを飛ばす
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