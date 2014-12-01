#include "stdafx.hpp"

#include "../LaserBase/CLaserBase.hpp"
#include "../AntiGravity/CAntiGravityLaser.hpp"
#include "../Holding/CHoldingLaser.hpp"
#include "../AttractionAndRepulsionLaser/CAttractionAndRepulsionLaser.hpp"

#include "../Particle/CParticle.hpp"
#include "../Particle/CParticleEmitter.hpp"
#include "CLaserManager.hpp"


/*===============================================*/
//! \brief コンストラクタ
/*===============================================*/
CLaserManager::CLaserManager( CPlayer *_pPlayer )
	: m_pMouse( MOUSE )
	, m_UsingLaserType( ANTI_GRAVITY )
	, m_pUsingLaser( nullptr )
{

	// 各レーザーのポインタを格納
	m_pLasers[ANTI_GRAVITY]				= new CAntiGravityLaser( _pPlayer );
	m_pLasers[HOLDING]					= new CHoldingLaser( _pPlayer );
	m_pLasers[ATTRACTION_AND_REPULSION] = new CAttractionAndRepulsionLaser( _pPlayer );

	// 各レーザーの色
	m_LaserGunColor[ANTI_GRAVITY]			  = LASER_COLOR( 0.0f , 0.0f , 1.0f );
	m_LaserGunColor[HOLDING]				  = LASER_COLOR( 0.0f , 1.0f , 0.0f );
	m_LaserGunColor[ATTRACTION_AND_REPULSION] = LASER_COLOR( 0.0f , 0.0f , 0.0f );


	// 使用するレーザーのポインタを格納
	m_pUsingLaser = m_pLasers[m_UsingLaserType];
	// 使用するレーザーの色を格納
	m_Color = m_LaserGunColor[m_UsingLaserType];

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLaserManager::~CLaserManager( void )
{
	for( auto &x : m_pLasers )
		SafeDelete( x );
}


/*================================*/
//! \brief 更新
/*================================*/
void CLaserManager::Exec( void )
{

	// レーザー銃の切り替えの確認、切り替え処理
	ChangeLaserType();

	// 現在使用しているレーザー銃の更新
	m_pUsingLaser->Exec();

	// パーティクルを移動させる
	LASER_COLOR Dummy;
	m_pUsingLaser->ParticleUpdate( Dummy , CLaserBase::ParticleUpdateFlag );
}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CLaserManager::Draw( CDirect3D & _d3d )
{
	m_pUsingLaser->Draw( _d3d , m_Color );
}


/*===========================================*/
//! \brief レーザーの切り替え処理
/*===========================================*/
void CLaserManager::ChangeLaserType( void )
{

	int MoveMent = m_pMouse->GetWheelMovement();

	// マウスのホイールが移動したか確認
	if( MoveMent )
	{
		m_pUsingLaser->SetLightFlag( FALSE );
		// レーザー銃のタイプ切り替え
		m_UsingLaserType = ( m_UsingLaserType + MoveMent ) % MAX_LASER_GUN;
		if( m_UsingLaserType < 0 ){ m_UsingLaserType += MAX_LASER_GUN; }
		// 対応するレーザー銃をセット
		m_pUsingLaser = m_pLasers[m_UsingLaserType];

	}

	// レーザー銃の色を切り替えていく
	ChangeingLaserColor();

}


/*=================================================*/
//! \brief レーザーの色を変更する
/*=================================================*/
void CLaserManager::ChangeingLaserColor( void )
{

	static const int MaxCount = 30;				// 色の変更にかかるフレーム数
	static int Count = 0;						// 色の変更時に使用するカウンタ
	static int BeforeLaserType = ANTI_GRAVITY;	// 切り替わる前のレーザー銃のタイプ
	static LASER_COLOR AddColor;				// マイフレームどれだけ色を加算していくか

	// 使用しているレーザー銃が前フレームと違うか調べる
	if( BeforeLaserType != m_UsingLaserType )
	{ 
		
		Count = 0;
		m_Color = m_LaserGunColor[BeforeLaserType];

		AddColor = ( m_LaserGunColor[m_UsingLaserType] - m_LaserGunColor[BeforeLaserType] ) / (float)MaxCount;

		Count++;

		// レーザータイプを更新
		BeforeLaserType = m_UsingLaserType;
	}

	if( Count == 0 ){ return; }

	if( Count == MaxCount )
	{
		Count = 0;
		m_Color = m_LaserGunColor[m_UsingLaserType];
		return;
	}

	Count++;
	m_Color += AddColor;

}
