#include "stdafx.hpp"

#include <string>

#include<d3dx9.h>
#pragma comment( lib , "d3d9.lib"   )
#pragma comment( lib , "d3dx9.lib"  )
#pragma comment( lib , "dxguid.lib" )

#include "Wrapper//Matrix/CMatrix.hpp"
#include "CDirect3D.hpp"


/*==============================================*/
//! CDirect3D�̃|�C���^�̒�`
/*==============================================*/
CDirect3D* CDirect3D::m_pInstance	= nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CDirect3D::CDirect3D( void )
	: m_lpD3D( nullptr )
	, m_lpD3DDevice( nullptr )
	, m_lpBackbuffer( nullptr )
	, m_adapter( 0 )
	, m_pFont( nullptr )
	, m_LightManager( nullptr )
{				
					
	m_CamRx		= 0.0f;
	m_CamRy		= 0.0f;
	m_CamRz		= 0.0f;
	m_CamZoom	= 0.0f;
	m_CamPos = D3DXVECTOR3( 0 , 0 , 0 );

	m_DrawDirectX = true;

}

/*======================================================================*/
//! \brief DirectX�̏���������
//! \param[in]	_hWnd	�E�B���h�E�n���h��
//! \param[in]	_fsc	�t���X�N���[���ɂ��邩�ǂ����̃t���O
//! \param[in]	_scw	�E�B���h�E�̕�
//! \param[in]	_sch	�E�B���h�E�̍���
//! \return	DirectX�̃f�o�C�X��Ԃ�
/*======================================================================*/
const LPDIRECT3DDEVICE9 CDirect3D::Initialize( const HWND _hwnd , const bool _fsc , const int _scw , const int _sch )
{

	m_lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !m_lpD3D )
	{
		// �I�u�W�F�N�g�쐬���s
		MessageBox( NULL , "Direct3D �̍쐬�Ɏ��s���܂����B" , "ERROR" , MB_OK | MB_ICONSTOP );
		// �I������
		return nullptr;
	}
	// �g�p����A�_�v�^�ԍ�
	m_adapter = 0;

	// �E�C���h�E�̍쐬�����������̂ŁADirect3D ������������
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp , sizeof( D3DPRESENT_PARAMETERS ) );
	// Direct3D �������p�����[�^�̐ݒ�
	if ( _fsc )
	{
		// �t���X�N���[�� : �قƂ�ǂ̃A�_�v�^�ŃT�|�[�g����Ă���t�H�[�}�b�g���g�p
		//d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
		D3DDISPLAYMODE disp;
		// ���݂̉�ʃ��[�h���擾
		m_lpD3D->GetAdapterDisplayMode( m_adapter , &disp );
		d3dpp.BackBufferFormat = disp.Format;
	}
	
	// �\���̈�T�C�Y�̐ݒ�
	d3dpp.BackBufferWidth  = _scw;
	d3dpp.BackBufferHeight = _sch;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if ( !_fsc )
	{
		// �E�C���h�E���[�h
		d3dpp.Windowed = 1;
	}

	// Z �o�b�t�@�̎����쐬
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//D3DFMT_D16;

	//�ޯ��ޯ̧��ۯ��\�ɂ���(GetDC���\�ɂȂ�)
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//ؾ�ėp�ɏ�ԏ���ۑ�
	m_param = d3dpp;

	// �f�o�C�X�̍쐬 - T&L HAL
	if ( FAILED( m_lpD3D->CreateDevice( m_adapter , D3DDEVTYPE_HAL , _hwnd , D3DCREATE_HARDWARE_VERTEXPROCESSING , &d3dpp , &m_lpD3DDevice ) ) )
	{
		// ���s�����̂� HAL �Ŏ��s
		if ( FAILED( m_lpD3D->CreateDevice( m_adapter , D3DDEVTYPE_HAL , _hwnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &m_lpD3DDevice ) ) )
		{
			// ���s�����̂� REF �Ŏ��s
			if ( FAILED( m_lpD3D->CreateDevice( m_adapter , D3DDEVTYPE_REF , _hwnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &m_lpD3DDevice ) ) )
			{
				// ���ǎ��s����
				MessageBox( NULL , "DirectX9���������ł��܂���B\n���Ή��̃p�\�R���Ǝv���܂��B" , "ERROR" , MB_OK | MB_ICONSTOP );
				SafeRelease( VARIABLE_DATA( m_lpD3D ) );
				// �I������
				return nullptr;
			}
		}
	}

	// �����_�����O�E�X�e�[�g��ݒ�
	// Z�o�b�t�@�L����->�O��֌W�̌v�Z�𐳊m�ɂ��Ă����
	m_lpD3DDevice->SetRenderState( D3DRS_ZENABLE	  , D3DZB_TRUE );
	m_lpD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE	   );

	// �A���t�@�u�����f�B���O�L����
	m_lpD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );

	// �A���t�@�u�����f�B���O���@��ݒ�
	m_lpD3DDevice->SetRenderState( D3DRS_SRCBLEND  , D3DBLEND_SRCALPHA    );
	m_lpD3DDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );

	// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP   , D3DTOP_MODULATE );
	// �e�N�X�`���̐F���g�p
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	// ���_�̐F���g�p
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );
	// �����_�����O���̐F�̌v�Z���@�̐ݒ�
	m_lpD3DDevice->SetTextureStageState( 0 , D3DTSS_COLOROP   , D3DTOP_MODULATE );

	//���ʃJ�����O
	m_lpD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );

	// �t�B���^�ݒ�
	m_lpD3DDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
	m_lpD3DDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	m_lpD3DDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );

	// ���C�g
	m_lpD3DDevice->SetRenderState( D3DRS_LIGHTING , TRUE );

	m_lpD3DDevice->GetBackBuffer( 0 , 0 , D3DBACKBUFFER_TYPE_MONO , &m_lpBackbuffer );

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_lpD3DDevice->SetFVF( FVF_Vertex );

	// �X�v���C�g�̍쐬
	m_Sprite = new CSprite();

	// ���C�g�}�l�[�W���̍쐬
    m_LightManager = new CLightManager();

	// �t�H���g�̍쐬
	D3DXCreateFont( m_lpD3DDevice ,	// �f�o�C�X
		24 ,						// �����̍���
		0 ,							// ������
		FW_NORMAL ,					// �t�H���g�̑���
		1 ,							// MIPMAP�̃��x��
		FALSE ,						// �C�^���b�N
		DEFAULT_CHARSET ,			// �����Z�b�g
		OUT_DEFAULT_PRECIS ,		// �o�͐��x
		DEFAULT_QUALITY ,			// �o�͕i��
		DEFAULT_PITCH | FF_SWISS ,	// �t�H���g�s�b�`�ƃt�@�~�� 
		"�l�r �o�S�V�b�N" ,			// �t�H���g��
		&m_pFont );					// �t�H���g�f�o�C�X
	

	m_CamRx		= 0.0f;
	m_CamRy		= 0.0f;
	m_CamZoom	= 0.0f;


	DEBUG->PrintConsole( "DirectX�̏������ɐ������܂���...\n" , CDebug::ConPreside );

	// ���������ꍇ�����������C���^�[�t�F�[�X���A��
	return m_lpD3DDevice;
	
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CDirect3D::~CDirect3D( void )
{
	SafeRelease( VARIABLE_DATA( m_pFont ) );
	SafeDelete( VARIABLE_DATA( m_Sprite ) );
	SafeDelete( VARIABLE_DATA( m_LightManager ) );
	SafeRelease( VARIABLE_DATA( m_lpD3DDevice ) );
}


