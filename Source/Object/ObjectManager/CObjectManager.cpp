#include "stdafx.hpp"

#include "../ObjectBase/CObjectBase.hpp"

// �v���C���[
#include "../Object/Player/Player/CPlayer.hpp"
#include "../Object/Player/LaserGun/LaserBase/CLaserBase.hpp"
#include "../Object/Player/LaserGun/LaserManager/CLaserManager.hpp"

// �I�u�W�F�N�g
#include "../Object/Object/Box/CBox.hpp"
#include "../Object/Rope/CRope.hpp"
#include "../Object/Rope/CChainBox.hpp"
#include "../Object/Object//Fence/CFence.hpp"
#include "../Object/Object//Bridge/CBridge.hpp"
#include "../Object/FluorescentLight/CFluorescentLight.hpp"
#include "../Object/BreakWall/CBreakWall.hpp"

// �M�~�b�N
#include "../Object/Gimmick/Switch/CSwitch.hpp"
#include "../Object/Gimmick/WireMesh/CWireMesh.hpp"
#include "../Object/Gimmick/Lift/CLiftX.hpp"
#include "../Object/Gimmick/Lift/CLiftY.hpp"
#include "../Object/Gimmick/Balance/CBalance.hpp"
#include "../Object/Gimmick/Stairs/CStairs.hpp"
#include "../Object/Gimmick/Conveyer/CConveyer.hpp"
#include "../Object/Gimmick/SurveillanceCamera/CSurveillanceCamera.hpp"
#include "../Object/Gimmick/ClueLever/CClueLever.hpp"

// �G
#include "../Object/Enemy/Fool/CFool.hpp"
#include "../Object/Enemy/Tackle/CTackle.hpp"

// ����
#include "../Object/Room/RoomManager/CRoomManager.hpp" 

#include "../Scene/Game/CGame.hpp"
#include "CObjectManager.hpp"


/*==============================================*/
//! CObjectManager�̃|�C���^�̒�`
/*==============================================*/
CObjectManager* CObjectManager::m_pInstance = nullptr;

/*==============================================*/
//! CobjectBase*���i�[����vector�̒�`
/*==============================================*/
std::vector< CObjectBase*  > CObjectManager::m_ObjectList;
std::vector< CGimmickBase* > CObjectManager::m_GimmickList;
std::vector< CObjectBase*  > CObjectManager::m_AddObjectList;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
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
//! \brief �f�X�g���N�^
/*=====================================*/
CObjectManager::~CObjectManager( void )
{
	// �m�ۂ��Ă���I�u�W�F�N�g������������������
	AllDeleteObject();
	// �v���C���[�̃C���X�^���X��AllDeleteObject�Ŋ��Ƀf���[�g���Ă���̂�nullptr���i�[����
	m_pPlayer = nullptr;
}


/*============================*/
//! \brief �X�V
/*============================*/
void CObjectManager::Updata( void )
{

	// �I�u�W�F�N�g�̍X�V
 	for( auto& x : m_ObjectList )
		x->Exec();
	for( auto& x : m_AddObjectList )
		x->Exec();


	// ���[�U�[�ɓ������Ă��邩���ʂ���ׂ̃t���O�̏�����
	for( auto& x : m_ObjectList )
		x->SetLaserHitFlag( false );
	for( auto& x : m_AddObjectList )
		x->SetLaserHitFlag( false );

}


/*============================*/
//! \brief �`��
/*============================*/
void CObjectManager::Draw( void )
{

	std::vector< CObjectBase * > Deferment;		// �`�����񂵂ɂ������

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

	// �A�N�^�[�̃��C���[�t���[����`��
	// �f�o�b�O�`��
	if( m_pPhysX->GetDebugRender() != nullptr && m_pPhysX->GetPhysXDrawFlag() )
	{
		m_pPhysX->GetDebugRender()->RenderData( m_pD3D->GetDevice(), m_pPhysX->GetScene()->getDebugRenderable() );
	}	


	// ��񂵂ɂ������̂��\�[�g//////////////
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

	// �\�[�g�������̂�`��
	for( i=0 ; i<Deferment.size() ; i++ )
	{
		Deferment[Data[i].ElementNum]->Draw( *m_pD3D );
	}

	// Z�o�b�t�@���N���A
	m_pD3D->ClearZBuffer();

	// ���[�U�[�e�Ƃ��̃p�[�e�B�N����`��
	m_pPlayer->GetLaserGun()->Draw( *m_pD3D );

}


