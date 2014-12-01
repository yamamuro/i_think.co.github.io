#include "stdafx.hpp"

#include "../Object/Gimmick/GimmickBase/CGimmickBase.hpp"
#include "CStairs.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CStairs::CStairs( void )
	: m_lpProp( nullptr )
{


	m_FilePass += "Object/Gimmick/Stairs/Stair.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	m_lpProp = XFILE_MANAGER->Load( m_pd3d->GetDevice() , "Resource/XFile/Object/Gimmick/Stairs/Prop.x" );

	m_StairsMaterial = m_lpXFile->GetMaterial();
	m_PropMaterial = m_lpProp->GetMaterial();



}


/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CStairs::DiscreteInit( void )
{

	CMatrix mTrans;
	CMatrix mRot;
	CMatrix mWorld;

	mRot.SetRotate( m_vRot );

	// ���W�̏�����
	for( int i=0 ; i<StairsNum ; i++ )
	{

		mTrans.SetTrans( 0.0f , 0.0f , 1.25f+2.5f * i );
		mWorld = mTrans * mRot;

		m_Pos[i] = NxVec3( m_vPos + mWorld.GetPos() );
		// �f�X�N���v�^
		NxBodyDesc bodyDesc;
		bodyDesc.setToDefault();	// ������
		NxBoxShapeDesc boxDesc;
		boxDesc.setToDefault();		// ������
		boxDesc.dimensions = NxVec3( 5.0f*m_vScaling.x , 11.0f , 1.25f );
		NxActorDesc actorDesc;
		actorDesc.setToDefault();	// ������
		actorDesc.body = NULL;
		actorDesc.globalPose.t = m_Pos[i];
		actorDesc.globalPose.M = m_ActorData.Rotation;
		actorDesc.shapes.pushBack( &boxDesc );	// �{�b�N�X���A�N�^�[�ɓo�^
	
		m_Stairs[i] = m_pPhysX->GetScene()->createActor( actorDesc );




		//�x�������̃A�N�^�[
		boxDesc.setToDefault();						// ������
		boxDesc.dimensions = NxVec3( 0.6f , 12.5f , 1.25f );
		actorDesc.setToDefault();					// ������
		actorDesc.body = NULL;
		actorDesc.globalPose.t = m_Pos[i] + NxVec3( -4.8f*m_vScaling.x , i*2.5f , 0.0f );
		actorDesc.shapes.pushBack( &boxDesc );		// �{�b�N�X���A�N�^�[�ɓo�^
		m_pPhysX->GetScene()->createActor( actorDesc );

		NxBoxShapeDesc boxDesc2;
		boxDesc2.setToDefault();					// ������
		boxDesc2.dimensions = NxVec3( 0.6f , 12.5f , 1.25f );
		actorDesc.setToDefault();					// ������
		actorDesc.body = NULL;
		actorDesc.globalPose.t = m_Pos[i] + NxVec3(  4.8f*m_vScaling.x , i*2.5f , 0.0f );
		actorDesc.shapes.pushBack( &boxDesc2 );	// �{�b�N�X���A�N�^�[�ɓo�^
		m_pPhysX->GetScene()->createActor( actorDesc );

	}

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CStairs::~CStairs( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CStairs::Exec( void )
{

	// �d���̊m�F
	if( OBJECT_MANAGER->GetWorkingFlag( m_Connector ) || m_Connector == 0 )
		m_bWorking = true;
	else 
		m_bWorking = false;
	

	// �t���O�����ɊK�i���N������
	static const float Height = 2.5f;
	static const float Speed = 0.4f;
	if( m_bWorking )
	{
		// �K�i���o�Ă���
		for( int i=StairsNum-1 ; i>=0 ; i-- )
		{
			if( m_Pos[i].y < m_vPos.y + i*Height  )
			{
				m_Pos[i].y += Speed;
				if( m_Pos[i].y > m_vPos.y + i*Height ){ m_Pos[i].y = m_vPos.y + i*Height; }
			}
		}
	}
	else
	{
		// �K�i���������Ă���
		for( int i=0 ; i<StairsNum ; i++ )
		{
			if( m_Pos[i].y > m_vPos.y/*����Y���W*/ )
			{
				m_Pos[i].y -= Speed;
				if( m_Pos[i].y < m_vPos.y ){ m_Pos[i].y = m_vPos.y; }
			}
		}
	}

	// �X�V�������W���A�N�^�[�ɃZ�b�g
	for( int i=0 ; i<StairsNum ; i++ )
	{
		NxVec3 Tmp( m_Pos[i].x , m_Pos[i].y , m_Pos[i].z );
		m_Stairs[i]->setGlobalPosition( Tmp );
	}
	

}

/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CStairs::Draw( CDirect3D & _Direct3D )
{

	if( m_lpXFile == nullptr || m_lpProp == nullptr ){ return; }


	m_StairsMaterial[0].Emissive.r = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_StairsMaterial[0].Emissive.g = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_StairsMaterial[0].Emissive.b = 1.0f - !m_bWorking ? 0.4f : 0.9f;

	m_StairsMaterial[0].Ambient.r  = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_StairsMaterial[0].Ambient.g  = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_StairsMaterial[0].Ambient.b  = 1.0f - !m_bWorking ? 0.4f : 0.9f;

	m_PropMaterial[1].Emissive.r   = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_PropMaterial[1].Emissive.g   = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_PropMaterial[1].Emissive.b   = 1.0f - !m_bWorking ? 0.4f : 0.9f;

	m_PropMaterial[1].Ambient.r    = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_PropMaterial[1].Ambient.g    = 1.0f - !m_bWorking ? 0.0f : 0.9f;
	m_PropMaterial[1].Ambient.b    = 1.0f - !m_bWorking ? 0.4f : 0.9f;


	m_lpXFile->SetMaterial( m_StairsMaterial );
	m_lpProp->SetMaterial( m_PropMaterial );


	CMatrix mScaling;
	mScaling.SetScale( m_vScaling.x , 1.0f , 1.0f );
	for( int i=0 ; i<StairsNum ; i++ )
	{
		D3DXMATRIXA16 matWorld;
		// �}�g���b�N�X�̓]�u
		CDPConverter::SetMatrix( &matWorld , m_Stairs[i] );

		matWorld = mScaling * matWorld;

		_Direct3D.SetWorldTrans( &matWorld );
		m_lpXFile->Draw( _Direct3D.GetDevice() );

	}

	for( int i=0 ; i<StairsNum ; i++ )
	{

		mScaling.SetScale( m_vScaling.x , 1.0f+i*0.2f , 1.0f );
		CMatrix mTrans;
		mTrans.SetTrans( m_vPos.x , m_vPos.y , m_Pos[i].z );
		CMatrix mRot;
		mRot.SetRotate( m_vRot );

		CMatrix mWorld;
		mWorld = mScaling * mRot * mTrans * mWorld;

		_Direct3D.SetWorldTrans( &mWorld );
		m_lpProp->Draw( _Direct3D.GetDevice() );

	}

}
