
#include "stdafx.hpp"

#include "Direct3D\CDirect3D.hpp"
#include "CDebugRender.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CDebugRender::CDebugRender( void )
	: m_DebugRenderVertex( nullptr )
{
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CDebugRender::~CDebugRender( void )
{
}


/*=============================================================*/
//! \brief ���_�f�[�^��PhysX����擾
//! \param[in]		_pD3DDevice		DirectX�̃f�o�C�X
//! \param[in]		*_Data			PhysX��̒��_�f�[�^
/*=============================================================*/
void CDebugRender::RenderData( const LPDIRECT3DDEVICE9 _pD3DDevice, const NxDebugRenderable *_Data )
{

	// ���_�̍��W���X�V���邱�Ƃňړ������Ă���悤�Ȃ̂Ń��[���h�s��͒P�ʍs��ɂ���
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	SafeDelete( m_DebugRenderVertex );

	if( _Data == NULL || _pD3DDevice == NULL )
		goto ERROR_EXIT;

	DWORD dwCount;

	/*===============================*/
	// �_�`��
	/*===============================*/

	// ���_�����擾
	unsigned int NbPoints = _Data->getNbPoints();
	if(NbPoints)
	{
		// ���_���Ԃ�̗̈�m��
		m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbPoints];
		if( m_DebugRenderVertex == NULL )
			goto ERROR_EXIT;

		// ���_�f�[�^���擾
		const NxDebugPoint* Points = _Data->getPoints();

		dwCount = 0;

		// �S���_�̏���Direct3D�̕`��p�̍\���̂ɃR�s�[
		while(NbPoints--)
		{
			m_DebugRenderVertex[dwCount].vPos  = D3DXVECTOR3( Points->p.x, Points->p.y, Points->p.z );
			m_DebugRenderVertex[dwCount].color = (DWORD)Points->color;
			dwCount++;
			Points++;
		}

		// �`�揈����
		RenderBuffer( _pD3DDevice, m_DebugRenderVertex, D3DPT_POINTLIST, _Data->getNbPoints() );

		SafeDelete( m_DebugRenderVertex );
	}

	/*===============================*/
	// �ӕ`��
	/*===============================*/

	// �Ӑ����擾
	unsigned int NbLines = _Data->getNbLines();
	if(NbLines)
	{
		// �Ӑ��Ԃ�̗̈�m��
		m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbLines * 2];
		if( m_DebugRenderVertex == NULL )
			goto ERROR_EXIT;

		// �Ӄf�[�^���擾
		const NxDebugLine* Lines = _Data->getLines();

		dwCount = 0;

		// �S�ӂ̏���Direct3D�̕`��p�̍\���̂ɃR�s�[
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

		// �`�揈����
		RenderBuffer( _pD3DDevice, m_DebugRenderVertex, D3DPT_LINELIST, _Data->getNbLines() );

		SafeDelete( m_DebugRenderVertex );
	}

	/*===============================*/
	// ���|���S���`��
	/*===============================*/

	// ���|���S�������擾
	unsigned int NbTriangles = _Data->getNbTriangles();
	if(NbTriangles)
	{
		// ���|���S�����Ԃ�̗̈�m��
		m_DebugRenderVertex = new DEBUG_RENDER_VERTEX[NbTriangles * 3];
		if( m_DebugRenderVertex == NULL )
			goto ERROR_EXIT;

		// ���|���S���f�[�^���擾
		const NxDebugTriangle* Triangles = _Data->getTriangles();

		dwCount = 0;

		// �S���|���S���̏���Direct3D�̕`��p�̍\���̂ɃR�s�[
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

		// �`�揈����
		RenderBuffer( _pD3DDevice, m_DebugRenderVertex, D3DPT_TRIANGLELIST, _Data->getNbTriangles() );

		SafeDelete( m_DebugRenderVertex );
	}

	return;

ERROR_EXIT:

	return;
}


/*=======================================================================*/
//! \brief �`�揈��
//! \param[in]		_pD3DDevice		DirectX�̃f�o�C�X
//!	\param[in]		*_pVertex		���_���
//! \param[in]		_Type			�����_�����O�̕��@
//! \param[in]		_VertexCount	���_�̐�
/*=======================================================================*/
void CDebugRender::RenderBuffer( const LPDIRECT3DDEVICE9 _pD3DDevice, const DEBUG_RENDER_VERTEX *_pVertex, const D3DPRIMITIVETYPE _Type, const int _VertexCount )
{

	// ���݂̃����_�[�X�e�[�g�̐ݒ��ޔ�

	DWORD RStateLightingBK;
	_pD3DDevice->GetRenderState( D3DRS_LIGHTING, &RStateLightingBK );
	// ���C�e�B���O�𖳌�
	_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD RStateCullModeBK;
	_pD3DDevice->GetRenderState( D3DRS_CULLMODE, &RStateCullModeBK );
	// ���ʃ����_�����O
	_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	IDirect3DBaseTexture9* ppTexBK = NULL;
	_pD3DDevice->GetTexture( 0, &ppTexBK );
	// �e�N�X�`���[�𖳌��ɂ���
	_pD3DDevice->SetTexture( 0, NULL );

	DWORD FVFBK;
	_pD3DDevice->GetFVF( &FVFBK );
	_pD3DDevice->SetFVF( m_FVF );

	// �`��
	_pD3DDevice->DrawPrimitiveUP( _Type, _VertexCount , _pVertex, sizeof(DEBUG_RENDER_VERTEX) );

	// �߂�
	_pD3DDevice->SetRenderState( D3DRS_LIGHTING, RStateLightingBK );
	_pD3DDevice->SetRenderState( D3DRS_CULLMODE, RStateCullModeBK );
	_pD3DDevice->SetTexture( 0, ppTexBK );
	_pD3DDevice->SetFVF( FVFBK );

}
