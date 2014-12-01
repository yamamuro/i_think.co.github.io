#include "stdafx.hpp"

#include "../Object/Player/Player/CPlayer.hpp"
#include "../GimmickBase/CGimmickBase.hpp"
#include "CWireMesh.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CWireMesh::CWireMesh( void )
	: m_pPlayer( nullptr )
	, m_lpGap( nullptr )
{

	m_FilePass += "Object/Gimmick/WireMesh/WireMesh.x";
	// ���b�V���̓ǂݍ���
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass );

	m_lpGap = XFILE_MANAGER->Load( m_pd3d->GetDevice() , "Resource/XFile/Object/Gimmick/WireMesh/Gap.x" );

	// ���W�̏�����
	for( int i=0 ; i<WireMeshNum ; i++ ){
		m_Pos[i] = D3DXVECTOR3( 0.0f , 0.0f , 2.0f*i );
	}

	m_DrawFlag = true;

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CWireMesh::DiscreteInit( void )
{
	m_Scale = ( m_vScaling.x + m_vScaling.y + m_vScaling.z ) / 3.0f;

	// �A�N�^�[�̋��ʃf�[�^��ݒ�
	m_ActorData.Type = 'B';
	m_ActorData.FixFlag = false;
	m_ActorData.Density = 100000000000.0f;
	m_ActorData.Dimension = NxVec3( 10.0f , 10.0f , 0.0f ) * m_Scale;
	m_ActorData.ObjectName = CObjectManager::OBJ_WIREMESH;
	m_ActorData.pObject = this;
	m_ActorData.SkinWidth = 0.05f;

	for( int i=0 ; i<WireMeshNum ; i++ )
	{
		D3DXVECTOR3 TmpVec = m_Pos[i];
		CMatrix mRot;
		mRot.SetRotate( m_vRot );
		D3DXVec3TransformCoord( &TmpVec , &TmpVec , &mRot );
		m_ActorData.Position  = CDPConverter::toPhysX( TmpVec + m_vPos );
		m_Actors[i] = m_pPhysX->CreateActor( &m_ActorData );
		m_Actors[i]->raiseBodyFlag( NX_BF_KINEMATIC );
		m_pPhysX->SetActorCollisionGroup( m_Actors[i] , CPhysX::CG_THROUGH );
	}
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CWireMesh::~CWireMesh( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CWireMesh::Exec( void )
{
	
	// �v���C���[�̃|�C���^���擾
	if( m_pPlayer == nullptr )
	{
		m_pPlayer = OBJECT_MANAGER->GetCPlayerObject();
		return;
	}

	// �d���̊m�F
	if( OBJECT_MANAGER->GetWorkingFlag( m_Connector ) || m_Connector == 0 )
		m_bWorking = true;
	else 
		m_bWorking = false;

	// ���͈�( �ǂꂾ���J���� )
	const float MovableRange = 19.5f*m_Scale;
	// �X�s�[�h
	static const float Speed = 0.05f*m_Scale;
	// ����
	if( m_bWorking )
	{
		for( int i=0 ; i<WireMeshNum ; i++ )
		{
			if( i%2 == 0 )
			{
				// �E�ɊJ��
				if( m_Pos[i].x < MovableRange )
				{
					m_Pos[i].x += Speed + Speed * ( WireMeshNum - (i-0) )*3;
					if( m_Pos[i].x > MovableRange ){ m_Pos[i].x = MovableRange; }
				}
			}
			else
			{
				// ���ɊJ��
				if( m_Pos[i].x > -MovableRange )
				{
					m_Pos[i].x -= Speed + Speed * ( WireMeshNum - (i-1) )*3;
					if( m_Pos[i].x < -MovableRange ){ m_Pos[i].x = -MovableRange; }
				}
			}
		}
	}
	else
	{
		// ����
		for( int i=0 ; i<WireMeshNum ; i++ )
		{
			if( i%2 == 0 )
			{
				if( m_Pos[i].x > 0 )
				{
					m_Pos[i].x -= Speed + Speed * ( WireMeshNum - (i-0) );
					if( m_Pos[i].x < 0 ){ m_Pos[i].x = 0; }
				}
			}
			else
			{
				if( m_Pos[i].x < 0 )
				{
					m_Pos[i].x += Speed + Speed * ( WireMeshNum - (i-1) );
					if( m_Pos[i].x > 0 ){ m_Pos[i].x = 0; }
				}
			}
		}
	}

	// �A�N�^�[�̍X�V
	for( int i=0 ; i<WireMeshNum ; i++ )
	{
		D3DXVECTOR3 TmpVec = m_Pos[i];
		CMatrix mRot;
		mRot.SetRotate( m_vRot );
		D3DXVec3TransformCoord( &TmpVec , &TmpVec , &mRot );
		m_Actors[i]->setGlobalPosition( CDPConverter::toPhysX( TmpVec + m_vPos ) ) ;
	}

}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
/*=============================================*/
void CWireMesh::Draw( CDirect3D & _Direct3D )
{

	if ( m_lpXFile == nullptr ){ return; }

	CMatrix mScaling;
	CMatrix mRot;
	CMatrix mTrans;
	CMatrix mWorld;

	mScaling.SetScale( m_Scale );
	mRot.SetRotate( m_vRot );


	// ���Ԃ̕`��
	CMatrix mAddRot;
	mAddRot.SetRotate( D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) ) ;
	for( int i=0 ; i<WireMeshNum ; i++ )
	{
		D3DXVECTOR3 TmpPos = m_Pos[i];
		TmpPos.x = 0.0f;
		mTrans.SetTrans( TmpPos );
		if( i % 2 == 0 )
			mWorld = mScaling * mTrans * mRot;
		else
			mWorld = mAddRot * mScaling * mTrans * mRot;

		mWorld._41 += m_vPos.x;
		mWorld._42 += m_vPos.y;
		mWorld._43 += m_vPos.z;
	
		_Direct3D.SetWorldTrans( &mWorld );
		m_lpGap->Draw( _Direct3D.GetDevice() );
	}


	D3DXVECTOR3 Vec = m_pPlayer->GetPos() - m_vPos;

	D3DXVec3Normalize( &Vec , &Vec );

	float Ang;
	D3DXVECTOR3 FrontVec;
	D3DXVec3TransformCoord( &FrontVec , &D3DXVECTOR3( 0.0f, 0.0f , 1.0f ) , &mRot );
	Ang = D3DXToDegree( D3DXVec3Dot( &Vec , &FrontVec ) );
	
	// �`�揇�����߂�ۂɎg�p
	bool DrawFlag = false;
	if( Ang < 0 ){ DrawFlag = true; }
	for( int i=0 ; i<WireMeshNum ; i++ )
	{

		D3DXVECTOR3 TmpPos = m_Pos[ DrawFlag ? (( WireMeshNum-1)-i ) : ( i ) ];

		mTrans.SetTrans( TmpPos );

		// �s��̍���
		mWorld = mScaling * mTrans * mRot;

		// �s��ɍ��W���I�u�W�F�N�g�̍��W�𑫂�����
		mWorld._41 += m_vPos.x;
		mWorld._42 += m_vPos.y;
		mWorld._43 += m_vPos.z;
	
		_Direct3D.SetWorldTrans( &mWorld );
		m_lpXFile->Draw( _Direct3D.GetDevice() );

	}
	
}

