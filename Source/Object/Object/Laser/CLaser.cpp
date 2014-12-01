
#include "stdafx.hpp"

#include "CLaser.hpp"

/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CLaser::CLaser( void )
{

	std::string EffectPass[LaserEffectNum] = { m_FilePass , m_FilePass };
	
	EffectPass[0] += "Object/Player/AntiGravityLaser/LaserEffect.x";
	EffectPass[1] += "Object/Player/AntiGravityLaser/LaserEffect2.x";
	for( int i=0 ; i<LaserEffectNum ; i++ )
	{
		m_lpLaserEffect[i]= XFILE_MANAGER->Load ( m_pd3d->GetDevice() , EffectPass[i] );
	}

	// ���b�V���̒��_�f�[�^��ۑ�
	SetLaserVertex();

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CLaser::~CLaser( void )
{
	for( int i=0 ; i<LaserEffectNum ; i++ )
		SafeDeleteArray( m_Vertex[i] );
}


/*==========================================================================*/
//! \brief �X�V
//! \param[in]	_RayPos		���[�U�[��L�΂��ۂɎg�p����n�_�ƏI�_
/*==========================================================================*/
void CLaser::Exec( CRaycastLine _RayPos )
{

	// ���[�U�[�̒����𐮂���
	LaserMeshTransform( _RayPos );

}


/*==============================================*/
//! \brief ���[�U�[�̒��_���W���i�[
/*==============================================*/
void CLaser::SetLaserVertex( void )
{

	for( int i=0 ; i<LaserEffectNum ; i++ )
	{

		LPD3DXMESH lpTmp;
		m_lpLaserEffect[i]->GetlpMesh()->CloneMeshFVF( D3DXMESH_MANAGED | D3DXMESH_NPATCHES , 
													CustomFVF , m_pd3d->GetDevice() , &lpTmp );

		m_lpLaserEffect[i]->GetlpMesh()->Release();
	
		m_lpLaserEffect[i]->SetlpMesh( &lpTmp );

		// ���b�N�J�n
		CLONEVECTOR *pV;
		m_lpLaserEffect[i]->GetlpMesh()->LockVertexBuffer( 0 , ( LPVOID* )&pV );
	
		NumVertex[i] = m_lpLaserEffect[i]->GetlpMesh()->GetNumVertices();

		// ���_�������e�ʂ��m��
		m_Vertex[i] = new D3DXVECTOR3[NumVertex[i]];

		// ���W���i�[����
		for( unsigned int j=0 ; j<NumVertex[i] ; j++ )
		{
			m_Vertex[i][j] = (pV+j)->Pos;
		}

		// ���b�N�I��
		m_lpLaserEffect[i]->GetlpMesh()->UnlockVertexBuffer();

	}

}


/*================================================================================*/
//! \brief ���[�U�[�̃��b�V���̒����𒲐߂���
//! \param[in]		_Line	�n�_�A�I�_���i�[���ꂽ���W
/*================================================================================*/
void CLaser::LaserMeshTransform( CRaycastLine _Line )
{

	D3DXVECTOR3 Len = CDPConverter::toDirectX( _Line.m_EndPoint ) - CDPConverter::toDirectX( _Line.m_StartPoint );
	float Distance = D3DXVec3Length( &Len );
	

	for( int i=0 ; i<LaserEffectNum ; i++ )
	{
		// ���b�N�J�n
		CLONEVECTOR *pV;
		m_lpLaserEffect[i]->GetlpMesh()->LockVertexBuffer( 0 , ( LPVOID* )&pV );
		for( unsigned int j=0 ; j<NumVertex[i] ; j++ )
		{
			// ���f�[�^�ŏ㏑��
			(pV+j)->Pos = m_Vertex[i][j];
			// ���b�V����z�����Ɉ������΂�
			if( (pV+j)->Pos.z > 0.0f )
				(pV+j)->Pos += D3DXVECTOR3( 0.0f, 0.0f , Distance );
	
		}
		// ���b�N�I��
		m_lpLaserEffect[i]->GetlpMesh()->UnlockVertexBuffer();
	}

}