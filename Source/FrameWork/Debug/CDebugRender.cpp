
#include "stdafx.hpp"

#include "Direct3D\CDirect3D.hpp"
#include "CDebugRender.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CDebugRender::CDebugRender( void )
	: m_DebugRenderVertex( nullptr )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CDebugRender::~CDebugRender( void )
{
}


/*=============================================================*/
//! \brief 頂点データをPhysXから取得
//! \param[in]		_pD3DDevice		DirectXのデバイス
//! \param[in]		*_Data			PhysX上の頂点データ
/*=============================================================*/
void CDebugRender::RenderData( const LPDIRECT3DDEVICE9 _pD3DDevice, const NxDebugRenderable *_Data )
{

	// 頂点の座標を更新することで移動させているようなのでワールド行列は単位行列にする
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	SafeDelete( m_DebugRenderVertex );

	if( _Data == NULL || _pD3DDevice == NULL )
		goto ERROR_EXIT;

	DWORD dwCount;

	/*===============================*/
	// 点描画
	/*===============================*/

	// 頂点数を取得
	unsigned int NbPoints = _Data->getNbPoints();
	if(NbPoints)
	{
		// 頂点数ぶんの領域確保
		m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbPoints];
		if( m_DebugRenderVertex == NULL )
			goto ERROR_EXIT;

		// 頂点データを取得
		const NxDebugPoint* Points = _Data->getPoints();

		dwCount = 0;

		// 全頂点の情報をDirect3Dの描画用の構造体にコピー
		while(NbPoints--)
		{
			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Points->p.x, Points->p.y, Points->p.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Points->color;
			dwCount++;
			Points++;
		}

		// 描画処理へ
		RenderBuffer( _pD3DDevice, m_DebugRenderVertex, D3DPT_POINTLIST, _Data->getNbPoints() );

		SafeDelete( m_DebugRenderVertex );
	}

	/*===============================*/
	// 辺描画
	/*===============================*/

	// 辺数を取得
	unsigned int NbLines = _Data->getNbLines();
	if(NbLines)
	{
		// 辺数ぶんの領域確保
		m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbLines * 2];
		if( m_DebugRenderVertex == NULL )
			goto ERROR_EXIT;

		// 辺データを取得
		const NxDebugLine* Lines = _Data->getLines();

		dwCount = 0;

		// 全辺の情報をDirect3Dの描画用の構造体にコピー
		while(NbLines--)
		{
			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Lines->p0.x, Lines->p0.y, Lines->p0.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Lines->color;
			dwCount++;

			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Lines->p1.x, Lines->p1.y, Lines->p1.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Lines->color;
			dwCount++;

			Lines++;
		}

		// 描画処理へ
		RenderBuffer( _pD3DDevice, m_DebugRenderVertex, D3DPT_LINELIST, _Data->getNbLines() );

		SafeDelete( m_DebugRenderVertex );
	}

	/*===============================*/
	// △ポリゴン描画
	/*===============================*/

	// △ポリゴン数を取得
	unsigned int NbTriangles = _Data->getNbTriangles();
	if(NbTriangles)
	{
		// △ポリゴン数ぶんの領域確保
		m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbTriangles * 3];
		if( m_DebugRenderVertex == NULL )
			goto ERROR_EXIT;

		// △ポリゴンデータを取得
		const NxDebugTriangle* Triangles = _Data->getTriangles();

		dwCount = 0;

		// 全△ポリゴンの情報をDirect3Dの描画用の構造体にコピー
		while(NbTriangles--)
		{
			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Triangles->p0.x, Triangles->p0.y, Triangles->p0.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Triangles->color;
			dwCount++;

			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Triangles->p1.x, Triangles->p1.y, Triangles->p1.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Triangles->color;
			dwCount++;

			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Triangles->p2.x, Triangles->p2.y, Triangles->p2.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Triangles->color;
			dwCount++;

			Triangles++;
		}

		// 描画処理へ
		RenderBuffer( _pD3DDevice, m_DebugRenderVertex, D3DPT_TRIANGLELIST, _Data->getNbTriangles() );

		SafeDelete( m_DebugRenderVertex );
	}

	return;

ERROR_EXIT:

	return;
}


/*=======================================================================*/
//! \brief 描画処理
//! \param[in]		_pD3DDevice		DirectXのデバイス
//!	\param[in]		*_pVertex		頂点情報
//! \param[in]		_Type			レンダリングの方法
//! \param[in]		_VertexCount	頂点の数
/*=======================================================================*/
void CDebugRender::RenderBuffer( const LPDIRECT3DDEVICE9 _pD3DDevice, const DEBUG_RENDER_VERTEX *_pVertex, const D3DPRIMITIVETYPE _Type, const int _VertexCount )
{

	// 現在のレンダーステートの設定を退避

	DWORD RStateLightingBK;
	_pD3DDevice->GetRenderState( D3DRS_LIGHTING, &RStateLightingBK );
	// ライティングを無効
	_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD RStateCullModeBK;
	_pD3DDevice->GetRenderState( D3DRS_CULLMODE, &RStateCullModeBK );
	// 両面レンダリング
	_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	IDirect3DBaseTexture9* ppTexBK = NULL;
	_pD3DDevice->GetTexture( 0, &ppTexBK );
	// テクスチャーを無効にする
	_pD3DDevice->SetTexture( 0, NULL );

	DWORD FVFBK;
	_pD3DDevice->GetFVF( &FVFBK );
	_pD3DDevice->SetFVF( m_FVF );

	// 描画
	_pD3DDevice->DrawPrimitiveUP( _Type, _VertexCount , _pVertex, sizeof(DEBUG_RENDER_VERTEX) );

	// 戻す
	_pD3DDevice->SetRenderState( D3DRS_LIGHTING, RStateLightingBK );
	_pD3DDevice->SetRenderState( D3DRS_CULLMODE, RStateCullModeBK );
	_pD3DDevice->SetTexture( 0, ppTexBK );
	_pD3DDevice->SetFVF( FVFBK );

}
