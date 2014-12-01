#include "stdafx.hpp"

#include "Wrapper/Vector3/CVector.hpp"
#include "../Scene/SceneManager/CSceneManager.hpp"
#include "../../Object/Box/CBox.hpp"

#include "../LaserGun/LaserBase/CLaserBase.hpp"
#include "../LaserGun/LaserManager/CLaserManager.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "Direct3D/CLightManager.hpp"
#include "CPlayer.hpp"


#include "../FrameWork/PhysX/Cloth/BmpLoader.h"
#include "../FrameWork/PhysX/Cloth/MediaPath.h"
#include "../FrameWork/PhysX/Cloth/MyCloth.h"
#include "../FrameWork/PhysX/Cloth/wavefront.h"


NxVec3 BoxPos = NxVec3(0.0f, 12.0f, 0.0f);

// Array of cloth objects
NxArray<MyCloth*> gCloths;

/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CPlayer::CPlayer( void ) 
	: m_pd3d( CAPP->GetCDirect3D() )
	, m_pKeyboard( CAPP->GetCKeyboard() )
	, m_pMouse( CAPP->GetCMouse() )
	, m_MoveVec( 500.0f )
	//, m_pBGM( nullptr )
	, m_pLaserGun( nullptr )
{

	//m_pBGM = SOUND_MANAGER->Load( "walk.wav" , 1 );

	// マウスを非表示にする
	m_pMouse->SetCursor( FALSE );

	// ウィンドウの中心座標
	m_WindowCenterPos = POINT();
	// 最初に向いている方向
	m_vRot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	// アクターデータ
	m_ActorData.Type = 'P';
	m_ActorData.ObjectName = CObjectManager::OBJ_PLAYER;
	m_ActorData.Radius = 2.7f;						// 太さ
	m_ActorData.Height = 9.0f;						// 高さ
	m_ActorData.FixFlag = false;					// 静的アクターか
	m_ActorData.AngularDamping = 50.0f;				// 摩擦
	m_ActorData.Density = 50000000.0f;				// 密度

	m_TotalHeight = m_ActorData.Radius*2 + m_ActorData.Height;

	//m_ActorData.MatDesc.MaterialDesc.frictionCombineMode = NX_CM_MIN;
	m_ActorData.MatDesc.MaterialDesc.restitution      = 0.0f;
	m_ActorData.MatDesc.MaterialDesc.restitutionCombineMode = NX_CM_MIN;
	m_ActorData.MatDesc.MaterialDesc.staticFriction   = 9.5f;
	m_ActorData.MatDesc.MaterialDesc.dynamicFriction  = 8.5f;
	m_ActorData.MatDesc.isInit = true;

}


/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CPlayer::DiscreteInit( void )
{


	// ウィンドウの中心を格納
	m_WindowCenterPos.x = CAPP->GetWindowSize().cx / 2;
	m_WindowCenterPos.y = CAPP->GetWindowSize().cy / 2;
	m_pMouse->SetPos( &m_WindowCenterPos );

	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	NxMat33 Rot;
	Rot.rotX( m_vRot.x );
	Rot.rotY( m_vRot.y );
	Rot.rotZ( m_vRot.z );
	m_ActorData.Rotation = Rot;


	// レーザー銃のインスタンス化
	m_pLaserGun = new CLaserManager( this );

	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_PLAYER );
	m_Actor->raiseBodyFlag( NX_BF_FROZEN_ROT );
	//m_Actor->raiseActorFlag
	
	// AGL、そのパーティクルは一番最後に描画する

	
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CPlayer::~CPlayer( void )
{
	// マウスを表示する
	m_pMouse->SetCursor( TRUE );

	SafeDelete( VARIABLE_DATA( m_pLaserGun ) );
}