/*=======================================================================================*/
//! \brief �I�u�W�F�N�g��ǂݍ���
//! \param[in]		_FilePass		�ǂݍ��ރf�[�^��ۑ����Ă���csv�t�@�C���̃p�X
//! \return ���� = true
//! \return ���s = false
/*=======================================================================================*/
bool CObjectManager::LoadObject( const std::string _FilePass )
{

	std::vector< OBJECT_DATA > Data;

	// ���ɃI�u�W�F�N�g���������Ă���̂ōď���������
	m_pPlayer = nullptr;

	// �J�E���^�[�̏�����
	m_ErrorCheckCounter = 0; 

	
	std::vector< LOAD_OBJECT_DATA > DataStr;
	const std::string Sep( "," );
	const std::string Ignore( " �@\t" );

	DEBUG->PrintConsole( "\n===Loading Object===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );

	// �f�[�^�̓ǂݍ���
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
				// �G���[����
				CLoadFile::FileErrorMessage( _FilePass , CLoadFile::READ_ERROR );
				PostQuitMessage( 0 );
				return false;
			}
		}

	}
	else
	{
		// �G���[����
		PostQuitMessage( 0 );
		return false;
	}

	
	// �f�[�^�����ɃI�u�W�F�N�g�𐶐�����
	for( auto &x : Data )
	{
		CObjectBase* Tmp = MakeObject( x , _FilePass );
		if( Tmp != nullptr )
		{
			m_ObjectList.push_back( Tmp );
		}
		else
		{
			// �G���[����
			CLoadFile::FileErrorMessage( _FilePass , CLoadFile::READ_ERROR );
			PostQuitMessage( 0 );
			return false;
		}
	}
	

	// MakeObject���ŃJ�E���g
	// �����f�[�^��ǂݍ���ł��Ȃ��A��������1�ȏ�̕����f�[�^��ǂݍ������Ƃ����ꍇ�G���[���o��
	if( m_ErrorCheckCounter != 1 )
	{
		// �G���[����
		CLoadFile::FileErrorMessage( _FilePass , CLoadFile::READ_ERROR );
		PostQuitMessage( 0 );
		return false;
	}

	DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );

	return true;

}


/*========================================================*/
//! \brief �I�u�W�F�N�g��ǉ�������
//! \param[in]	_Name	�ǉ�����I�u�W�F�N�g�̖��O
//! \param[in]	_Pos	�ǉ�����ۂ̏������W
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
//! \brief �I�u�W�F�N�g��ǉ�������
//! \param[in]	*_Object	�ǉ�����I�u�W�F�N�g
/*========================================================*/
void CObjectManager::AddObject( CObjectBase *_Object )
{
	m_ObjectList.push_back( _Object );
}


