#include "stdafx.hpp"

#include "../../ObjectBase/CObjectBase.hpp"
#include "../Object/Player/Player/CPlayer.hpp"
#include "CBox.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CBox::CBox( void )
	: m_MovablePower( 0 )
	, m_isJoint( false )
{

	m_FilePass += "Object/Object/Box/Box.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( DIRECTX->GetDevice() , m_FilePass );

	// �}�e���A������ۑ�����
	m_Material = m_lpXFile->GetMaterial();

	// �A�N�^�[�f�[�^
	m_ActorData.Type = 'B';
	m_ActorData.ObjectName = CObjectManager::OBJ_BOX;
	m_ActorData.Dimension = NxVec3( 3.7f , 3.7f , 3.7f );	// �傫��
	m_ActorData.FixFlag = false;
	m_ActorData.AngularDamping = 0.0f;						// �]����₷��
	m_ActorData.Density = 80000000000.0f;					// ���x
	
}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CBox::DiscreteInit( void )
{
	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_HEAVY_OBJECT );
	m_Mass = m_Actor->getMass();
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CBox::~CBox( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CBox::Exec( void )
{

	// ���W���擾
	D3DXMATRIXA16 mWorld;
	CDPConverter::SetMatrix( &mWorld , m_Actor );
	m_vPos.x = mWorld._41;
	m_vPos.y = mWorld._42;
	m_vPos.z = mWorld._43;

	// ���[�U�[�q�b�g���G�l���M�[�𒙂߂�
	AddAntiGravity_Energy( 30 );
	

	if( m_AntiGravity_Energy )
	{


		NxVec3 UpVec( 0.0f , 0.1f , 0.0f ); 
		// ���ʂ��y������
		if( !m_isJoint )
		{
			m_Actor->setMass( m_Mass / 10000 );
			m_Actor->raiseBodyFlag( NX_BF_DISABLE_GRAVITY );
		}

		
		// ������ւ̃x�N�g�����Z�b�g�����V������
		m_Actor->setLinearVelocity( UpVec * ( ( m_AntiGravity_Energy/(float)MaxEnergy ) ) * 200.0f );
		
		// �A�N�^�[�ɉ�]��������
		m_Actor->addTorque( NxVec3( 6.0f , 12.0f , 9.2f )*20000000000.0f );
		
		// �v���C���[�ƐڐG������v���C���[�̐i�s�����ֈړ�����
		ACTOR_DATA* BoxUserData = static_cast<ACTOR_DATA*>( m_Actor->userData );
		if( BoxUserData->ContactPairFlag > 0 )
		{
			// �ڐG�����A�N�^�[�̃��[�U�[�f�[�^���擾
			NxActor *pContactActor = BoxUserData->pContactActor;
			if( pContactActor != nullptr )
			{
				ACTOR_DATA *ContactUserData = static_cast<ACTOR_DATA*>( pContactActor->userData );
				if( ContactUserData != nullptr )
				{ 
					// �ڐG�����A�N�^�[�̃I�u�W�F�N�g�𒲂ׁA���ꂪ�v���C���[�Ȃ�ΏՓ˂��ꂽ�����֗͂�������
					CObjectBase *Object = ContactUserData->pObject;
					if( Object->GetName() == "Player" )
					{
						m_MovableVec = OBJECT_MANAGER->GetCPlayerObject()->GetPos() - m_vPos;
						m_MovableVec = -m_MovableVec;
						m_MovableVec.y = 0.0f;
						D3DXVec3Normalize( &m_MovableVec , &m_MovableVec );
						//m_MovablePower += (int)D3DXVec3Length( &CDPConverter::toDirectX( m_Actor->getLinearVelocity() ) );
						
						static const int MaxMovablePower = 50;
						m_MovablePower = MaxMovablePower;
						if( m_MovablePower > MaxMovablePower ){ m_MovablePower = MaxMovablePower; }
					}
				}
			}

		}
		if( m_MovablePower )
		{
			m_Actor->setLinearVelocity( CDPConverter::toPhysX( m_MovableVec ) * (float)m_MovablePower );

			m_MovablePower -= 1;
			if( m_MovablePower < 0 ){ m_MovablePower = 0; }
		}
	}
	else
	{
		if( !m_isJoint )
		{ 
			m_Actor->setMass( m_Mass ); 
			m_Actor->clearBodyFlag( NX_BF_DISABLE_GRAVITY );
			// �n�ʂɉ����t����
			m_Actor->addForce( NxVec3( 0.0f , -2800000000000000.0f , 0.0f ) );
		}
	}

	// ���V�\���Ԃ̌o��
	SubAntiGravity_Energy( 1 );

}


/*================================================*/
//! \brief �`��
//! \param[in] &_d3d	�`�悷��ׂɎg�p
/*================================================*/
void CBox::Draw( CDirect3D & _d3d )
{
	if ( m_lpXFile == nullptr ){ return; }

	ColorUpdate();
	
	m_lpXFile->SetMaterial( m_Material );

	D3DXMATRIXA16 matWorld;
	// �}�g���b�N�X�̓]�u
	CDPConverter::SetMatrix( &matWorld , m_Actor );
	_d3d.SetWorldTrans( &matWorld );
	m_lpXFile->Draw( _d3d.GetDevice() );

}


/*=========================================================*/
//! \brief �I�u�W�F�N�g�̐F���X�V����
/*=========================================================*/
void CBox::ColorUpdate( void )
{

	// ���V���Ԃ������قǐF�����Ȃ�
	m_Material[0].Diffuse.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Diffuse.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Diffuse.b = 1.0f;

	m_Material[0].Emissive.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Emissive.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Emissive.b = 1.0f;
	
	m_Material[0].Ambient.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Ambient.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[0].Ambient.b = 1.0f;



	m_Material[1].Diffuse.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Diffuse.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Diffuse.b = 1.0f;

	m_Material[1].Emissive.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Emissive.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Emissive.b = 1.0f;
	
	m_Material[1].Ambient.r = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Ambient.g = 1.0f - m_AntiGravity_Energy / (float)MaxEnergy * 1.0f;
	m_Material[1].Ambient.b = 1.0f;

	m_Material[0].Power = 40.0f;

}


/*=======================================================*/
//! \brief �s����w�肵�ĕ`��
//! \param[in] &_d3d		�`�悷��ׂɎg�p
//! \param[in] *_mWorld		�s��̎w��
/*=======================================================*/
void CBox::Draw( CDirect3D & _d3d , D3DXMATRIX *_mWorld )
{
	if ( m_lpXFile == nullptr ){ return; }

	ColorUpdate();

	m_lpXFile->SetMaterial( m_Material );

	_d3d.SetWorldTrans( _mWorld );
	m_lpXFile->Draw( _d3d.GetDevice() );
}
