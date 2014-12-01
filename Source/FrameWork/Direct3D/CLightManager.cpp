#include "stdafx.hpp"

#include "CDirect3D.hpp" 
#include "CLightManager.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CLightManager::CLightManager( void )
	: m_pd3d( nullptr )
	, m_LightNum( 0 )
{
	m_pd3d = DIRECTX;
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLightManager::~CLightManager( void )
{
}


/*===========================================================*/
//! \brief ライトを追加する
//! \param[in]	_Light		追加するライト
//! \param[in]	_Flag		ライトのON,OFF
//! \return インデックス番号
/*===========================================================*/
int CLightManager::AddLight( D3DLIGHT9 _Light , BOOL _Flag )
{
	m_LightList.push_back( _Light );
	m_pd3d->GetDevice()->SetLight( m_LightNum , &m_LightList[m_LightNum] );
	m_pd3d->GetDevice()->LightEnable( m_LightNum , _Flag );
	// 次のライトのためにインデックス番号を更新
	return m_LightNum++;
}


/*============================================================*/
//! \brief ライトの状態を更新する
//! \param[in]	_Light		更新するライト
//! \param[in]	_Index		更新するライトのインデックス番号
//! \param[in]	_Flag		ライトのON,OFF
/*============================================================*/
void CLightManager::UpdateLight( D3DLIGHT9 _Light , int _Index , BOOL _Flag )
{
	if( _Index < (int)m_LightList.size() && _Index >= 0 )
	{
		m_LightList[_Index] = _Light;
		m_pd3d->GetDevice()->SetLight( _Index , &m_LightList[_Index] );
		m_pd3d->GetDevice()->LightEnable( _Index , _Flag );
	}
}


/*===============================================================*/
//! \brief ライトの設定を変更する
//! \param[in]	_Index		変更するライトのインデックス番号
//! \param[in]	_Flag		ライトのON,OFF
/*===============================================================*/
void CLightManager::SetLightFlag( DWORD _Index , BOOL _Flag )
{
	if( _Index < (int)m_LightList.size() && _Index >= 0 )
	{
		m_pd3d->GetDevice()->LightEnable( _Index , _Flag );
	}
}


/*==============================================================*/
//! \brief すべてのライトの設定を変更する
//! \param[in]	_Flag		ライトのON,OFF
/*==============================================================*/
void CLightManager::SetAllLightFlag( BOOL _Flag )
{
	for( int i=0 ; i<m_LightNum ; i++ )
		m_pd3d->GetDevice()->LightEnable( i , FALSE );
}


/*========================================*/
//! \brief ライトを全て消す
/*========================================*/
void CLightManager::AllDelete( void )
{
	// ライトを全てOFFにする
	SetAllLightFlag( FALSE );

	m_LightList.clear();

	// インデックスの初期化
	m_LightNum = 0;
}