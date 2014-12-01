#pragma once

/*===================================*/
//!< 前方宣言
/*===================================*/
class CPlayer;
class CLaser;
class CParticleEmitter;

#include "../../../ObjectBase/CObjectBase.hpp"
#include "../LaserManager/CLaserManager.hpp"


class CLaserBase : public CObjectBase
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CLaserBase
	//! \brief <b>レーザー銃の基底クラス</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/26
	//! \since 2014/11/26
	//!
	/*=========================================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CLaserBase( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CLaserBase( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	virtual void Exec( void ) = 0;


	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	//! \param[in]	_Color	レーザーの色
	/*=============================================*/
	void Draw( CDirect3D & _d3d , LASER_COLOR _Color );


	static const bool AddParticleFlag = false;		//!< パーティクルを追加、削除する時のフラグ
	static const bool ParticleUpdateFlag = true;	//!< パーティクルの更新時に使用するフラグ
	/*===================================================*/
	//! \brief パーティクルの更新
	//! \param[in]	_Color		パーティクルの色
	//! \param[in]	_Flag		更新の種類
	//! \param[in]	_deltaTime	更新する時間
	/*===================================================*/
	void ParticleUpdate( LASER_COLOR _Color , bool _Flag , NxReal _DeltaTime = 1/(float)FRAME_RATE );

	/*============================================*/
	//! \brief レーザー部分の更新
	/*============================================*/
	void LaserUpdate( void );


	/*================================*/
	//! ライトのセッタ
	/*================================*/
	void SetLightFlag( BOOL _Flag ){ m_pd3d->GetCLightManager()->UpdateLight( m_Light , m_LightIndex , _Flag ); }


protected:

	/*===========================================================*/
	//! \brief レイの当たり判定
	//! \param[out]	*Distance	オブジェクトまでの距離
	//! \return ヒット = true
	//! \return ヒットしなかった = false
	/*===========================================================*/
	bool CLaserBase::HitCheck( float *Distance );


	/*==============================================*/
	//! \brief 左クリック時のアクション
	/*==============================================*/
	virtual void LeftClickAction( void ) = 0;

	/*==============================================*/
	//! \brief 右クリック時のアクション
	/*==============================================*/
	virtual void RightClickAction( void ) = 0;


	NxActor *m_pHitActor;		//!< レーザーの当たったアクターのポインタ
	CObjectBase *m_pHitObject;	//!< レーザーの当たったオブジェクトのポインタ

	static CPlayer *m_pPlayer;	//!< プレイヤーのポインタ

	CMouse *m_pMouse;			//!< CMouseのポインタ

	CRaycastLine RayPos;		//!< レイの初期位置と終点を格納

	CXFile *m_lpAGLEffect;		//!< AGL本体の起動時エフェクト

	D3DLIGHT9 m_Light;			//!< レーザーの当たった地点を照らすライト
	int m_LightIndex;			//!< ライトのインデックス番号


	static CParticleEmitter *m_pParticleEmitter;	//!< 当たった場所に飛び散るパーティクルエフェクト
	CLaser *m_pLaser;			//!< レーザーのポインタ

	bool m_LaserReverseFlag;	//!< レーザーを逆流するアニメーションにするかのフラグ
	
};