/*=======================================================*/
//! \brief �m�ۂ��Ă���S�I�u�W�F�N�g���������
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
//! \brief �����őS�ẴI�u�W�F�N�g�𐶐�����
//! \brief �O���t�@�C������ǂݍ���
//! \param[in]	_ObjectData		�I�u�W�F�N�g���\������f�[�^
//! \param[in]	_FilePass		�G���[���ɏo�͂Ŏg�p����s��̂���t�@�C���̃t�@�C����
//! \return ���������I�u�W�F�N�g
/*=================================================================================================*/
CObjectBase* CObjectManager::MakeObject( const OBJECT_DATA _ObjectData , const std::string _FilePass )
{

	CObjectBase* pObject = nullptr;
	CGimmickBase* pGimmick = nullptr;
	bool GimmickFlag = false;
	

	std::string ObjectName;
	ObjectName = _ObjectData.Name;

	if( false ){}

	// ��l��
	else if( _ObjectData.Name == "Player" ){ pObject = m_pPlayer = new CPlayer(); }

	// �I�u�W�F�N�g
	else if( _ObjectData.Name == "Box"       ){ pObject = new CBox();			   }
	else if( _ObjectData.Name == "Rope"      ){ pObject = new CRope();             }
	else if( _ObjectData.Name == "ChainBox"  ){ pObject = new CChainBox();         }
	else if( _ObjectData.Name == "Fence"     ){ pObject = new CFence();			   }
	else if( _ObjectData.Name == "Bridge"    ){ pObject = new CBridge();		   }
	else if( _ObjectData.Name == "Light"     ){ pObject = new CFluorescentLight(); }
	else if( _ObjectData.Name == "BreakWall" ){ pObject = new CBreakWall();        }

	// �M�~�b�N
	else if( _ObjectData.Name == "Switch"				){ pObject = pGimmick = new CSwitch();				GimmickFlag = true; }
	else if( _ObjectData.Name == "WireMesh"				){ pObject = pGimmick = new CWireMesh();			GimmickFlag = true; }
	else if( _ObjectData.Name == "LiftX"				){ pObject = pGimmick = new CLiftX();				GimmickFlag = true; }
	else if( _ObjectData.Name == "LiftY"				){ pObject = pGimmick = new CLiftY();				GimmickFlag = true; }
	else if( _ObjectData.Name == "Balance"				){ pObject = pGimmick = new CBalance();				GimmickFlag = true; }
	else if( _ObjectData.Name == "Stairs"				){ pObject = pGimmick = new CStairs();				GimmickFlag = true; }
	else if( _ObjectData.Name == "Conveyer"				){ pObject = pGimmick = new CConveyer();			GimmickFlag = true; }
	else if( _ObjectData.Name == "SurveillanceCamera"	){ pObject = pGimmick = new CSurveillanceCamera();	GimmickFlag = true; }
	else if( _ObjectData.Name == "ClueLever"			){ pObject = pGimmick = new CClueLever();			GimmickFlag = true; }

	// �G
	else if( _ObjectData.Name == "Fool"	  ){ pObject = new CFool();   }
	else if( _ObjectData.Name == "Tackle" ){ pObject = new CTackle(); }

	// ����
	else if( _ObjectData.Name == "TutorialRoom"   ){ pObject = new CRoomManager( CGame::TUTORIAL_ROOM ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Passageway"	  ){ pObject = new CRoomManager( CGame::PASSAGEWAY    ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room1"		  ){ pObject = new CRoomManager( CGame::ROOM1         ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room2"		  ){ pObject = new CRoomManager( CGame::ROOM2         ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room3"		  ){ pObject = new CRoomManager( CGame::ROOM3         ); m_ErrorCheckCounter++; }
	else if( _ObjectData.Name == "Room4"		  ){ pObject = new CRoomManager( CGame::ROOM4         ); m_ErrorCheckCounter++; }


	// �����ō��W�Ȃǂ��Z�b�g
	pObject->Init( _ObjectData );
	pObject->SetActor( CAPP->GetCPhysX()->CreateActor( pObject->GetActorData() ) );
	
	m_pPhysX->SetActorCollisionGroup( pObject->GetActor() , CPhysX::CG_DEFAULT );

	std::string String = ( boost::format( "Name : %-22s�̃I�u�W�F�N�g���쐬���܂���\n" ) % pObject->GetName().c_str() ).str();
	DEBUG->PrintConsole( String , CDebug::ConObjectMake );
	
	pObject->DiscreteInit();


	// �M�~�b�N���p�̃x�N�^�[�Ɋi�[����
	if( GimmickFlag ){ m_GimmickList.push_back( pGimmick ); }

	return pObject;

}


/*==========================================================*/
// �w�肵��ID�̃M�~�b�N��T���A���삵�Ă���̂��Ԃ�
//! \param[in]	_Connector	�T�������M�~�b�N��ID
//! \return ���쒆 = true�@
//! \return ���삵�Ă��Ȃ� = false
/*==========================================================*/
bool CObjectManager::GetWorkingFlag( const int _Connector )
{
	for( auto& x : m_GimmickList )
	{
		// ���������ɓd���ƂȂ�M�~�b�N�𑖍�
		if( abs(_Connector) == x->GetSenderID() )
		{
			// ���̃M�~�b�N�����삵�Ă���̂��Ԃ�
			if( _Connector > 0 ){ return x->GetWorking(); }
			// Not�̏ꍇ
			else{ return !x->GetWorking(); }
		}
	}
	return false;
}


/*============================================================*/
//! \brief �w�肵���I�u�W�F�N�g�����V�������ׂ�
//! \param[in]	*_Actor		�T���I�u�W�F�N�g�̃A�N�^�[
//! \return ���V�� = true
//! \return ���V���Ă��Ȃ� = false 
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
//! \brief �A�N�^�[����I�u�W�F�N�g��������
//! \param[in]	*_Actor		�T���I�u�W�F�N�g�̃A�N�^�[
//! \return �I�u�W�F�N�g�̃|�C���^
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
//! \brief �I�u�W�F�N�g�̕��V�\���Ԃ�0�ɂ���
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