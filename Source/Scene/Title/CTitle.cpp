#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "CTitle.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
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
//! \brief デストラクタ
/*=====================================*/
CTitle::~CTitle( void )
{
	SafeDelete( VARIABLE_DATA( m_lpTitleTexture ) );
	SafeDelete( VARIABLE_DATA( m_lpPPushSpaceTexture ) );
}


/*===========================================*/
//! \brief 更新
//! \return シーンの変更 = true
//! \return 継続 　　　　= false
/*===========================================*/
bool CTitle::Updata( void )
{

	// エスケープキーでゲームを終了する
	if( m_pKeyboard->GetState( CKeyboard::KEY_ESCAPE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::END );
		return true;
	}

	// ゲームシーンへ移行する
	if( m_pKeyboard->GetState( CKeyboard::KEY_SPACE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::GAME );
		return true;
	}

#ifdef _DEBUG
	// 部屋作成シーンへ移行する
	//if( m_pKeyboard->GetState( CKeyboard::KEY_RETURN ) == CKeyboard::KST_PUSH )
	//{
	//	m_pSceneManager->ChangeScene( CSceneManager::ROOM_CREATE );
	//	return true;
	//}
#endif

	return false;

}


/*=================================*/
//! \brief 画面表示
/*=================================*/
void CTitle::Draw( void )
{

	// テクスチャのサイズを取得//////////////////
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


	// 3Dの描画
	m_pd3d->Begin();

	m_pd3d->SetProj();
	m_pd3d->SetCamera();

	m_pd3d->GetDevice()->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 255 , 255 , 255 ) , 1.0f , 0 );

	
	// スプライト描画開始
	m_pSprite->Begin();
	
	// 移動行列
	CMatrix mTrans;

	// 移動行列にロゴの座標をセット
	mTrans.SetTrans( D3DXVECTOR3( (float)(WINDOW_WIDTH/2-(LogoTextureDesc.Width*1.2f)/2) , (float)(WINDOW_HEIGHT/2-(LogoTextureDesc.Height*1.2f)/2)-50.0f , 0.0f ) );
		
	// 拡大率のセット
	CMatrix mScale;
	mScale.SetScale( D3DXVECTOR3( 1.2f , 1.2f , 0.0f ) );

	// 行列の適用
	CMatrix mWorld;
	mWorld = mScale * mTrans;
	m_pd3d->SetWorldTrans( &mWorld );

	// ロゴマークの表示
	m_pSprite->GetSprite()->Draw( m_lpTitleTexture->GetTexture() ,
					&LogoTextureRect,
					&D3DXVECTOR3( 0.0f, 0.0f , 0.0f ),
					NULL,
					D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) );
	
	// スプライト描画終了
	m_pSprite->End();

	// スプライト描画開始
	m_pSprite->Begin();

	// 移動量
	D3DXVECTOR3 BasePos = D3DXVECTOR3( 310.0f , 440.0f , 0.0f );

	// 移動行列に移動量をセット
	mTrans.SetTrans( BasePos );

	

	// 行列の適用
	mWorld = mTrans;
	m_pd3d->SetWorldTrans( &mWorld );

	m_pSprite->GetSprite()->Draw( m_lpPPushSpaceTexture->GetTexture() ,
						&PushSpaceTextureRect,
						&D3DXVECTOR3( 0.0f, 0.0f , 0.0f ),
						NULL,
						D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) );

	// スプライト描画終了
	m_pSprite->End();

	// 描画終了
	m_pd3d->End();

}


