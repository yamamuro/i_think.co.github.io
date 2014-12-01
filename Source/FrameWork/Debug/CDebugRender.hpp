#pragma once

#include "stdafx.hpp"
#include "Wrapper/DirectX_PhysX_Converter/DirectX_PhysX_Converter.hpp"

class CDebugRender
{

	/*==========================================================*/
	//!
	//! \file
	//! \class CDebugRender
	//! \brief <b>PhysX�̃f�o�b�O�`��N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/22
	//! \since 2014/09/22
	//!
	/*==========================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CDebugRender( void );
	

	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CDebugRender( void );


	/*=============================================================*/
	//! \brief ���_�f�[�^��PhysX����擾
	//! \param[in]		_pD3DDevice		DirectX�̃f�o�C�X
	//! \param[in]		*_Data			PhysX��̒��_�f�[�^
	/*=============================================================*/
	void RenderData( const LPDIRECT3DDEVICE9 _pD3DDevice, const NxDebugRenderable *_Data );


private:

	/*========================================================*/
	//! \struct _DEBUG_RENDER_VERTEX
	//! \brief <b>DebugRenderer�p���_�t�H�[�}�b�g</b>
	/*========================================================*/
	typedef struct _DEBUG_RENDER_VERTEX
	{
		D3DXVECTOR3 vPos;		//!< ���_���W
		DWORD color;			//!< ���_�̐F
	}DEBUG_RENDER_VERTEX;

	/*=======================================================================*/
	//! \brief �`�揈��
	//! \param[in]		_pD3DDevice		DirectX�̃f�o�C�X
	//!	\param[in]		*_pVertex		���_���
	//! \param[in]		_Type			�����_�����O�̕��@
	//! \param[in]		_VertexCount	���_�̐�
	/*=======================================================================*/
	void RenderBuffer( const LPDIRECT3DDEVICE9 _pD3DDevice, const DEBUG_RENDER_VERTEX *_pVertex, const D3DPRIMITIVETYPE _Type, const int _VertexCount );

	
	static const DWORD m_FVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE );		//! ���_�f�[�^�̍\��

	DEBUG_RENDER_VERTEX* m_DebugRenderVertex;						//! ���_�t�H�[�}�b�g�̃|�C���^

	
};


class CRaycastLine
{

	/*==================================================*/
	//! 
	//! \class CRaycastLine
	//! \brief <b>���C����̎n�_�ƏI�_���Ǘ�</b>
	//! 
	//! \author Y.Yamamuro
	//! \date  2014/09/28
	//! \since 2014/09/28
	//! 
	/*==================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CRaycastLine( void )
	{
	}

	/*=========================================================*/
	// \brief �R���X�g���N�^
	//! \param[in]	_StartPoint		�n�_�̍��W
	//! \param[in]	_EndPoint		�I�_�̍��W
	//! \param[in]	_Color			�F���
	/*=========================================================*/
	CRaycastLine( NxVec3 _StartPoint , NxVec3 _EndPoint , DWORD _Color )
	{
		m_StartPoint = _StartPoint;
		m_EndPoint   = _EndPoint;
		m_Color		 = _Color;
		m_pd3d		 = DIRECTX;
	}


	/*===========================================================*/
	//! ���C�̔��ˈʒu�Ɠ��������ʒu�����Ԑ���`�悷��
	/*===========================================================*/
	void DrawLine( void )
	{

		D3DXVECTOR3 vec = CDPConverter::toDirectX( m_EndPoint ) - CDPConverter::toDirectX( m_StartPoint );

		LINE_VERTEX v[2];

		v[0].color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
		v[1].color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );

		v[0].vPos = CDPConverter::toDirectX( m_StartPoint );
		v[1].vPos = CDPConverter::toDirectX( m_EndPoint );


		// �ꎞ�I�Ƀ��C�g�̌v�Z�𒆎~����
		m_pd3d->GetDevice()->SetRenderState( D3DRS_LIGHTING , FALSE );
		// ���_�̍\���f�[�^�̃Z�b�g
		m_pd3d->GetDevice()->SetFVF( LineVertexFVF );

		// ���C���̕`��
		m_pd3d->GetDevice()->DrawPrimitiveUP( D3DPT_LINESTRIP , 1 , v , sizeof( LINE_VERTEX ) );


		// ���C�g�̌v�Z���ĊJ����
		m_pd3d->GetDevice()->SetRenderState( D3DRS_LIGHTING , TRUE );

	}

	NxVec3 m_StartPoint;		//!< �n�_�̍��W
	NxVec3 m_EndPoint;			//!< �I�_�̍��W
	DWORD  m_Color;				//!< �F���

	CDirect3D *m_pd3d;			//!< �`��Ŏg�p����CDirect3D�̃|�C���^

private:

	/*============================================================*/
	//! \struct LINE_VERTEX
	//! \brief <b>���C���`��Ɏg�p���钸�_���</b>
	/*============================================================*/
	typedef struct LINE_VERTEX
	{
		D3DXVECTOR3 vPos;		//!< ���_���W
		DWORD color;			//!< ���_�F
	}LINE_VERTEX;
	static const int LineVertexFVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE );		//!< ���_�f�[�^�̍\��

};

