#include "stdafx.hpp"

#include "../../ObjectBase/CObjectBase.hpp"
#include "CGimmickBase.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CGimmickBase::CGimmickBase( void )
{
}

/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CGimmickBase::~CGimmickBase( void )
{
}

/*==========================================================================================*/
//! \brief ������
//! \param[in]	_Data		�I�u�W�F�N�g�̏������Ɏg�p����f�[�^
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

