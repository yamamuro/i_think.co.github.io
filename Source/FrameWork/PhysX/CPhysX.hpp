#pragma once

#include "Debug\CDebugRender.hpp"
#include "Wrapper\Matrix\CMatrix.hpp"
#include "Direct3D\CDirect3D.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"

/*===================================*/
//!< 前方宣言
/*===================================*/
class ContactCallBack;
class MyJointNotify;


/*================================================================*/
//! \struct CUSTOMVERTEX
//! \brief <b>デバッグ時に表示するワイヤーフレームの頂点情報</b>
/*================================================================*/
typedef struct CUSTOMVERTEX
{
    FLOAT x, y, z;
}CUSTOMVERTEX;


/*================================================================*/
//! \struct MATERIAL_DATA
//! \brief <b>アクターのマテリアルを作成する際に使用</b>
/*================================================================*/
typedef struct MATERIAL_DATA
{

	NxMaterialDesc MaterialDesc;	//!< マテリアル情報(別のアクターとの衝突時の計算などを設定する)
	bool isInit;

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	MATERIAL_DATA( void )
		: isInit( false )
	{
		MaterialDesc.setToDefault();
	}

}MATERIAL_DATA;


/*============================================*/
//! \struct ACTOR_DATA
//! \brief <b>アクターのデータを管理</b>
//! \brief 主に初期化の際に使用
/*============================================*/
typedef struct ACTOR_DATA
{

	char Type;						//!< アクターの型( B , S , C , P , G )
	int ObjectName;					//!< CObjectManagerで定義している定数
	NxVec3 Dimension;				//!< ボックスの寸法
	float Radius;					//!< 球の半径
	float Height;					//!< カプセルの胴長
	bool FixFlag;					//!< アクターを固定するか剛体にするかのフラグ
	NxReal AngularDamping;			//!< 回転による衰退係数 転がりやすさ
	NxReal Density;					//!< 質量 重さ

	NxVec3 LocalPosition;			//!< ローカル座標
	NxMat33 LocalRotation;			//!< ローカルな姿勢
	NxVec3 Position;				//!< アクターの初期位置
	NxMat33 Rotation;				//!< アクターの初期姿勢

	NxVec3 MeshScale;				//!< メッシュの拡大縮小
	NxVec3 MeshTranslation;			//!< メッシュの平行移動
	D3DXMATRIX MeshRotation;		//!< メッシュの回転
	CObjectBase* pObject;			//!< 自身を管理するオブジェクトのポインタ

	NxU32 ContactPairFlag;			//!< アクターの衝突判定
	NxActor *pContactActor;			//!< 衝突したアクターのポインタ

	MATERIAL_DATA MatDesc;			//!< マテリアル
	NxReal SkinWidth;				//!< 他のアクターにめり込む量


	/*=====================================*/
	// デフォルトコンストラクタ
	/*=====================================*/
	ACTOR_DATA()
		: Type( '0' )
		, ObjectName( CObjectManager::OBJ_NONE )
		, Radius( 0.0f )
		, Height( 0.0f )
		, pObject( nullptr )
		, pContactActor( nullptr )
	{
		Dimension = NxVec3( 0.0f , 0.0f , 0.0f );
		LocalPosition = NxVec3( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 Vec = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
		CDPConverter::SetRot( &LocalRotation , &Vec );
		Position = NxVec3( 0.0f , 0.0f , 0.0f );
		MeshScale = NxVec3( 1.0f , 1.0f , 1.0f );
		MeshTranslation = NxVec3( 0.0f , 0.0f , 0.0f );
		D3DXMatrixIdentity( &MeshRotation );
		ContactPairFlag = 0;
		SkinWidth = -1;
	}

}ACTOR_DATA;


class CPhysX
{

	/*================================================*/
	//!
	//! \file
	//! \class CPhysX
	//! \brief <b>PhysXの管理</b>
	//!
	//! \author	okawa & Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/18
	//!
	/*================================================*/

public:


	/*=======================================================================================================*/
	//! \enum CollisionGroup
	//! \brief コリジョングループ
	//! \note 内部的にフラグ管理されるので | (or)を使用したコリジョンの複数判定も可能
	/*=======================================================================================================*/
	enum CollisionGroup
	{
		CG_DEFAULT,			//!< デフォルトのコリジョングループ
		CG_HEAVY_OBJECT,	//!< 主人公よりも重いオブジェクト
		CG_STATIC,			//!< 動くことのないオブジェクト( 部屋のアクターなど )
		CG_PLAYER,			//!< 主人公
		CG_THROUGH,			//!< レーザーがすり抜けるもの
		CG_PARTICLE			//!< パーティクル
	};
	// CollisionGroupの定数分シフトする
	static inline int BitShift( CollisionGroup _Bit ){ return ( 1 << _Bit ); }

	//!< CG_THROUGH,CG_PARTICLE以外
	static const unsigned int CG_OBJECT_ALL = ( CG_DEFAULT | CG_HEAVY_OBJECT | CG_STATIC | CG_PLAYER );


	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CPhysX* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CPhysX();
		}
		return m_pInstance;
	}


	/*==================================*/
	//! \brief 初期化
	//! \return 成功 = true 
	//! \return 失敗 = fasle
	/*==================================*/
	bool Initialize( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CPhysX( void );

	/*==================================*/
	//! \brief 終了処理
	/*==================================*/
	void Finalize( void );


	/*=================================================================*/
	//! \brirf アクターの動きをシーン上でシミュレーションする
	/*=================================================================*/
	void Simulation( void );


	/*=====================================================================*/
	//! \brief コリジョングループのセット
	//! \param[in]	*_Actor		セットするアクター
	//! \param[in]	_Group		セットするグループID
	/*=====================================================================*/
	void SetActorCollisionGroup( NxActor *_Actor , NxCollisionGroup _Group );


	/*==========================================================*/
	//! \brief コリジョングループの初期化
	/*==========================================================*/
	void InitActorCollisionGroups( void );


	/*====================================================*/
	//! \brief ユーザレポートの初期化
	/*====================================================*/
	void InitContactReportsPerPair( NxActor *_Actor );


	/*===============================================*/
	//! \brief アクターを削除する
	//! \param[in]	*_pActor	削除するアクター
	//! \note 未実装
	/*===============================================*/
	void DeleteActor( NxActor *_pActor );


	/*==============================================*/
	//! \brief 存在しているアクターを全て削除
	/*==============================================*/
	void AllDeleteActor( void );


	/*=========
	簡易のボックス作成
	======*/
	NxActor* CreateBox(const NxVec3& pos, const NxVec3& boxDim, const NxReal density);
	/* カプセル */
	NxActor* CreateCapsule( const NxVec3 &_Pos , const NxReal _Height , const NxReal _Radius , const NxReal _Density , BOOL flag , int _MaterialIndex = 0 );
	
	/*=======================================================================*/
	//! \brief アクターの追加
	//! \param[in]	*_pActorData			作成するアクターのデータ
	//! \return 成功 = 追加したアクター
	//! \return 失敗 = nullptr
	/*=======================================================================*/
	NxActor* CreateActor( ACTOR_DATA *_pActorData );


	/*======================================================================*/
	//! \brief メッシュからアクターを作成する
	//! \param[in]	_lpMesh		メッシュデータ
	//! \return 成功 = 追加したアクター
	//! \return 失敗 = nullptr
	/*======================================================================*/
	NxActor* CreateConvexActor( LPD3DXMESH _lpMesh );


	/*=====================================================================*/
	//! \brief ロープを作成する
	//! \param[out]		**_ropeSegs		ロープのアクターのポインタ
	//! \param[out]		**_ropeLinks	ロープのジョイント部分のポインタ
	//! \param[in]		_Pos			配置する座標
	//! \param[in]		_segHeight		1つのアクターの高さ
	//! \param[in]		_segRadius		1つのアクターの半径
	//! \param[in]		_nbSegments		いくつのアクターを繋ぐか
	//! \param[in]		_density		1つのアクターの密度
	//! \param[in]		_MaterialIndex	マテリアルのインデックス番号
	/*=====================================================================*/
	void CreateRope( NxActor **_ropeSegs , NxSphericalJoint **_ropeLinks ,  const NxVec3& _Pos , const NxReal _segHeight , const NxReal _segRadius , const int _nbSegments , const NxReal _density , int _MaterialIndex = 0 );


	/*================================================================*/
	//! \brief アクターをジョイントする
	//! \param[in]	*_a0			ジョイントするアクター
	//! \param[in]	*_a1			ジョイントするアクター
	//! \param[in]	&_globalAnchor	ジョイントする座標
	//! \param[in]	&_globalAxis	ジョイントする軸
	/*================================================================*/
	NxSphericalJoint* CreateRopeSphericalJoint( NxActor *_a0 , NxActor *_a1 , const NxVec3 &_globalAnchor , const NxVec3 &_globalAxis );

	NxRevoluteJoint* CreateRevoluteJoint(NxActor* a0, NxActor* a1, NxVec3 globalAnchor, NxVec3 globalAxis);
	
	/*============================================================*/
	//! \brief 2つのアクターをジョイントする
	//! \param[in]		ジョイントするアクター
	//! \param[in]		ジョイントするアクター
	//! \param[in]		ジョイントする座標
	//! \param[in]		ジョイントする軸
	/*============================================================*/
	NxFixedJoint* CPhysX::CreateFixedJoint( NxActor *_a0 , NxActor *_a1 , NxVec3 _globalAnchor , NxVec3 _globalAxis );


	/*======================================*/
	//! \brief シーンを返す
	/*======================================*/
	inline NxScene* GetScene( void ){ return m_pScene; }


	/*=============================================*/
	//! \brief デバッグ描画クラスを返す
	/*=============================================*/
	inline CDebugRender* GetDebugRender( void ){ return m_pDebugRender; }


	/*==========================================================*/
	//! \brief ジョイントのユーザレポートを返す
	/*==========================================================*/
	inline MyJointNotify* GetJointNotify( void ){ return m_JointNotify; }


	/*==========================================*/
	//! \brief デバッグデータのセット
	//! \return 成功 = S_OK
	//! \return 失敗 = E_FAIL
	/*==========================================*/
	HRESULT CreateDebugBase( void );


	/*===================================================================*/
	//! \brief ワイヤーフレームを描画するかのフラグを反転する
	/*===================================================================*/
	inline void NotDrawPhysX( void )
	{
#ifdef _DEBUG
		m_DrawPhysX = !m_DrawPhysX;
#else
		m_DrawPhysX = false;
#endif
	}


	/*============================================================*/
	//! \brief PhysXのデバッグ描画フラグを返す
	/*============================================================*/
	inline bool GetPhysXDrawFlag( void ){ return m_DrawPhysX; }


	/*============================================================*/
	//! \brief アクターをワイヤーフレームで描画する
	/*============================================================*/
	void RenderPhysX( void );


	/*============================================================*/
	//! \brief PhysX上のボックスをワイヤーフレームで描画する
	//!	\param[in]	*_Actor			描画するアクター
	//!	\param[in]	&_MatWorld		セットする行列
	/*============================================================*/
	void DrawBox( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld );

	/*============================================================*/
	//! \brief PhysX上の球をワイヤーフレームで描画する
	//!	\param[in]	*_Actor			描画するアクター
	//!	\param[in]	&_MatWorld		セットする行列
	/*============================================================*/
	void DrawSphere( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld );

	/*============================================================*/
	//! \brief PhysX上のカプセルをワイヤーフレームで描画する
	//!	\param[in]	*_Actor			描画するアクター
	//!	\param[in]	&_MatWorld		セットする行列
	/*============================================================*/
	void DrawCapsule( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld );

	/*============================================================*/
	//! \brief PhysX上の地面をワイヤーフレームで描画する
	/*============================================================*/
	void DrawGround( void ); 


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CPhysX( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CPhysX( const CPhysX& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CPhysX& operator = ( const CPhysX& single );


	static const int D3DFVF_CustomVertex = ( D3DFVF_XYZ );		//!< 頂点データの構成

	static const float SIMULATE_STEP;			//!< シーンの更新スピード

	//! \brief インスタンスのポインタ
	static CPhysX *m_pInstance;					//!< CPhysXのポインタ

	NxPhysicsSDK* m_pPhysicsSDK;				//!< PhysXのSDK
	NxScene*	m_pScene;						//!< シーンを格納
	CDebugRender* m_pDebugRender;				//!< デバッグ描画
	LPDIRECT3DVERTEXBUFFER9 m_lpVB;				//!< 頂点バッファ
	bool m_DrawPhysX;							//!< PhysXの描画フラグ

	static std::vector< NxActor* > m_Data;		//!< コリジョングループのリスト

	ContactCallBack *m_ContactCallBack;			//!< 衝突検知用のコールバッククラス
	MyJointNotify *m_JointNotify;				//!< ジョイントのコールバッククラス

};

/*================================*/
//! \def PHYSX
//! \brief コール時に使用する
/*================================*/
#define PHYSX CPhysX::GetInstance()



class ContactCallBack : public NxUserContactReport
{

	/*================================================*/
	//!
	//! \file
	//! \class ContactCallBack
	//! \brief <b>コンタクト(衝突)時のコールバッククラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/22
	//! \since 2014/10/24
	//!
	/*================================================*/


	/*==================================================================================*/
	//! \brief コールバック時に実行する関数
	//! \param[in]	&pair		衝突した2つのアクター
	//! \param[in]	events		衝突直後、衝突中、衝突終了のどれかが入る
	/*==================================================================================*/
	void onContactNotify( NxContactPair& pair, NxU32 events );
};


class MyJointNotify : public NxUserNotify
{
public:

	MyJointNotify( void )
		: toBreak( false )
	{
	}

	virtual void onWake(NxActor** actors, NxU32 count)
	{
		
	}
	virtual void onSleep(NxActor** actors, NxU32 count)
	{
		
	}

	virtual bool onJointBreak(NxReal breakingImpulse, NxJoint& brokenJoint)
	{
		return toBreak = true;
	}

	bool toBreak;

};