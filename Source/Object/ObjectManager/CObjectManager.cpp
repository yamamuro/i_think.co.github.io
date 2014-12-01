#include "stdafx.hpp"

#include "../ObjectBase/CObjectBase.hpp"

// プレイヤー
#include "../Object/Player/Player/CPlayer.hpp"
#include "../Object/Player/LaserGun/LaserBase/CLaserBase.hpp"
#include "../Object/Player/LaserGun/LaserManager/CLaserManager.hpp"

// オブジェクト
#include "../Object/Object/Box/CBox.hpp"
#include "../Object/Rope/CRope.hpp"
#include "../Object/Rope/CChainBox.hpp"
#include "../Object/Object//Fence/CFence.hpp"
#include "../Object/Object//Bridge/CBridge.hpp"
#include "../Object/FluorescentLight/CFluorescentLight.hpp"
#include "../Object/BreakWall/CBreakWall.hpp"

// ギミック
#include "../Object/Gimmick/Switch/CSwitch.hpp"
#include "../Object/Gimmick/WireMesh/CWireMesh.hpp"
#include "../Object/Gimmick/Lift/CLiftX.hpp"
#include "../Object/Gimmick/Lift/CLiftY.hpp"
#include "../Object/Gimmick/Balance/CBalance.hpp"
#include "../Object/Gimmick/Stairs/CStairs.hpp"
#include "../Object/Gimmick/Conveyer/CConveyer.hpp"
#include "../Object/Gimmick/SurveillanceCamera/CSurveillanceCamera.hpp"
#include "../Object/Gimmick/ClueLever/CClueLever.hpp"

// 敵
#include "../Object/Enemy/Fool/CFool.hpp"
#include "../Object/Enemy/Tackle/CTackle.hpp"

// 部屋
#include "../Object/Room/RoomManager/CRoomManager.hpp" 

#include "../Scene/Game/CGame.hpp"
#include "CObjectManager.hpp"


/*==============================================*/
//! CObjectManagerのポインタの定義
/*==============================================*/
CObjectManager* CObjectManager::m_pInstance = nullptr;

