#include "stdafx.hpp"

#include "CDirect3D.hpp"
#include "CSprite.hpp"


/*==============================================*/
//! CSprite�̃|�C���^�̒�`
/*==============================================*/
LPD3DXSPRITE CSprite::m_lpSprite = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CSprite::CSprite( void )
{
	//�X�v���C�g�̏�����
	D3DXCreateSprite( DIRECTX->GetDevice() , &m_lpSprite );
	m_lpSprite->OnResetDevice();
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CSprite::~CSprite( void )
{
	SafeRelease( VARIABLE_DATA( m_lpSprite ) );
}


/*=========================================================*/
//! \brief �X�v���C�g�̕`��J�n����
//! \brief �`��I�����ɂ�End()���R�[������
/*=========================================================*/
void CSprite::Begin( void )
{
	if( m_lpSprite != nullptr )
		m_lpSprite->Begin( D3DXSPRITE_ALPHABLEND );
}


/*=========================================================*/
//! \brief �X�v���C�g�̕`��I������
//! \brief �`��J�n���ɂ�Begin()���R�[������
/*=========================================================*/
void CSprite::End( void )
{
	m_lpSprite->End();
}


/*========================================================================*/
//! \brief �e�N�X�`���̓ǂݍ���
//! \note �e�N�X�`���}�l�[�W�����쐬�����炻����Ɉړ�������
//! \param[out]	_lpTex		�e�N�X�`���̃|�C���^���i�[����
//! \param[in]	_FilePass	�t�@�C�������w��
//! \param[in]	_Width		�s�N�Z���P�ʂŃe�N�X�`���̕����w��
//! \param[in]	_Height		�s�N�Z���P�ʂŃe�N�X�`���̍������w��
//! \param[in]	_Color		�����ɂ���F�̎w��
/*========================================================================*/
void CSprite::LoadText( LPDIRECT3DTEXTURE9 *_lpTex , std::string _FilePass , int _Width , int _Height , D3DCOLOR _Color )
{

	if( _Width  == 0 ){ _Width = D3DX_DEFAULT;  }
	if( _Height == 0 ){ _Height = D3DX_DEFAULT; }

	D3DXCreateTextureFromFileEx(
		DIRECTX->GetDevice() ,		// �f�o�C�X�̃|�C���^
		_FilePass.c_str() ,			// �t�@�C���p�X
		_Width , _Height ,			// ���A����
		1 ,							// �~�b�v ���x���̐�
		0 ,							// �����_�����O �^�[�Q�b�g
		D3DFMT_UNKNOWN ,			// �e�N�X�`���ɑ΂��ėv�����ꂽ�s�N�Z�� �t�H�[�}�b�g
		D3DPOOL_MANAGED ,			// �e�N�X�`���̔z�u��ƂȂ郁�����N���X
		D3DX_FILTER_NONE ,			// �C���[�W���t�B���^�����O������@
		D3DX_DEFAULT ,				// �C���[�W���t�B���^�����O������@
		_Color ,					// �����ɂ���J���[�L�[
		NULL ,						// D3DXIMAGE_INFO�\���̂̃|�C���^
		NULL ,						// PALETTEENTRY�\���̂̃|�C���^
		_lpTex );

}