#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "CTitle.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CTitle::CTitle( void )
	: m_lpTitleTexture( nullptr )
	, m_lpPPushSpaceTexture( nullptr )
	, m_pSprite( nullptr )
{
	m_pSprite = m_pd3d->GetCSprite();
	
	m_lpTitleTexture = new CTexture;
	m_lpTitleTexture->Load( m_pd3d->GetDevice() , "Resource/Texture/Logo.png" );

	m_lpPPushSpaceTexture = new CTexture;
	m_lpPPushSpaceTexture->Load( m_pd3d->GetDevice() , "Resource/Texture/PushSpace.png" );

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CTitle::~CTitle( void )
{
	SafeDelete( VARIABLE_DATA( m_lpTitleTexture ) );
	SafeDelete( VARIABLE_DATA( m_lpPPushSpaceTexture ) );
}


/*===========================================*/
//! \brief �X�V
//! \return �V�[���̕ύX = true
//! \return �p�� �@�@�@�@= false
/*===========================================*/
bool CTitle::Updata( void )
{

	// �G�X�P�[�v�L�[�ŃQ�[�����I������
	if( m_pKeyboard->GetState( CKeyboard::KEY_ESCAPE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::END );
		return true;
	}

	// �Q�[���V�[���ֈڍs����
	if( m_pKeyboard->GetState( CKeyboard::KEY_SPACE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::GAME );
		return true;
	}

#ifdef _DEBUG
	// �����쐬�V�[���ֈڍs����
	//if( m_pKeyboard->GetState( CKeyboard::KEY_RETURN ) == CKeyboard::KST_PUSH )
	//{
	//	m_pSceneManager->ChangeScene( CSceneManager::ROOM_CREATE );
	//	return true;
	//}
#endif

	return false;

}


/*=================================*/
//! \brief ��ʕ\��
/*=================================*/
void CTitle::Draw( void )
{

	// �e�N�X�`���̃T�C�Y���擾//////////////////
	D3DSURFACE_DESC LogoTextureDesc;
	if( FAILED( m_lpTitleTexture->GetTexture()->GetLevelDesc( 0 , &LogoTextureDesc ) ) )
	{
		LogoTextureDesc.Width  = 0;
		LogoTextureDesc.Height = 0;
	}
	RECT LogoTextureRect = { 0 , 0 , LogoTextureDesc.Width , LogoTextureDesc.Height };
	
	D3DSURFACE_DESC PushSpaceTextureDesc;
	if( FAILED( m_lpPPushSpaceTexture->GetTexture()->GetLevelDesc( 0 , &PushSpaceTextureDesc ) ) )
	{
		PushSpaceTextureDesc.Width  = 0;
		PushSpaceTextureDesc.Height = 0;
	}
	RECT PushSpaceTextureRect = { 0 , 0 , PushSpaceTextureDesc.Width , PushSpaceTextureDesc.Height };
	/////////////////////////////////////////////


	// 3D�̕`��
	m_pd3d->Begin();

	m_pd3d->SetProj();
	m_pd3d->SetCamera();

	m_pd3d->GetDevice()->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 1.0f , 0 );

	
	// �X�v���C�g�`��J�n
	m_pSprite->Begin();
	
	// �ړ��s��
	CMatrix mTrans;

	// �ړ��s��Ƀ��S�̍��W���Z�b�g
	mTrans.SetTrans( D3DXVECTOR3( (float)(WINDOW_WIDTH/2-(LogoTextureDesc.Width*1.2f)/2) , (float)(WINDOW_HEIGHT/2-(LogoTextureDesc.Height*1.2f)/2)-50.0f , 0.0f ) );
		
	// �g�嗦�̃Z�b�g
	CMatrix mScale;
	mScale.SetScale( D3DXVECTOR3( 1.2f , 1.2f , 0.0f ) );

	// �s��̓K�p
	CMatrix mWorld;
	mWorld = mScale * mTrans;
	m_pd3d->SetWorldTrans( &mWorld );

	// ���S�}�[�N�̕\��
	m_pSprite->GetSprite()->Draw( m_lpTitleTexture->GetTexture() ,
					&LogoTextureRect,
					&D3DXVECTOR3( 0.0f, 0.0f , 0.0f ),
					NULL,
					D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) );
	
	// �X�v���C�g�`��I��
	m_pSprite->End();

	// �X�v���C�g�`��J�n
	m_pSprite->Begin();

	// �ړ���
	D3DXVECTOR3 BasePos = D3DXVECTOR3( 310.0f , 440.0f , 0.0f );

	// �ړ��s��Ɉړ��ʂ��Z�b�g
	mTrans.SetTrans( BasePos );

	

	// �s��̓K�p
	mWorld = mTrans;
	m_pd3d->SetWorldTrans( &mWorld );

	m_pSprite->GetSprite()->Draw( m_lpPPushSpaceTexture->GetTexture() ,
						&PushSpaceTextureRect,
						&D3DXVECTOR3( 0.0f, 0.0f , 0.0f ),
						NULL,
						D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) );

	// �X�v���C�g�`��I��
	m_pSprite->End();

	// �`��I��
	m_pd3d->End();

}


