#include "stdafx.hpp"

#include <string>
#include <stdio.h>
#include <Windows.h>
#include "../Object/Player/Player/CPlayer.hpp"
#include "../Scene/Game/CGame.hpp"
#include "CDebug.hpp"


/*==============================================*/
//! CDebugのポインタの定義
/*==============================================*/
CDebug* CDebug::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CDebug::CDebug( void )
	: m_BeforeTime( 0 )
	, m_FPSCounter( 0 )
	, m_SaveFPS( 0 )
{

//デバッグ中かのフラグを格納
#ifdef _DEBUG
	m_bDebugFlag = true;
#else if
	m_bDebugFlag = false;
#endif
	
	// コンソールの作成
	if( m_bDebugFlag == true )
	{  
		::AllocConsole();
		m_Console = GetStdHandle( STD_OUTPUT_HANDLE );
		std::string Title = GAME_TITLE + "デバッグコンソール";
		SetConsoleTitle( TEXT( Title.c_str() ) );
		GetConsoleScreenBufferInfo( m_Console , &m_Backup );
		COORD Tmp;
		Tmp.X = 80;
 		Tmp.Y = 4000;	// バッファを余分に確保しておく
		SetConsoleScreenBufferSize( m_Console , Tmp );
		PrintConsole( "デバッグモードで起動します...\n" );
	}

	// デバッグで作るボックスの座標///////////////////////////
	BoxMakePos[CGame::TUTORIAL_ROOM][0] = D3DXVECTOR3( 0.0f, 60.0f , 0.0f );
	BoxMakePos[CGame::TUTORIAL_ROOM][1] = D3DXVECTOR3( 0.0f,  0.0f , 0.0f );
	BoxMakePos[CGame::TUTORIAL_ROOM][2] = D3DXVECTOR3( 0.0f,  0.0f , 0.0f );

	BoxMakePos[CGame::PASSAGEWAY][0] = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	BoxMakePos[CGame::PASSAGEWAY][1] = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	BoxMakePos[CGame::PASSAGEWAY][2] = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );

	BoxMakePos[CGame::ROOM1][0] = D3DXVECTOR3( 0.0f, 80.0f , 0.0f );
	BoxMakePos[CGame::ROOM1][1] = D3DXVECTOR3( 0.0f,  0.0f , 0.0f );
	BoxMakePos[CGame::ROOM1][2] = D3DXVECTOR3( 0.0f,  0.0f , 0.0f );
	//////////////////////////////////////////////////////////

	
	// デバッグでプレイヤーが移動する座標/////////////////////
	PlayerPos[CGame::TUTORIAL_ROOM][0] = NxVec3( 0.0f, 20.0f , 70.0f );
	PlayerPos[CGame::TUTORIAL_ROOM][1] = NxVec3( 0.0f,  0.0f ,  0.0f );
	PlayerPos[CGame::TUTORIAL_ROOM][2] = NxVec3( 0.0f,  0.0f ,  0.0f );

	PlayerPos[CGame::PASSAGEWAY][0] = NxVec3( -10.0f, 25.0f , 30.0f );
	PlayerPos[CGame::PASSAGEWAY][1] = NxVec3( -10.0f, 45.0f , 40.0f );
	PlayerPos[CGame::PASSAGEWAY][2] = NxVec3(   0.0f,  0.0f ,  0.0f );

	PlayerPos[CGame::ROOM1][0] = NxVec3( 0.0f, 90.0f , 20.0f );
	PlayerPos[CGame::ROOM1][1] = NxVec3( 0.0f,  0.0f ,  0.0f );
	PlayerPos[CGame::ROOM1][2] = NxVec3( 0.0f,  0.0f ,  0.0f );
	//////////////////////////////////////////////////////////

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CDebug::~CDebug( void )
{
	// コンソールの解放
	if( m_bDebugFlag == true )
	{
		PrintConsole( "\nお疲れ様でした...\n" );
		SetConsoleTextAttribute( m_Console , m_Backup.wAttributes );
		::FreeConsole();
	}
}


/*=======================================================*/
//! \brief コンソールに文字列を出力する
//! \param[in]	_String		出力する文字列
//! \param[in]	_Color		色情報
/*=======================================================*/
void CDebug::PrintConsole( const std::string _String , const WORD _Color )
{
#ifdef _DEBUG
	if( m_pInstance != nullptr )
	{

		int n;	// 文字数が格納されるが使用しない

		// 色を更新
		SetConsoleTextAttribute( m_Console , _Color );
		
		// コンソールに出力
		WriteConsole( m_Console , _String.c_str() , _String.size() , (LPDWORD)&n , NULL );

	}
#endif
}


/*=================================*/
//! \brief FPSを表示する
/*=================================*/
void CDebug::DrawFPS( void )
{

	m_FPSCounter++;
	if( m_BeforeTime + 1000 < timeGetTime() )
	{
		m_BeforeTime = timeGetTime();
		m_SaveFPS = m_FPSCounter;
		m_FPSCounter = 0;
	}

	std::string String = "FPS : " + std::to_string( m_SaveFPS );
	DIRECTX->DrawString( String , m_SaveFPS * 5 , 10 );

}


/*==================================================*/
//! \brief デバッグキーを使用する
//! \param[in]	_Room	現在の部屋ID
/*==================================================*/
void CDebug::DebugKeyCood( int _Room )
{
	
	int RoomID = static_cast<CGame *>( SCENE_MANAGER->GetScene() )->GetRoomID();

	
	// ボックス
	if( KEYBOARD->GetState( CKeyboard::KEY_B ) == CKeyboard::KST_PUSH )
		OBJECT_MANAGER->AddObject( "Box" , BoxMakePos[RoomID][0] );

	if( KEYBOARD->GetState( CKeyboard::KEY_N ) == CKeyboard::KST_PUSH )
		OBJECT_MANAGER->AddObject( "Box" , BoxMakePos[RoomID][1] );

	if( KEYBOARD->GetState( CKeyboard::KEY_M ) == CKeyboard::KST_PUSH )
		OBJECT_MANAGER->AddObject( "Box" , BoxMakePos[RoomID][2] );

	// プレイヤー
	if( KEYBOARD->GetState( CKeyboard::KEY_H ) == CKeyboard::KST_PUSH )
		OBJECT_MANAGER->GetCPlayerObject()->SetActorPos( PlayerPos[RoomID][0] );

	if( KEYBOARD->GetState( CKeyboard::KEY_J ) == CKeyboard::KST_PUSH )
		OBJECT_MANAGER->GetCPlayerObject()->SetActorPos( PlayerPos[RoomID][1] );

	if( KEYBOARD->GetState( CKeyboard::KEY_K ) == CKeyboard::KST_PUSH )
		OBJECT_MANAGER->GetCPlayerObject()->SetActorPos( PlayerPos[RoomID][2] );
	
	
}