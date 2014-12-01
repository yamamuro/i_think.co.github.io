#pragma once

#include "../Object/ObjectManager/CObjectManager.hpp"
#include "Direct3D/CDirect3D.hpp"
#include "PhysX/CPhysX.hpp"
#include "XFile/CXFile.hpp"

class CObjectBase
{

	/*===================================================*/
	//!
	//! \file
	//! \class CObjectBase
	//! \brief <b>オブジェクトの基底クラス</b>
	//!
	//! \author 配布データ+Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/10
	//!
	/*===================================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CObjectBase( void );

	/*==========================================================================================*/
	//! \brief 初期化
	//! \param[in]	_Data		オブジェクトの初期化に使用するデータ
	/*==========================================================================================*/
	virtual void Init( CObjectManager::OBJECT_DATA _Data );
	
	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	virtual void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	virtual ~CObjectBase( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	virtual void Exec( void );


	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	/*=============================================*/
	virtual void Draw( CDirect3D &_d3d );


	/*==================================*/
	//! \brief 行列を返す
	/*==================================*/
	CMatrix GetMatrix( void ); 


	/*==================================*/
	//! \brief 座標を返す
	/*==================================*/
	inline D3DXVECTOR3 GetPos( void ){ return m_vPos; }

	/*============================================*/
	//! \brief 座標をセットする
	//! \param[in]	_v		セットする座標
	/*============================================*/
	inline void SetPos( const D3DXVECTOR3 _v ){ m_vPos = _v; }


	/*=================================================*/
	//! \brief アクターの座標を更新する
	//! \param[in]	_v		セットする座標
	/*=================================================*/
	inline void SetActorPos( const NxVec3 _v ){ m_Actor->setGlobalPosition( _v ); }


	/*==================================*/
	//! \brief 現在の角度を返す
	/*==================================*/
	inline D3DXVECTOR3 GetRot( void ){ return m_vRot; }


	/*============================================*/
	//! \brief 角度をセットする
	//! \param[in]	_v		セットする角度
	/*============================================*/
	inline void SetRot( const D3DXVECTOR3 _v ){ m_vRot = _v; }

	/*============================================*/
	//! \brief 角度を追加する
	//! \param[in]	_v		追加する角度
	/*============================================*/
	inline void AddRot( const D3DXVECTOR3 _v ){ m_vRot += _v; }


	/*=============================================================*/
	//! \brief アクターをセットする
	//! \param[in]	*_Actor		セットするアクターのポインタ
	/*===========================================================s==*/
	inline void SetActor( NxActor* _Actor ){ m_Actor = _Actor; }

	/*===================================*/
	//! \brief アクターを返す
	/*===================================*/
	inline NxActor* GetActor( void ){ return m_Actor; }


	/*========================================*/
	//! \brief アクターデータを返す
	/*========================================*/
	ACTOR_DATA* GetActorData( void ){ return &m_ActorData; }


	/*=============================*/
	//! \brief XFileを返す
	/*=============================*/
	inline CXFile* GetXFile( void ){ return m_lpXFile; }


	/*===========================================*/
	//! \brief オブジェクト名を返す
	/*===========================================*/
	inline std::string GetName( void ){ return Name; }


	/*==========================================*/
	//! \brief 描画を後回しにするか返す
	/*==========================================*/
	inline bool GetDefermentFlag( void ){ return m_DrawFlag; }


	/*==================================================*/
	//! \brief レーザーに当たっているかをセット
	/*==================================================*/
	inline void SetLaserHitFlag( const bool _Flag ){ m_LaserHitFlag = _Flag; }


	/*================================================*/
	//! \brief 浮遊中かどうか返す
	//! \return 浮遊中 = true
	//! \retrun 浮遊していない = false
	/*================================================*/
	inline bool GetAntiGravityFlag( void ){ return m_AntiGravity_Energy != 0 ? 1 : 0; }


	/*======================================*/
	//! \brief 浮遊キャンセル
	/*======================================*/
	inline virtual void ResetAntiGravity( void ){ m_AntiGravity_Energy = 0; }


	/*==================================================*/
	//! \brief コリジョングループを返す
	/*==================================================*/
	inline int GetCollisionGroup( void ){ return m_CollisionGroup; }

	/*==================================================*/
	//! \brief コリジョングループをセットする
	/*==================================================*/
	inline void SetCollisionGroup( int _Group ){ m_CollisionGroup = _Group; }


protected:

	/*===================================================*/
	//! \brief 浮遊時間の加算
	//! \param[in]	_Power	加算するエネルギー量
	/*===================================================*/
	inline void AddAntiGravity_Energy( const int _Power )
	{ 
		if( m_LaserHitFlag )
		{
			m_AntiGravity_Energy += _Power;
			if( m_AntiGravity_Energy > MaxEnergy ){ m_AntiGravity_Energy = MaxEnergy; }
		}
	}

	/*=========================================================*/
	//! \brief 消費するエネルギー処理
	//! \param[in]	_ReducePower	減らすエネルギー量
	/*=========================================================*/
	inline void SubAntiGravity_Energy( const int _ReducePower )
	{
		m_AntiGravity_Energy -= _ReducePower;
		if( m_AntiGravity_Energy < 0 ){ m_AntiGravity_Energy = 0; }
	}


	std::string Name;				//!< オブジェクトの名前

	CDirect3D *m_pd3d;				//!< CDirect3Dのポインタ
	CPhysX *m_pPhysX;				//!< CPhysXのポインタ

	std::string m_FilePass;			//!< ファイルの相対パスを指定
	CXFile *m_lpXFile;				//!< メッシュデータ
	ACTOR_DATA  m_ActorData;		//!< アクターを構成する情報	
	NxActor *m_Actor;				//!< アクター本体
	NxMaterial *m_NxMaterial;		//!< アクターのマテリアル　どんな材質なのか
	
	D3DXVECTOR3 m_vPos;				//!< オブジェクトの位置
	D3DXVECTOR3 m_vRot;				//!< オブジェクトの角度
	D3DXVECTOR3 m_vScaling;			//!< オブジェクトの拡大率

	bool m_DrawFlag;				//!< 描画を後に行うかのフラグ

	static const int MaxEnergy = 1000;
	bool m_LaserHitFlag;			//!< レーザーに当たっているかのフラグ
	int m_AntiGravity_Energy;		//!< アンチグラビティーのエネルギー量

	D3DMATERIAL9 *m_Material;		//!< マテリアル情報を格納

	int m_CollisionGroup;			//!< コリジョングループ

};