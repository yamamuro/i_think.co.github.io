#pragma once

/*===================================*/
//!< �O���錾
/*===================================*/
class CSprite;

#include "CLightManager.hpp"

#include "Wrapper/Matrix/CMatrix.hpp"
#include "System.hpp"


class CDirect3D
{

	/*==========================================================*/
	//!
	//! \file
	//! \class CDirect3D
	//! \brief <b>DirectX�̊Ǘ�</b>
	//!
	//! \author �z�z�f�[�^+Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/04
	//!
	/*==========================================================*/

public:

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
	/*==============================================*/
	static CDirect3D* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CDirect3D();
		}
		return m_pInstance;
	}

	/*======================================================================*/
	//! \brief DirectX�̏���������
	//! \param[in]	_hWnd	�E�B���h�E�n���h��
	//! \param[in]	_fsc	�t���X�N���[���ɂ��邩�ǂ����̃t���O
	//! \param[in]	_scw	�E�B���h�E�̕�
	//! \param[in]	_sch	�E�B���h�E�̍���
	//! \return	DirectX�̃f�o�C�X��Ԃ�
	/*======================================================================*/
	const LPDIRECT3DDEVICE9 Initialize( const HWND _hwnd , const bool _fsc = false , const int _scw = WINDOW_WIDTH , const int _sch = WINDOW_HEIGHT );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CDirect3D( void );


	/*=======================================================*/
	//! \brief �`��J�n����
	//! \brief �`��I�����ɂ�End()���R�[������
	//! \return	DirectX�̃f�o�C�X��Ԃ�
	/*=======================================================*/
	const LPDIRECT3DDEVICE9 Begin( void );	


	/*============================*/
	// Z�o�b�t�@���N���A
	/*============================*/
	inline void ClearZBuffer( void ){ m_lpD3DDevice->Clear( 0 , NULL , D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 ); }


	/*=======================================================*/
	//! \brief �`��I������
	//! \brief �`��J�n���ɂ�Begin()���R�[������
	//! \return ���� = true
	//! \return ���s(�f�o�C�X�̃��X�g�Ȃ�) = false
	/*=======================================================*/
	bool End( void );	


	/*============================================================*/
	//! \brief ��������ʂɕ\������
	//! \param[in]	_str	�\�����镶����
	//! \param[in]	_x		�\�����镶�����x���W
	//! \param[in]	_y		�\�����镶�����y���W
	/*============================================================*/
	void DrawString( const std::string _str , const int _x , const int _y );


	/*==========================================================*/
	//! \brief ��ʐݒ�
	//! \param[in]	_fovy	����p
	//! \param[in]	_asp	�A�X�y�N�g��
	//! \param[in]	_zn		Z�o�b�t�@��0.0f�̈ʒu
	//! \param[in]	_zf		Z�o�b�t�@��1.0f�̈ʒu
	/*==========================================================*/
	void SetProj( const float _fovy = 45.0f , const float _asp = 4.0f / 3.0f , const float _zn = 1.0f , const float _zf = 1000.0f );


	/*================================*/
	//! \brief �J�����̍X�V
	/*================================*/
	void SetCamera( void );


	/*===============================================*/
	//! \brief �s����Z�b�g����
	//! \param[in] *_lpmwt	�Z�b�g����s��
	/*===============================================*/
	inline void SetWorldTrans( const D3DXMATRIX *_lpmwt ){ m_lpD3DDevice->SetTransform( D3DTS_WORLD , _lpmwt ); }
	

	/*=================================*/
	//! \brief D3DDevice��Ԃ�
	/*=================================*/
	inline LPDIRECT3DDEVICE9 GetDevice( void ){ return m_lpD3DDevice; }


	/*=======================================================*/
	//! \brief �J�������w����W�ɃZ�b�g����
	//! \param[in]	_v	�J�������Z�b�g������W
	/*=======================================================*/
	inline void SetCamPos( const D3DXVECTOR3 _v ){ m_CamPos = _v; }

	/*=========================================*/
	//! \brief �J�����̍��W��Ԃ�
	/*=========================================*/
	inline D3DXVECTOR3 GetCamPos( void ){ return m_CamPos; }


	/*=============================================================*/
	//! \brief �J�����̃Y�[���A�������Z�b�g����
	//! \param[in]	_Zoom	�J�������ǂꂾ���ߊ�邩
	/*=============================================================*/
	inline void SetZoom( const float _Zoom ){ m_CamZoom = _Zoom; }


	/*=======================================================*/
	//! \brief �J�������w�肵�������֌�����
	//! \param[in]	_v		�J�����̉�]�p
	/*=======================================================*/
	inline void SetCamRot( const D3DXVECTOR3 _v ){ m_CamRx = _v.x; m_CamRy = _v.y; m_CamRz = _v.z; }

	/*=========================================*/
	//! \brief �J�����̊p�x��Ԃ�
	/*=========================================*/
	inline D3DXVECTOR3 GetCamRot( void ){ return D3DXVECTOR3( m_CamRx , m_CamRy , m_CamRz ); }


	/*==============================================================*/
	//! \brief ���b�V����`�悷�邩�̃t���O�𔽓]����
	/*==============================================================*/
	inline void NotDrawDirectX( void )
	{
#ifdef _DEBUG
		m_DrawDirectX = !m_DrawDirectX;
#else
		m_DrawDirectX = true;
#endif
	}


	/*=========================================*/
	//! \brief �`��t���O��Ԃ�
	/*=========================================*/
	inline bool GetDrawFlag( void ){ return m_DrawDirectX; }


	/*=================================================*/
	//! \brief CSprite��Ԃ�
	/*=================================================*/
	inline CSprite* GetCSprite( void ) const { return m_Sprite; }


	/*=================================================*/
	//! \brief CLightManager��Ԃ�
	/*=================================================*/
	inline CLightManager* GetCLightManager( void ) const { return m_LightManager; }


	/*================================================================================*/
	//! \brief �E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��o�b�t�@�T�C�Y��ύX����
	//! \param[in]	_WindowSize		�E�B���h�E�̃T�C�Y
	/*================================================================================*/
	inline void SetBackBufferSize( const SIZE _WindowSize )
	{
		//m_param.BackBufferWidth = _WindowSize.cx;
		//m_param.BackBufferHeight = _WindowSize.cy;
	}


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CDirect3D( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CDirect3D( const CDirect3D& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CDirect3D& operator = ( const CDirect3D& single );


	//! \brief �C���X�^���X�̃|�C���^
	static CDirect3D *m_pInstance;				//!< DirectX�̃|�C���^

	// 3D�p�̕ϐ��錾
	LPDIRECT3D9				m_lpD3D;			//!< Direct3D�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9		m_lpD3DDevice;		//!< Direct3DDevice�C���^�[�t�F�C�X
	LPDIRECT3DSURFACE9		m_lpBackbuffer;		//!< �o�b�N�o�b�t�@
	int						m_adapter;			//!< �A�_�v�^�ԍ����i�[
	D3DPRESENT_PARAMETERS	m_param;			//!< �v���[���e�[�V�����p�����[�^
	LPD3DXFONT				m_pFont;			//!< �t�H���g�\���p
	CSprite					*m_Sprite;			//!< �X�v���C�g�N���X�̃|�C���^
	CLightManager			*m_LightManager;	//!< ���C�g���Ǘ�

	// �J�����p�̕ϐ��錾
	float m_CamRx;								//!< �����_����̃J�����̉�]Y��
	float m_CamRy;								//!< �����_����̃J�����̉�]Y��
	float m_CamZoom;							//!< �����_�ƃJ�����̋���
	float m_CamRz;								//!< �J�����̓���̊p�x
	D3DXVECTOR3 m_CamPos;						//!< �����_�̈ʒu
    CMatrix m_mViewSrc;							//!< �����_�̍s��
	CMatrix m_mProj;							//!< ������W�n�p�[�X�y�N�e�B�u�ˉe�s��

	bool m_DrawDirectX;							//!< DirectX�̕`��t���O

};

/*=========================================*/
//! \def DIRECTX
//! \brief �R�[�����Ɏg�p����
/*=========================================*/
#define DIRECTX CDirect3D::GetInstance()