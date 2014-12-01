#include "stdafx.hpp"

#include "../Scene/Game/CGame.hpp"
#include "../Object/ObjectBase/CObjectBase.hpp"
#include "CRoomManager.hpp"


/*=====================================================*/
//! \brief コンストラクタ
//! \param[in]	_Name	読み込む部屋の名前
/*=====================================================*/
CRoomManager::CRoomManager( const int _Name )
{

	m_FilePass = "Resource/XFile/Object/Room/";
	// 部屋のメッシュが格納されているファイルのパス
	m_RoomXFilePass[CGame::TUTORIAL_ROOM] = m_FilePass + "TutorialRoom.x";
	m_RoomXFilePass[CGame::PASSAGEWAY]    = m_FilePass + "Passageway.x";
	m_RoomXFilePass[CGame::ROOM1]		  = m_FilePass + "Room1.x";
	m_RoomXFilePass[CGame::ROOM2]		  = m_FilePass + "Room2.x";
	m_RoomXFilePass[CGame::ROOM3]		  = m_FilePass + "Room3.x";
	m_RoomXFilePass[CGame::ROOM4]		  = m_FilePass + "Room4.x";

	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_RoomXFilePass[_Name] );
	m_Actor = m_pPhysX->CreateConvexActor( m_lpXFile->GetlpMesh() );

	// ライトの初期化
	ZeroMemory( &m_Light, sizeof( D3DLIGHT9 ) );
    m_Light.Type       = D3DLIGHT_DIRECTIONAL;             // 光源の種類 ディレクショナルライト

    m_Light.Diffuse  = D3DXCOLOR( 0.4f , 0.4f , 0.4f , 0.3f );
	m_Light.Specular = D3DXCOLOR( 0.3f , 0.3f , 0.3f , 0.3f );
	m_Light.Ambient  = D3DXCOLOR( 0.4f , 0.4f , 0.4f , 0.3f );
   
	m_Light.Direction  = D3DXVECTOR3( 0.0f , -1.0f , 0.0f );     // 光の方向

	// ライトを追加する
    m_pd3d->GetCLightManager()->AddLight( m_Light );


	// フォグの設定
	m_pd3d->GetDevice()->SetRenderState( D3DRS_FOGENABLE , TRUE );
	m_pd3d->GetDevice()->SetRenderState( D3DRS_FOGCOLOR , D3DCOLOR_XRGB( 0 , 0 , 0 ) );
	m_pd3d->GetDevice()->SetRenderState( D3DRS_FOGVERTEXMODE , D3DFOG_LINEAR );
	float Start = 10.0f;
	float End = 200.0f;
	m_pd3d->GetDevice()->SetRenderState( D3DRS_FOGSTART , *(DWORD*)(&Start) );
	m_pd3d->GetDevice()->SetRenderState( D3DRS_FOGEND , *(DWORD*)(&End) );

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CRoomManager::DiscreteInit( void )
{
	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_STATIC );	
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CRoomManager::~CRoomManager( void )
{
}

