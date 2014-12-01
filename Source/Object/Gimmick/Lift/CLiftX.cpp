#include "stdafx.hpp"

#include "../Application/CApplication.hpp"
#include "../GimmickBase/CGimmickBase.hpp"
#include "CLiftX.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CLiftX::CLiftX( void )
	: m_MoveFlag( MoveLeft )
{

	m_FilePass += "Object/Gimmick/Lift/Lift.x";
	// メッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CLiftX::DiscreteInit( void )
{

	// アクターの作成
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();									// 初期化

	// X方向にのみ動き、回転も禁止する
	bodyDesc.flags |= NX_BF_FROZEN_POS_Y;
	bodyDesc.flags |= NX_BF_FROZEN_POS_Z;
	bodyDesc.flags |= NX_BF_FROZEN_ROT;
	
	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();										// 初期化
	boxDesc.dimensions = NxVec3( 10.0f , 2.3f , 10.0f );
	
	NxActorDesc actorDesc;
	actorDesc.setToDefault();									// 初期化
	actorDesc.body			= &bodyDesc;						// 動的データを格納
	actorDesc.density		= 1000000000000000.0f;					// 質量密度（kg/m^3）
	NxVec3 LocalPos( 0.0f , -2.0f , 0.0f );
	actorDesc.globalPose.t  = LocalPos + m_ActorData.Position;	// シーン上での位置(グローバル)
	
	actorDesc.shapes.pushBack( &boxDesc );						// ボックスをシーンに登録

	m_Actor  = m_pPhysX->GetScene()->createActor( actorDesc );

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLiftX::~CLiftX( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CLiftX::Exec( void )
{

	// デバッグ
	if( KEYBOARD->GetState( CKeyboard::KEY_O ) == CKeyboard::KST_PUSH )
	{
		OBJECT_MANAGER->AddObject( "Box" , D3DXVECTOR3( m_vPos.x , m_vPos.y + 2.0f , m_vPos.z ) );
	}

	static const float Dist = 12.0f;
	static const float Speed = 10.0f;

	// 進行方向にレイを飛ばす
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
//! \brief 進行方向にレイを飛ばし、距離を返す
/*=================================================*/
float CLiftX::MoveCheck( void )
{

	// レイを飛ばす場所
	NxVec3 orig = m_vPos;

	// レイの飛ばす方向
	NxVec3 dir = NxVec3( m_MoveFlag == MoveLeft ? -1.0f : 1.0f , 0.0f , 0.0f );
	NxRay ray( orig , dir );

	NxRaycastHit hit;
	NxReal dist;

	// レイを飛ばす
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