//! \brief 更新
void CPlayer::Exec( void )
{

	// マウスに関連した処理
	MouseExec();
	
	// 移動
	Move();

	// レーザー銃の更新
	m_pLaserGun->Exec();

#ifdef _DEBUG
	if( m_pKeyboard->GetState( CKeyboard::KEY_I ) == CKeyboard::KST_HOLD )
	{
		OBJECT_MANAGER->AddObject( "Box" , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	}
#endif

}


//! \brief マウスに関連した処理
void CPlayer::MouseExec( void )
{

	// ウィンドウの中心を格納
	m_WindowCenterPos.x = CAPP->GetWindowSize().cx / 2;
	m_WindowCenterPos.y = CAPP->GetWindowSize().cy / 2;
	
	// Shiftを押している時はマウスを表示する
	if( m_pKeyboard->GetState( CKeyboard::KEY_SHIFT ) == CKeyboard::KST_PUSH )
	{
		// マウスを表示する
		m_pMouse->SetCursor( TRUE );
	}
	else if( m_pKeyboard->GetState( CKeyboard::KEY_SHIFT ) == CKeyboard::KST_PULL )
	{
		// マウスを非表示にする
		m_pMouse->SetCursor( FALSE );
	}
	else if( m_pKeyboard->GetState( CKeyboard::KEY_SHIFT ) == CKeyboard::KST_FREE )
	{
		// カメラの方向を更新
		AddRot( D3DXVECTOR3( ( m_pMouse->GetPos().y - m_WindowCenterPos.y ) / 4.7f , ( m_pMouse->GetPos().x - m_WindowCenterPos.x ) / 4.7f , 0.0f ) );
		D3DXVECTOR3 CamAng = GetRot();
		if( CamAng.x >  85 ){ CamAng.x  = 85; }// 下方向
		if( CamAng.x < -85 ){ CamAng.x = -85; }// 上方向
		m_vRot = D3DXVECTOR3( CamAng );
		DIRECTX->SetZoom( -2.0f );
		DIRECTX->SetCamRot( CamAng );
		m_pMouse->SetPos( &m_WindowCenterPos );
	}

}

// \brief 移動
void CPlayer::Move( void )
{

	// デバッグ　浮遊
	if( m_pKeyboard->GetState( CKeyboard::KEY_Z ) == CKeyboard::KST_HOLD ) 
	{
		m_Actor->setLinearVelocity( NxVec3( 0.0f, 18.0f , 0.0f ) );
	}

	
	D3DXVECTOR3 vVec( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vTmp;
	CMatrix mRot;
	
	mRot.SetRotate( GetRot() );

	//! \note 本来であれば W = flont , A = Left...となるが
	//! \note setAngularVelocityを使用するのでそれを考慮する
	if( m_pKeyboard->GetState( CKeyboard::KEY_A ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_S ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( -1.0f , 0.0f , 0.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_W ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_D ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( 0.0f , 0.0f , -1.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}

	// ジャンプ処理
	static NxVec3 JumpVec = NxVec3( 0.0f, 0.0f , 0.0f );
	static int Count = 0;
	static const int MaxCount = 10;
	if( KEYBOARD->GetState( CKeyboard::KEY_SPACE ) == CKeyboard::KST_PUSH && CheckFooting() )
	{
		Count = 0;
		JumpVec = NxVec3( 0.0f, 25.0f , 0.0f );
	}
	// 押し続けるとより高く飛ぶ
	if( KEYBOARD->GetState( CKeyboard::KEY_SPACE ) == CKeyboard::KST_HOLD )
	{
		if( Count < MaxCount )
		{
			JumpVec += NxVec3( 0.0f , 5.7f , 0.0f );
		}
		Count++;
	}else
	{
		Count--;
		if( Count < 0 ){ Count = 0; }
	}


	 
	// 移動方向を正規化
	D3DXVec3Normalize( &vVec , &vVec );

	// 進行方向へ力を加える
	// 移動
	D3DXVECTOR3 MoveVec( vVec * m_MoveVec );
	
	m_Actor->setAngularVelocity( NxVec3( MoveVec ) );

	// ジャンプ
	CMatrix mTmpRot;
	// ジャンプ中であれば移動ベクトルを正しい値に補正する
	mTmpRot.SetRotate( D3DXVECTOR3( 0.0f, -90.0f , 0.0f ) );
	if( JumpVec.y > 0.0001f )
		D3DXVec3TransformCoord( &MoveVec , &MoveVec , &mTmpRot );
	// そうでないならば0.0fを代入
	else 
		MoveVec = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//                        ジャンプ中の移動　　　　　　　　ジャンプによる上へのベクトル
	m_Actor->addForce( NxVec3( MoveVec ) * 5000000000.0f + ( JumpVec ) * 200000000000.0f );

	// ジャンプの上方向へのベクトルを減算していく
	JumpVec.y -= 5.0f; 
	if( JumpVec.y < 0.0f ){ JumpVec.y = 0.0f; }

	// 地面に押さえつける力を加える
	m_Actor->addForce( NxVec3( 0.0f , -800000000000.0f , 0.0f ) );

	
	// 座標の更新
	D3DXMATRIXA16 mWorld;
	CDPConverter::SetMatrix( &mWorld , m_Actor );
	// 座標の更新
	m_vPos.x = mWorld._41;
	m_vPos.y = mWorld._42 + ( m_TotalHeight/2 ) * 0.7f;
	m_vPos.z = mWorld._43;

	DIRECTX->SetCamPos( m_vPos );

}

/*============================================*/
//! \brief 足場判定
//! \return 接地中 = true
//! \return 空中にいる = false
/*============================================*/
bool CPlayer::CheckFooting( void )
{

	// レイを飛ばす場所
	NxVec3 orig = m_vPos;

	// レイの飛ばす方向
	NxVec3 dir = NxVec3( 0.0f , -1.0f , 0.0f );

	NxRay ray( orig , dir );
	NxRaycastHit hit;
	
	// レイ発射
	NxShape* closestShape = m_pPhysX->GetScene()->raycastClosestShape( ray , NX_ALL_SHAPES , hit , ~CPhysX::BitShift( CPhysX::CG_PLAYER ) );
	// 当たらなかった場合NULL
	if( closestShape )
	{
		if( hit.distance < m_TotalHeight ) 
		{
			return true;
		}
		return false; 
	} 
	else 
	{
		return false;
	}
}