/*==============================================*/
//! CobjectBase*を格納するvectorの定義
/*==============================================*/
std::vector< CObjectBase*  > CObjectManager::m_ObjectList;
std::vector< CGimmickBase* > CObjectManager::m_GimmickList;
std::vector< CObjectBase*  > CObjectManager::m_AddObjectList;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CObjectManager::CObjectManager( void )
	: m_ErrorCheckCounter( 0 )
	, m_pPlayer( nullptr )
	, m_pD3D( nullptr )
	, m_pPhysX( nullptr )
{

	m_pD3D = DIRECTX;
	m_pPhysX = PHYSX;

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CObjectManager::~CObjectManager( void )
{
	// 確保しているオブジェクトをメモリから解放する
	AllDeleteObject();
	// プレイヤーのインスタンスはAllDeleteObjectで既にデリートしているのでnullptrを格納する
	m_pPlayer = nullptr;
}


/*============================*/
//! \brief 更新
/*============================*/
void CObjectManager::Updata( void )
{

	// オブジェクトの更新
 	for( auto& x : m_ObjectList )
		x->Exec();
	for( auto& x : m_AddObjectList )
		x->Exec();


	// レーザーに当たっているか判別する為のフラグの初期化
	for( auto& x : m_ObjectList )
		x->SetLaserHitFlag( false );
	for( auto& x : m_AddObjectList )
		x->SetLaserHitFlag( false );

}


/*============================*/
//! \brief 描画
/*============================*/
void CObjectManager::Draw( void )
{

	std::vector< CObjectBase * > Deferment;		// 描画を後回しにするもの

	if( m_pD3D->GetDrawFlag() )
	{

		for( auto& x : m_AddObjectList )
		{
				x->Draw( *m_pD3D );
		}

		for( auto& x : m_ObjectList )
		{
			if( !x->GetDefermentFlag() )
				x->Draw( *m_pD3D );
			else
				Deferment.push_back( x );
		}
		
	}

	// アクターのワイヤーフレームを描画
	// デバッグ描画
	if( m_pPhysX->GetDebugRender() != nullptr && m_pPhysX->GetPhysXDrawFlag() )
	{
		m_pPhysX->GetDebugRender()->RenderData( m_pD3D->GetDevice(), m_pPhysX->GetScene()->getDebugRenderable() );
	}	


	// 後回しにしたものをソート//////////////
	std::vector< SORT_STRUCT > Data;
	unsigned int i = 0;
	for( auto &x : Deferment )
	{
		SORT_STRUCT Tmp;
		D3DXVECTOR3 Dis = x->GetPos() - m_pPlayer->GetPos();
		Tmp.Distance = D3DXVec3Length( &Dis );
		Tmp.ElementNum = i;
		Data.push_back( Tmp );
		i++;
	}
	std::sort( Data.begin() , Data.end() , std::greater<SORT_STRUCT>() );
	/////////////////////////////////////////

	// ソートしたものを描画
	for( i=0 ; i<Deferment.size() ; i++ )
	{
		Deferment[Data[i].ElementNum]->Draw( *m_pD3D );
	}

	// Zバッファをクリア
	m_pD3D->ClearZBuffer();

	// レーザー銃とそのパーティクルを描画
	m_pPlayer->GetLaserGun()->Draw( *m_pD3D );

}


/*=======================================================================================*/
//! \brief オブジェクトを読み込む
//! \param[in]		_FilePass		読み込むデータを保存しているcsvファイルのパス
//! \return 成功 = true
//! \return 失敗 = false
/*=======================================================================================*/
bool CObjectManager::LoadObject( const std::string _FilePass )
{

	std::vector< OBJECT_DATA > Data;

	// 既にオブジェクトが解放されているので再初期化する
	m_pPlayer = nullptr;

	// カウンターの初期化
	m_ErrorCheckCounter = 0; 

	
	std::vector< LOAD_OBJECT_DATA > DataStr;
	const std::string Sep( "," );
	const std::string Ignore( " 　\t" );

	DEBUG->PrintConsole( "\n===Loading Object===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );

	// データの読み込み
	if( CLoadFile::LoadFile( DataStr , DATA_STRUCTURE_MAX , _FilePass , Sep , Ignore ) )
	{

		for( auto &x : DataStr )
		{
			OBJECT_DATA Tmp;
			if( Tmp.Init( x ) )
			{
				Data.push_back( Tmp );
			}
			else
			{
				// エラー処理
				CLoadFile::FileErrorMessage( _FilePass , CLoadFile::READ_ERROR );
				PostQuitMessage( 0 );
				return false;
			}
		}

	}
	else
	{
		// エラー処理
		PostQuitMessage( 0 );
		return false;
	}

	
	// データを元にオブジェクトを生成する
	for( auto &x : Data )
	{
		CObjectBase* Tmp = MakeObject( x , _FilePass );
		if( Tmp != nullptr )
		{
			m_ObjectList.push_back( Tmp );
		}
		else
		{
			// エラー処理
			CLoadFile::FileErrorMessage( _FilePass , CLoadFile::READ_ERROR );
			PostQuitMessage( 0 );
			return false;
		}
	}
	

	// MakeObject内でカウント
	// 部屋データを読み込んでいない、もしくは1つ以上の部屋データを読み込もうとした場合エラーを出す
	if( m_ErrorCheckCounter != 1 )
	{
		// エラー処理
		CLoadFile::FileErrorMessage( _FilePass , CLoadFile::READ_ERROR );
		PostQuitMessage( 0 );
		return false;
	}

	DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );

	return true;

}


/*========================================================*/
//! \brief オブジェクトを追加をする
//! \param[in]	_Name	追加するオブジェクトの名前
//! \param[in]	_Pos	追加する際の初期座標
/*========================================================*/
CObjectBase* CObjectManager::AddObject( const std::string _Name , const D3DXVECTOR3 _Pos )
{

	std::string FileName = "Makeing";
	
	LOAD_OBJECT_DATA Data;
	Data.Str[OBJECT_NAME]		= _Name;
	Data.Str[POS_X]				= std::to_string( _Pos.x );
	Data.Str[POS_Y]				= std::to_string( _Pos.y );
	Data.Str[POS_Z]				= std::to_string( _Pos.z );
	Data.Str[ROT_X]				= "0";
	Data.Str[ROT_Y]				= "0";
	Data.Str[ROT_Z]				= "0";
	Data.Str[GIMMICK_ID]		= "0";
	Data.Str[GIMMICK_RECEVER]	= "0";
	OBJECT_DATA TmpData;
	TmpData.Init( Data );
	CObjectBase* Tmp = MakeObject( TmpData , FileName );
	m_AddObjectList.push_back( Tmp );

	return Tmp;

}

/*========================================================*/
//! \brief オブジェクトを追加をする
//! \param[in]	*_Object	追加するオブジェクト
/*========================================================*/
void CObjectManager::AddObject( CObjectBase *_Object )
{
	m_ObjectList.push_back( _Object );
}


/*=======================================================*/
//! \brief 確保している全オブジェクトを解放する
/*=======================================================*/
void CObjectManager::AllDeleteObject( void )
{

	if( m_ObjectList.size() != 0 || m_AddObjectList.size() != 0 )
	{

		DEBUG->PrintConsole( "===Release Object===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		for( auto& x : m_ObjectList )
		{
			SafeDelete( x , x->GetName() );
		}
		m_ObjectList.clear();
		m_GimmickList.clear();


		for( auto& x : m_AddObjectList )
		{
			SafeDelete( x , x->GetName() );
		}
		m_AddObjectList.clear();
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		DEBUG->PrintConsole( "\n" );
	}

}


/*=================================================================================================*/
//! \brief ここで全てのオブジェクトを生成する
//! \brief 外部ファイルから読み込む
//! \param[in]	_ObjectData		オブジェクトを構成するデータ
//! \param[in]	_FilePass		エラー時に出力で使用する不具合のあるファイルのファイル名
//! \return 生成したオブジェクト
/*=================================================================================================*/
CObjectBase* CObjectManager::MakeObject( const OBJECT_DATA _ObjectData , const std::string _FilePass )
{

	CObjectBase* pObject = nullptr;
	CGimmickBase* pGimmick = nullptr;
	bool GimmickFlag = false;
	

	std::string ObjectName;
	ObjectName = _ObjectData.Name;

	if( false ){}

	// 主人公
	else if( _ObjectData.Name == "Player" ){ pObject = m_pPlayer = new CPlayer(); }

	// オブジェクト
	else if( _ObjectData.Name == "Box"       ){ pObject = new CBox();			   }
	else if( _ObjectData.Name == "Rope"      ){ pObject = new CRope();             }
	else if( _ObjectData.Name == "ChainBox"  ){ pObject = new CChainBox();         }
	else if( _ObjectData.Name == "Fence"     ){ pObject = new CFence();			   }
	else if( _ObjectData.Name == "Bridge"    ){ pObject = new CBridge();		   }
	else if( _ObjectData.Name == "Light"     ){ pObject = new CFluorescentLight(); }
	else if( _ObjectData.Name == "BreakWall" ){ pObject = new CBreakWall();        }

	// ギミック
	else if( _ObjectData.Name == "Switch"				){ pObject = pGimmick = new CSwitch();				GimmickFlag = true; }
	else if( _ObjectData.Name == "WireMesh"				){ pObject = pGimmick = new CWireMesh();			GimmickFlag = true; }
	else if( _ObjectData.Name == "LiftX"				){ pObject = pGimmick = new CLiftX();				GimmickFlag = true; }
	else if( _ObjectData.Name == "LiftY"				){ pObject = pGimmick = new CLiftY();				GimmickFlag = true; }
	else if( _ObjectData.Name == "Balance"				){ pObject = pGimmick = new CBalance();				GimmickFlag = true; }
	else if( _ObjectData.Name == "Stairs"				){ pObject = pGimmick = new CStairs();				GimmickFlag = true; }
	else if( _ObjectData.Name == "Conveyer"				){ pObject = pGimmick = new CConveyer();			GimmickFlag = true; }
	else if( _ObjectData.Name == "SurveillanceCamera"	){ pObject = pGimmick = new CSurveillanceCamera();	GimmickFlag = true; }
	else if( _ObjectData.Name == "ClueLever"			){ pObject = pGimmick = new CClueLever();			GimmickFlag = true; }

	// 敵
	else if( _ObjectData.Name == "Fool"	  ){ pObject = new CFool();   }
	else if( _ObjectData.Name == "Tackle" ){ pObject = new CTackle(); }

	// 部屋
	else if( _ObjectData.Name == "TutorialRoom"   ){ pObject = new CRoomManager( CGame::TUTORIAL_ROOM ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Passageway"	  ){ pObject = new CRoomManager( CGame::PASSAGEWAY    ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room1"		  ){ pObject = new CRoomManager( CGame::ROOM1         ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room2"		  ){ pObject = new CRoomManager( CGame::ROOM2         ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room3"		  ){ pObject = new CRoomManager( CGame::ROOM3         ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room4"		  ){ pObject = new CRoomManager( CGame::ROOM4         ); m_ErrorCheckCounter++; }


	// ここで座標などをセット
	pObject->Init( _ObjectData );
	pObject->SetActor( CAPP->GetCPhysX()->CreateActor( pObject->GetActorData() ) );
	
	m_pPhysX->SetActorCollisionGroup( pObject->GetActor() , CPhysX::CG_DEFAULT );

	std::string String = ( boost::format( "Name : %-22sのオブジェクトを作成しました\n" ) % pObject->GetName().c_str() ).str();
	DEBUG->PrintConsole( String , CDebug::ConObjectMake );
	
	pObject->DiscreteInit();


	// ギミックを専用のベクターに格納する
	if( GimmickFlag ){ m_GimmickList.push_back( pGimmick ); }

	return pObject;

}


/*==========================================================*/
// 指定したIDのギミックを探し、動作しているのか返す
//! \param[in]	_Connector	探したいギミックのID
//! \return 動作中 = true　
//! \return 動作していない = false
/*==========================================================*/
bool CObjectManager::GetWorkingFlag( const int _Connector )
{
	for( auto& x : m_GimmickList )
	{
		// 引数を元に電源となるギミックを走査
		if( abs(_Connector) == x->GetSenderID() )
		{
			// そのギミックが動作しているのか返す
			if( _Connector > 0 ){ return x->GetWorking(); }
			// Notの場合
			else{ return !x->GetWorking(); }
		}
	}
	return false;
}


/*============================================================*/
//! \brief 指定したオブジェクトが浮遊中か調べる
//! \param[in]	*_Actor		探すオブジェクトのアクター
//! \return 浮遊中 = true
//! \return 浮遊していない = false 
/*============================================================*/
bool CObjectManager::GetAntiGravityFlag( const NxActor *_Actor )
{
	for( auto& x : m_ObjectList )
	{
		if( x->GetAntiGravityFlag() )
		{
			if( x->GetActor() == _Actor ){ return true; }
		}
	}
	for( auto& x : m_AddObjectList )
	{
		if( x->GetAntiGravityFlag() )
		{
			if( x->GetActor() == _Actor ){ return true; }
		}
	}
	return false;
}


/*============================================================*/
//! \brief アクターからオブジェクトを見つける
//! \param[in]	*_Actor		探すオブジェクトのアクター
//! \return オブジェクトのポインタ
/*============================================================*/
CObjectBase* CObjectManager::GetActorToObject( const NxActor *_Actor )
{
	for( auto& x : m_ObjectList )
	{
		if( x->GetActor() == _Actor ){ return x; }
	}
	for( auto& x : m_AddObjectList )
	{
		if( x->GetActor() == _Actor ){ return x; }
	}
	return nullptr;
}


/*=======================================================*/
//! \brief オブジェクトの浮遊可能時間を0にする
/*=======================================================*/
void CObjectManager::ResetActiGravity( void )
{
	for( auto& x : m_ObjectList )
	{
		x->ResetAntiGravity();
	}
	for( auto& x : m_AddObjectList )
	{
		x->ResetAntiGravity();
	}
}