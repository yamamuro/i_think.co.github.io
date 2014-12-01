#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "CGame.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CGame::CGame( void )
	//: m_pBGM( nullptr )
{

	m_FilePass = "Resource/Load/Room/";

	m_RoomsDataPass[TUTORIAL_ROOM]	= m_FilePass + "TutorialRoom/TutorialRoom.csv";
	m_RoomsDataPass[PASSAGEWAY]		= m_FilePass + "Passageway/Passageway.csv";
	m_RoomsDataPass[ROOM1]			= m_FilePass + "Room1/Room1.csv";
	m_RoomsDataPass[ROOM2]			= m_FilePass + "Room2/Room2.csv";
	m_RoomsDataPass[ROOM3]			= m_FilePass + "Room3/Room3.csv";
	m_RoomsDataPass[ROOM4]			= m_FilePass + "Room4/Room4.csv";

	m_Name[TUTORIAL_ROOM] = "TutorialRoom";
	m_Name[PASSAGEWAY]	  = "Passageway";
	m_Name[ROOM1]		  = "Room1";
	m_Name[ROOM2]		  = "Room2";
	m_Name[ROOM3]		  = "Room3";
	m_Name[ROOM4]		  = "Room4";

	// ステージのオブジェクトを読み込む
	ChangeRoom( TUTORIAL_ROOM );
	//ChangeRoom( PASSAGEWAY );

	//m_pBGM = SOUND_MANAGER->Load( "BGM.wav" , 1 );

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CGame::~CGame( void )
{
}


/*===========================================*/
//! \brief 更新
//! \return シーンの変更 = true
//! \return 継続 　　　　= false
/*===========================================*/
bool CGame::Updata( void )
{
	
	// エスケープキーでタイトル画面に戻る
	if( m_pKeyboard->GetState( CKeyboard::KEY_ESCAPE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::TITLE );
		return true;
	}

	// ステージ移動
//#ifdef _DEBUG
	if( m_pKeyboard->GetState( CKeyboard::KEY_1 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( TUTORIAL_ROOM );
		return true;
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_2 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( PASSAGEWAY );
		return true;
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_3 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( ROOM1 );
		return true;
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_4 ) == CKeyboard::KST_PUSH )
	{
		ChangeRoom( ROOM2 );
		return true;
	}

//#endif


	DEBUG->DebugKeyCood( m_RoomID );

	m_pObjectManager->Updata();

	//m_pBGM->RePlay();

	return false;

}


/*=================================*/
//! \brief 画面表示
/*=================================*/
void CGame::Draw( void )
{

	// 描画準備
	LPDIRECT3DDEVICE9 Direct3D = m_pd3d->Begin();
	m_pd3d->SetProj();
	m_pd3d->SetCamera();

	m_pObjectManager->Draw();

	// FPSを表示する
	if( m_pDebug != nullptr ){ m_pDebug->DrawFPS(); }
	
	m_pd3d->End();

}


/*=================================*/
//! \brief 部屋を切り替える
/*=================================*/
void CGame::ChangeRoom( const int _ID )
{

	DEBUG->PrintConsole( "\n\n\n" );
	DEBUG->PrintConsole( "\n===前部屋のデータ解放===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );

	// ライトの削除
	m_pd3d->GetCLightManager()->AllDelete();

	// 現在持っているオブジェクトを削除する
	OBJECT_MANAGER->AllDeleteObject();

	// XFileを解放する
	XFILE_MANAGER->AllClear();

	// テクスチャの解放
	TEXTURE_MANAGER->AllClear();

	// Waveの解放
	//SOUND_MANAGER->ReleaseWaves();

	// アクターの解放
	m_pPhysX->AllDeleteActor();
	// ジョイント破壊フラグを下げる
	m_pPhysX->GetJointNotify()->toBreak = false;

	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( "\n\n" );

	// 部屋IDのセット
	SetRoomID( _ID );

	// 部屋のパスをセット
	SetRoomPass( m_RoomsDataPass[_ID] );


	// ここにステージ切り替えのエフェクトを入れる
	 
	DEBUG->PrintConsole( "\n\n\n" );
	DEBUG->PrintConsole( "\n===ステージ[" + m_Name[_ID] + "]を読み込みます===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	// オブジェクトを読み込む
	m_pObjectManager->LoadObject( GetRoomDataPass() );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( "\n\n" );


}