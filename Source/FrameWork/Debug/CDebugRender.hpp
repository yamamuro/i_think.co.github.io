#pragma once

#include "stdafx.hpp"
#include "Wrapper/DirectX_PhysX_Converter/DirectX_PhysX_Converter.hpp"

class CDebugRender
{

	/*==========================================================*/
	//!
	//! \file
	//! \class CDebugRender
	//! \brief <b>PhysXのデバッグ描画クラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/22
	//! \since 2014/09/22
	//!
	/*==========================================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CDebugRender( void );
	

	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CDebugRender( void );


	/*=============================================================*/
	//! \brief 頂点データをPhysXから取得
	//! \param[in]		_pD3DDevice		DirectXのデバイス
	//! \param[in]		*_Data			PhysX上の頂点データ
	/*=============================================================*/
	void RenderData( const LPDIRECT3DDEVICE9 _pD3DDevice, const NxDebugRenderable *_Data );


private:

	/*========================================================*/
	//! \struct _DEBUG_RENDER_VERTEX
	//! \brief <b>DebugRenderer用頂点フォーマット</b>
	/*========================================================*/
	typedef struct _DEBUG_RENDER_VERTEX
	{
		D3DXVECTOR3 vPos;		//!< 頂点座標
		DWORD color;			//!< 頂点の色
	}DEBUG_RENDER_VERTEX;

	/*=======================================================================*/
	//! \brief 描画処理
	//! \param[in]		_pD3DDevice		DirectXのデバイス
	//!	\param[in]		*_pVertex		頂点情報
	//! \param[in]		_Type			レンダリングの方法
	//! \param[in]		_VertexCount	頂点の数
	/*=======================================================================*/
	void RenderBuffer( const LPDIRECT3DDEVICE9 _pD3DDevice, const DEBUG_RENDER_VERTEX *_pVertex, const D3DPRIMITIVETYPE _Type, const int _VertexCount );

	
	static const DWORD m_FVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE );		//! 頂点データの構成

	DEBUG_RENDER_VERTEX* m_DebugRenderVertex;						//! 頂点フォーマットのポインタ

	
};


class CRaycastLine
{

	/*==================================================*/
	//! 
	//! \class CRaycastLine
	//! \brief <b>レイ判定の始点と終点を管理</b>
	//! 
	//! \author Y.Yamamuro
	//! \date  2014/09/28
	//! \since 2014/09/28
	//! 
	/*==================================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CRaycastLine( void )
	{
	}

	/*=========================================================*/
	// \brief コンストラクタ
	//! \param[in]	_StartPoint		始点の座標
	//! \param[in]	_EndPoint		終点の座標
	//! \param[in]	_Color			色情報
	/*=========================================================*/
	CRaycastLine( NxVec3 _StartPoint , NxVec3 _EndPoint , DWORD _Color )
	{
		m_StartPoint = _StartPoint;
		m_EndPoint   = _EndPoint;
		m_Color		 = _Color;
		m_pd3d		 = DIRECTX;
	}


	/*===========================================================*/
	//! レイの発射位置と当たった位置を結ぶ線を描画する
	/*===========================================================*/
	void DrawLine( void )
	{

		D3DXVECTOR3 vec = CDPConverter::toDirectX( m_EndPoint ) - CDPConverter::toDirectX( m_StartPoint );

		LINE_VERTEX v[2];

		v[0].color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
		v[1].color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );

		v[0].vPos = CDPConverter::toDirectX( m_StartPoint );
		v[1].vPos = CDPConverter::toDirectX( m_EndPoint );


		// 一時的にライトの計算を中止する
		m_pd3d->GetDevice()->SetRenderState( D3DRS_LIGHTING , FALSE );
		// 頂点の構成データのセット
		m_pd3d->GetDevice()->SetFVF( LineVertexFVF );

		// ラインの描画
		m_pd3d->GetDevice()->DrawPrimitiveUP( D3DPT_LINESTRIP , 1 , v , sizeof( LINE_VERTEX ) );


		// ライトの計算を再開する
		m_pd3d->GetDevice()->SetRenderState( D3DRS_LIGHTING , TRUE );

	}

	NxVec3 m_StartPoint;		//!< 始点の座標
	NxVec3 m_EndPoint;			//!< 終点の座標
	DWORD  m_Color;				//!< 色情報

	CDirect3D *m_pd3d;			//!< 描画で使用するCDirect3Dのポインタ

private:

	/*============================================================*/
	//! \struct LINE_VERTEX
	//! \brief <b>ライン描画に使用する頂点情報</b>
	/*============================================================*/
	typedef struct LINE_VERTEX
	{
		D3DXVECTOR3 vPos;		//!< 頂点座標
		DWORD color;			//!< 頂点色
	}LINE_VERTEX;
	static const int LineVertexFVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE );		//!< 頂点データの構成

};

