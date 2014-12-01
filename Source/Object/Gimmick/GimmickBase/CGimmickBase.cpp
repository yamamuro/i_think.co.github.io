#include "stdafx.hpp"

#include "../../ObjectBase/CObjectBase.hpp"
#include "CGimmickBase.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CGimmickBase::CGimmickBase( void )
{
}

/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CGimmickBase::~CGimmickBase( void )
{
}

/*==========================================================================================*/
//! \brief 初期化
//! \param[in]	_Data		オブジェクトの初期化に使用するデータ
/*==========================================================================================*/
void CGimmickBase::Init( CObjectManager::OBJECT_DATA _Data )
{

	Name = _Data.Name;

	m_vPos = _Data.Pos;
	m_vRot = _Data.Rot;
	m_vScaling = _Data.Scaling;

	m_Sender = _Data.GimmickID;
	m_Connector = _Data.Connectoror;

	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	CDPConverter::SetRot( &m_ActorData.Rotation , &m_vRot );

	m_ActorData.pObject = this;

}

