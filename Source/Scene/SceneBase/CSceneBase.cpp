#include "stdafx.hpp"

#include "../Scene/SceneManager/CSceneManager.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "CSceneBase.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CSceneBase::CSceneBase( void )
	: m_pd3d( CAPP->GetCDirect3D() )
	, m_pPhysX( CAPP->GetCPhysX() )
	, m_pKeyboard( CAPP->GetCKeyboard() )
	, m_pDebug( CAPP->GetCDebug() )
	, m_pObjectManager( CAPP->GetCObjectManager() )
	, m_pSceneManager( CAPP->GetCSceneManager() )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CSceneBase::~CSceneBase( void )
{
	m_pPhysX->AllDeleteActor();
	m_pObjectManager->AllDeleteObject();
}



