
#include "stdafx.hpp"

#include "CLaser.hpp"

/*===============================================*/
//! \brief デフォルトコンストラクタ
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

	// メッシュの頂点データを保存
	SetLaserVertex();

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLaser::~CLaser( void )
{
	for( int i=0 ; i<LaserEffectNum ; i++ )
		SafeDeleteArray( m_Vertex[i] );
}


/*==========================================================================*/
//! \brief 更新
//! \param[in]	_RayPos		レーザーを伸ばす際に使用する始点と終点
/*==========================================================================*/
void CLaser::Exec( CRaycastLine _RayPos )
{

	// レーザーの長さを整える
	LaserMeshTransform( _RayPos );

}


/*==============================================*/
//! \brief レーザーの頂点座標を格納
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

		// ロック開始
		CLONEVECTOR *pV;
		m_lpLaserEffect[i]->GetlpMesh()->LockVertexBuffer( 0 , ( LPVOID* )&pV );
	
		NumVertex[i] = m_lpLaserEffect[i]->GetlpMesh()->GetNumVertices();

		// 頂点数だけ容量を確保
		m_Vertex[i] = new D3DXVECTOR3[NumVertex[i]];

		// 座標を格納する
		for( unsigned int j=0 ; j<NumVertex[i] ; j++ )
		{
			m_Vertex[i][j] = (pV+j)->Pos;
		}

		// ロック終了
		m_lpLaserEffect[i]->GetlpMesh()->UnlockVertexBuffer();

	}

}


/*================================================================================*/
//! \brief レーザーのメッシュの長さを調節する
//! \param[in]		_Line	始点、終点が格納された座標
/*================================================================================*/
void CLaser::LaserMeshTransform( CRaycastLine _Line )
{

	D3DXVECTOR3 Len = CDPConverter::toDirectX( _Line.m_EndPoint ) - CDPConverter::toDirectX( _Line.m_StartPoint );
	float Distance = D3DXVec3Length( &Len );
	

	for( int i=0 ; i<LaserEffectNum ; i++ )
	{
		// ロック開始
		CLONEVECTOR *pV;
		m_lpLaserEffect[i]->GetlpMesh()->LockVertexBuffer( 0 , ( LPVOID* )&pV );
		for( unsigned int j=0 ; j<NumVertex[i] ; j++ )
		{
			// 元データで上書き
			(pV+j)->Pos = m_Vertex[i][j];
			// メッシュをz方向に引き延ばす
			if( (pV+j)->Pos.z > 0.0f )
				(pV+j)->Pos += D3DXVECTOR3( 0.0f, 0.0f , Distance );
	
		}
		// ロック終了
		m_lpLaserEffect[i]->GetlpMesh()->UnlockVertexBuffer();
	}

}