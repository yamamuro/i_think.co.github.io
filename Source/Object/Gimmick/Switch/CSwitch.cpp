#include "stdafx.hpp"

#include "../GimmickBase/CGimmickBase.hpp"
#include "CSwitch.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CSwitch::CSwitch( void )
	: m_lpPushTexture( nullptr )
{

	m_FilePass += "Object/Gimmick/Switch/Switch.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	// �A�N�^�[�f�[�^
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_SWITCH;
	m_ActorData.Dimension = NxVec3( 5.2f , 3.0f , 5.2f );	// �傫��
	m_ActorData.AngularDamping = 1000000.0f;
	m_ActorData.FixFlag = true;

	// �e�N�X�`���̓ǂݍ���
	m_lpPushTexture = TEXTURE_MANAGER->Load( &std::string( "Grey.jpg" ) );

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CSwitch::DiscreteInit( void )
{


	// �X�C�b�`����
	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );


	// �S�̕����̃A�N�^�[���쐬����
	// �f�X�N���v�^
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();							// ������
	NxBoxShapeDesc boxDesc;
	boxDesc.setToDefault();								// ������
	boxDesc.dimensions = NxVec3( 8.2f , 2.0f , 8.2f );
	NxActorDesc actorDesc;
	actorDesc.setToDefault();							// ������
	actorDesc.body = NULL;
	actorDesc.globalPose.t = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	actorDesc.shapes.pushBack( &boxDesc );				// �{�b�N�X���A�N�^�[�ɓo�^

	m_pPhysX->GetScene()->createActor( actorDesc );

	// �I�[�o�[���b�v���Ɏg�p����f�[�^
	m_worldBox.extents = m_ActorData.Dimension + NxVec3( 0.0f , 0.1f , 0.0f );		// �����]�T����������
	m_worldBox.center = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );

}



/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CSwitch::~CSwitch( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CSwitch::Exec( void )
{

	D3DXMATRIXA16 mWorld;
	CDPConverter::SetMatrix( &mWorld , m_Actor );

	// ���W�𒊏o
	m_vPos.x = mWorld._41;
	m_vPos.y = mWorld._42;
	m_vPos.z = mWorld._43;

	// �X�C�b�`�̃v�b�V������X�V
	NxMat33 NxMat;
	NxQuat quat( 0.0f , NxVec3( 0 , 1 , 0 ) ); 
	NxMat.fromQuat( quat );
	m_worldBox.rot = NxMat;
	if ( m_pPhysX->GetScene()->checkOverlapOBB( m_worldBox , NX_DYNAMIC_SHAPES , CPhysX::BitShift( CPhysX::CG_HEAVY_OBJECT ) | CPhysX::BitShift( CPhysX::CG_PLAYER ) ) == true )
		m_bWorking = true;
	else
		m_bWorking = false;

}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CSwitch::Draw( CDirect3D &_d3d )
{
	
	if( m_lpXFile == nullptr ){ return; }

	if( m_bWorking )
		// ������Ă����Ԃ̃e�N�X�`���𒣂�t����
		m_lpXFile->GetTexture()[1]->SetTexture( m_lpPushTexture->GetTexture() );
	else 
		// �ғ����̃e�N�X�`���𒣂�t����
		m_lpXFile->GetTexture()[1]->Restore();
	
	D3DXMATRIXA16 matWorld;
	// �}�g���b�N�X�̓]�u
	CDPConverter::SetMatrix( &matWorld , m_Actor );
	_d3d.SetWorldTrans( &matWorld );
	m_lpXFile->Draw( _d3d.GetDevice() );

}


