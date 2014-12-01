#include "stdafx.hpp"

#include "CDirect3D.hpp"
#include "CSprite.hpp"


/*==============================================*/
//! CSpriteのポインタの定義
/*==============================================*/
LPD3DXSPRITE CSprite::m_lpSprite = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CSprite::CSprite( void )
{
	//スプライトの初期化
	D3DXCreateSprite( DIRECTX->GetDevice() , &m_lpSprite );
	m_lpSprite->OnResetDevice();
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CSprite::~CSprite( void )
{
	SafeRelease( VARIABLE_DATA( m_lpSprite ) );
}


/*=========================================================*/
//! \brief スプライトの描画開始処理
//! \brief 描画終了時にはEnd()をコールする
/*=========================================================*/
void CSprite::Begin( void )
{
	if( m_lpSprite != nullptr )
		m_lpSprite->Begin( D3DXSPRITE_ALPHABLEND );
}


/*=========================================================*/
//! \brief スプライトの描画終了処理
//! \brief 描画開始時にはBegin()をコールする
/*=========================================================*/
void CSprite::End( void )
{
	m_lpSprite->End();
}


/*========================================================================*/
//! \brief テクスチャの読み込み
//! \note テクスチャマネージャを作成したらそちらに移動させる
//! \param[out]	_lpTex		テクスチャのポインタを格納する
//! \param[in]	_FilePass	ファイル名を指定
//! \param[in]	_Width		ピクセル単位でテクスチャの幅を指定
//! \param[in]	_Height		ピクセル単位でテクスチャの高さを指定
//! \param[in]	_Color		透明にする色の指定
/*========================================================================*/
void CSprite::LoadText( LPDIRECT3DTEXTURE9 *_lpTex , std::string _FilePass , int _Width , int _Height , D3DCOLOR _Color )
{

	if( _Width  == 0 ){ _Width = D3DX_DEFAULT;  }
	if( _Height == 0 ){ _Height = D3DX_DEFAULT; }

	D3DXCreateTextureFromFileEx(
		DIRECTX->GetDevice() ,		// デバイスのポインタ
		_FilePass.c_str() ,			// ファイルパス
		_Width , _Height ,			// 幅、高さ
		1 ,							// ミップ レベルの数
		0 ,							// レンダリング ターゲット
		D3DFMT_UNKNOWN ,			// テクスチャに対して要求されたピクセル フォーマット
		D3DPOOL_MANAGED ,			// テクスチャの配置先となるメモリクラス
		D3DX_FILTER_NONE ,			// イメージをフィルタリングする方法
		D3DX_DEFAULT ,				// イメージをフィルタリングする方法
		_Color ,					// 透明にするカラーキー
		NULL ,						// D3DXIMAGE_INFO構造体のポインタ
		NULL ,						// PALETTEENTRY構造体のポインタ
		_lpTex );

}