/*=======================================================*/
//! \brief �`��J�n����
//! \brief �`��I�����ɂ�End()���R�[������
//! \return	DirectX�̃f�o�C�X��Ԃ�
/*=======================================================*/
const LPDIRECT3DDEVICE9 CDirect3D::Begin( void )
{										
	m_lpD3DDevice->BeginScene();
	// �o�b�N�o�b�t�@�� Z�o�b�t�@���N���A
	m_lpD3DDevice->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );					
	return m_lpD3DDevice;
}


/*=======================================================*/
//! \brief �`��I������
//! \brief �`��J�n���ɂ�Begin()���R�[������
//! \return ���� = true
//! \return ���s(�f�o�C�X�̃��X�g�Ȃ�) = false
/*=======================================================*/
bool CDirect3D::End( void )
{										
	m_lpD3DDevice->EndScene();

	
	// �f�o�C�X�̃��X�g���Ď�
	try
	{
		// �o�b�N�o�b�t�@���v���C�}���o�b�t�@�ɃR�s�[
 		m_lpD3DDevice->Present( NULL , NULL , NULL , NULL );
	}
	catch( std::exception ex )
	{
		m_lpD3DDevice->Reset( &m_param );
		CAPP->SetErrorFlag();
		ErrorMessage( "DirectX�̃f�o�C�X�����X�g���܂���" );
		PostQuitMessage( 0 );
		return false;
	}

	return true;
}


/*============================================================*/
//! \brief ��������ʂɕ\������
//! \param[in]	_str	�\�����镶����
//! \param[in]	_x		�\�����镶�����x���W
//! \param[in]	_y		�\�����镶�����y���W
/*============================================================*/
void CDirect3D::DrawString( const std::string _str , const int _x , const int _y )
{
	RECT rc;
	rc.left   = _x;
	rc.top    = _y;
	rc.right  = WINDOW_WIDTH;
	rc.bottom = WINDOW_HEIGHT;
	m_pFont->DrawText( NULL , _str.c_str() , -1 , &rc , NULL , 0xFF88FF88 );

}


/*==========================================================*/
//! \brief ��ʐݒ�
//! \param[in]	_fovy	����p
//! \param[in]	_asp	�A�X�y�N�g��
//! \param[in]	_zn		Z�o�b�t�@��0.0f�̈ʒu
//! \param[in]	_zf		Z�o�b�t�@��1.0f�̈ʒu
/*==========================================================*/
void CDirect3D::SetProj( const float _fovy , const float _asp , const float _zn , const float _zf )
{
	D3DXMatrixPerspectiveFovLH( &m_mProj , D3DXToRadian( _fovy ) , _asp , _zn , _zf );
	m_lpD3DDevice->SetTransform( D3DTS_PROJECTION , &m_mProj ); 
}


/*================================*/
//! \brief �J�����̍X�V
/*================================*/
void CDirect3D::SetCamera( void )
{
	CMatrix mPos, mZoom, mRot;
	mZoom.SetTrans( 0 , 0 , m_CamZoom );
	mPos.SetTrans( m_CamPos.x , m_CamPos.y , m_CamPos.z );
	mRot.SetRotate( m_CamRx , m_CamRy , m_CamRz );
	m_mViewSrc = mZoom * mRot * mPos;							// ���_�s��̌�

	m_mViewSrc = m_mViewSrc.GetInverse();						// ���_�s��̍쐬(�t�s��)			
	m_lpD3DDevice->SetTransform( D3DTS_VIEW , &m_mViewSrc );
}

