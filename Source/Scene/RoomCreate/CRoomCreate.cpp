#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"
#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"
#include "CRoomCreate.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CRoomCreate::CRoomCreate( void )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CRoomCreate::~CRoomCreate( void )
{
}


/*===========================================*/
//! \brief 更新
//! \return シーンの変更 = true
//! \return 継続 　　　　= false
/*===========================================*/
bool CRoomCreate::Updata( void )
{
	
	// エスケープキーでタイトル画面に戻る
	if( m_pKeyboard->GetState( CKeyboard::KEY_ESCAPE ) == CKeyboard::KST_PUSH )
	{
		m_pSceneManager->ChangeScene( CSceneManager::TITLE );
		return true;
	}


	m_pObjectManager->Updata();


	return false;

}


/*=================================*/
//! \brief 画面表示
/*=================================*/
void CRoomCreate::Draw( void )
{

	// 描画準備
	LPDIRECT3DDEVICE9 Direct3D = m_pd3d->Begin();
	m_pd3d->SetProj();
	m_pd3d->SetCamera();

	m_pObjectManager->Draw();

	// デバッグ描画
	if( m_pPhysX->GetDebugRender() != nullptr && m_pPhysX->GetPhysXDrawFlag() )
	{
		m_pPhysX->GetDebugRender()->RenderData( m_pd3d->GetDevice(), m_pPhysX->GetScene()->getDebugRenderable() );
	}	
	// FPSを表示する
	if( m_pDebug != nullptr ){ m_pDebug->DrawFPS(); }
	
	m_pd3d->End();

}