#include "stdafx.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Particle/CParticleEmitter.hpp"
#include "../Object/Object/Laser/CLaser.hpp"
#include "../../Player/CPlayer.hpp"
#include "../LaserManager/CLaserManager.hpp"
#include "CLaserBase.hpp"

CPlayer* CLaserBase::m_pPlayer = nullptr;
CParticleEmitter* CLaserBase::m_pParticleEmitter = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CLaserBase::CLaserBase( void )
	: m_pMouse( MOUSE )
	, m_lpAGLEffect( nullptr )
	, m_pHitObject( nullptr )
	, m_pHitActor( nullptr )
	, m_LaserReverseFlag( false )
{
	
	// 本体のメッシュの読み込み
	m_lpXFile = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_FilePass + "Object/Player/AntiGravityLaser/AGL.x" );


	std::string AGLEffectPass = m_FilePass;
	AGLEffectPass += "Object/Player/AntiGravityLaser/AGLEffect.x";
	m_lpAGLEffect = XFILE_MANAGER->Load ( m_pd3d->GetDevice() , AGLEffectPass );
	m_Material = m_lpAGLEffect->GetMaterial();


	m_vPos = D3DXVECTOR3(  0.8f , -0.5f , 0.0f );
	m_vRot = D3DXVECTOR3( -3.0f , -4.0f , 0.0f );

	// パーティクルエフェクトの作成
	if( m_pParticleEmitter == nullptr )
		m_pParticleEmitter = new CParticleEmitter( m_pPhysX->GetScene() , 5.0f , NxVec3( 0.0f , 5.0f , 0.0f ) );
	// レーザー部分の作成
	m_pLaser = new CLaser();


	
	// ライトの初期化
	ZeroMemory( &m_Light , sizeof( D3DLIGHT9 ) );
	m_Light.Type = D3DLIGHT_POINT;									// ポイントライト

	m_Light.Diffuse  = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );		// ディフューズ色
	m_Light.Specular = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );		// スペキュラー色
	m_Light.Ambient  = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );		// アンビエント色
	
	m_Light.Position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );			// ライトの位置
	m_Light.Range = 25.0f;											// ライトの有効範囲
	m_Light.Attenuation0 = 0.0f;									// 減衰定数

	// ライトを追加する
   m_LightIndex = m_pd3d->GetCLightManager()->AddLight( m_Light , FALSE );

   // マテリアルの取得
   //m_Material = m_lpXFile->GetMaterial();

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLaserBase::~CLaserBase( void )
{
	SafeDelete( m_pParticleEmitter );
	SafeDelete( m_pLaser );
}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
//! \param[in]	_Color	レーザーの色
/*=============================================*/
void CLaserBase::Draw( CDirect3D & _d3d , LASER_COLOR _Color )
{


	bool PushFlag = false;
	if( m_pMouse->GetState( CMouse::B_LEFT  ) == CMouse::MST_HOLD ||
	  ( m_pMouse->GetState( CMouse::B_RIGHT ) == CMouse::MST_HOLD && ( this->GetName() == "AARL" || this->GetName() == "AGL" ) ) )
	{		 

		PushFlag = true;
		
		// レーザーの長さを調節する
		LaserUpdate();
		// レーザーのメッシュを取得
		CXFile **m_lpLaserEffect = m_pLaser->GetLaserXFile();

		// レーザー1の描画/////////////////////////////
		CMatrix mTrans;
		CMatrix mRot;
		CMatrix mWorld;
		CMatrix mParent;
		mParent = m_pPlayer->GetMatrix();
		//                              レーザーの発射位置の補正
		mTrans.SetTrans( m_vPos + D3DXVECTOR3( 0.077f , -0.097f , 2.0f ) );
		mWorld = mTrans * mParent;
		_d3d.SetWorldTrans( &mWorld );

		m_lpLaserEffect[0]->GetMaterial()->Diffuse.r = _Color.Red;
		m_lpLaserEffect[0]->GetMaterial()->Diffuse.g = _Color.Green;
		m_lpLaserEffect[0]->GetMaterial()->Diffuse.b = _Color.Blue;

		m_lpLaserEffect[0]->GetMaterial()->Ambient.r = _Color.Red;
		m_lpLaserEffect[0]->GetMaterial()->Ambient.g = _Color.Green;
		m_lpLaserEffect[0]->GetMaterial()->Ambient.b = _Color.Blue;

		m_lpLaserEffect[0]->GetMaterial()->Emissive.r = _Color.Red;
		m_lpLaserEffect[0]->GetMaterial()->Emissive.g = _Color.Green;
		m_lpLaserEffect[0]->GetMaterial()->Emissive.b = _Color.Blue;

		m_lpLaserEffect[0]->Draw( _d3d.GetDevice() );
		///////////////////////////////////////////////


		// レーザー2の描画////////////////////////////////////////////////////
		// テクスチャアニメ―ション=================
		m_pd3d->GetDevice()->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_COUNT2 );
		D3DXMATRIX mTex;
		D3DXMatrixIdentity( &mTex );
		// テクスチャの座標を移動する
		static float TextureAnimation = 1.0f;

		// レーザーを逆流する
		if( m_LaserReverseFlag ){ TextureAnimation += 0.017f; }
		// レーザーを吐き出す
		else{					  TextureAnimation -= 0.017f; }

		if( TextureAnimation < 0.0f ){ TextureAnimation += 1.0f; }
		if( TextureAnimation > 1.0f ){ TextureAnimation -= 1.0f; }

		mTex._31 = TextureAnimation;	//U
		mTex._32 = TextureAnimation/2;	//V
		m_pd3d->GetDevice()->SetTransform( D3DTS_TEXTURE0 , &mTex );
		m_pd3d->GetDevice()->SetTransform( D3DTS_WORLD , &mTrans );
		//==========================================

		// 行列を合成する
		mWorld = mTrans * mParent;
		_d3d.SetWorldTrans( &mWorld );

		m_lpLaserEffect[1]->GetMaterial()->Diffuse.r = _Color.Red;
		m_lpLaserEffect[1]->GetMaterial()->Diffuse.g = _Color.Green;
		m_lpLaserEffect[1]->GetMaterial()->Diffuse.b = _Color.Blue;

		m_lpLaserEffect[1]->GetMaterial()->Emissive.r = _Color.Red;
		m_lpLaserEffect[1]->GetMaterial()->Emissive.g = _Color.Green;
		m_lpLaserEffect[1]->GetMaterial()->Emissive.b = _Color.Blue;

		// 加算合成開始
		m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );
		// レーザーの描画
		m_lpLaserEffect[1]->Draw( _d3d.GetDevice() );
		// 加算合成終了
		m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
		
		// テクスチャアニメーションの設定を元に戻す
		m_pd3d->GetDevice()->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
		//////////////////////////////////////////////////////////////////////


		// ライトの更新//////////////////////////
		m_Light.Diffuse  = D3DXCOLOR( _Color.Red , _Color.Green , _Color.Blue , 1.0f );
		m_Light.Specular = D3DXCOLOR( _Color.Red , _Color.Green , _Color.Blue , 1.0f );
		m_Light.Ambient  = D3DXCOLOR( _Color.Red , _Color.Green , _Color.Blue , 1.0f );

		m_Light.Position = CDPConverter::toDirectX( RayPos.m_EndPoint );
		m_pd3d->GetCLightManager()->UpdateLight( m_Light , m_LightIndex , TRUE );
		/////////////////////////////////////////

	}
	else
	{
		// AGL起動時以外はライトを消す
		m_pd3d->GetCLightManager()->UpdateLight( m_Light , m_LightIndex , FALSE );
	}

	if( m_lpXFile == nullptr ){ return; }

	CMatrix mTrans;
	CMatrix mRot;
	CMatrix mWorld;

	CMatrix mParent;
	mParent = m_pPlayer->GetMatrix();
	mTrans.SetTrans( m_vPos );
	mRot.SetRotate( m_vRot );
	mWorld = mTrans * mRot * mParent;
	_d3d.SetWorldTrans( &mWorld );

	// レーザー銃の色を変更する
	float ColorPower = 0.9f;
	float ColorBase  = 0.05f;
	m_lpXFile->xData.Mat[0].Ambient.a = 1.0f;
	m_lpXFile->xData.Mat[0].Ambient.r = _Color.Red   * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[0].Ambient.g = _Color.Green * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[0].Ambient.b = _Color.Blue  * ColorPower + ColorBase;

	m_lpXFile->xData.Mat[0].Emissive.a = 1.0f;
	m_lpXFile->xData.Mat[0].Emissive.r = _Color.Red   * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[0].Emissive.g = _Color.Green * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[0].Emissive.b = _Color.Blue  * ColorPower + ColorBase;


	m_lpXFile->xData.Mat[1].Ambient.a = 1.0f;
	m_lpXFile->xData.Mat[1].Ambient.r = _Color.Red   * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[1].Ambient.g = _Color.Green * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[1].Ambient.b = _Color.Blue  * ColorPower + ColorBase;

	m_lpXFile->xData.Mat[1].Emissive.a = 1.0f;
	m_lpXFile->xData.Mat[1].Emissive.r = _Color.Red   * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[1].Emissive.g = _Color.Green * ColorPower + ColorBase;
	m_lpXFile->xData.Mat[1].Emissive.b = _Color.Blue  * ColorPower + ColorBase;


	// AGL本体の描画
	m_lpXFile->Draw( _d3d.GetDevice() );

	// AGL本体のエフェクト
	if( PushFlag )
	{
		// 加算合成開始
		m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );
		m_lpAGLEffect->SetMaterial( m_Material );
		// AGL本体のエフェクトの描画
		//m_lpAGLEffect->Draw( _d3d.GetDevice() ); 
		// 加算合成終了
		m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	}


	// パーティクルの描画
	m_pParticleEmitter->DrawParticle();

}


/*===================================================*/
//! \brief パーティクルの更新
//! \param[in]	_Color		パーティクルの色
//! \param[in]	_Flag		更新の種類
//! \param[in]	_deltaTime	更新する時間
/*===================================================*/
void CLaserBase::ParticleUpdate( LASER_COLOR _Color , bool _Flag , NxReal _DeltaTime )
{
	m_pParticleEmitter->update( _DeltaTime , RayPos.m_EndPoint , _Color , _Flag );
}


/*============================================*/
//! \brief レーザー部分の更新
/*============================================*/
void CLaserBase::LaserUpdate( void )
{
	m_pLaser->Exec( RayPos );
}


/*===========================================================*/
//! \brief レイの当たり判定
//! \param[out]	*Distance	オブジェクトまでの距離
//! \return ヒット = true
//! \return ヒットしなかった = false
/*===========================================================*/
bool CLaserBase::HitCheck( float *Distance )
{

	// レイを飛ばす場所
	NxVec3 orig = m_pd3d->GetCamPos();

	// レイの飛ばす方向
	NxVec3 dir;

	NxMat33 m;
	CMatrix mRot;
	mRot.SetRotate( m_pd3d->GetCamRot() );
	
	NxF64 Tmp[3][3];
	CDPConverter::SetMatrix( Tmp , &mRot );
	m.setColumnMajor( Tmp );
    m.getColumn( 2 , dir );

	NxRay ray( orig , dir );
	NxRaycastHit hit;
	NxReal dist;
	
	// レイ発射
	NxShape* closestShape = m_pPhysX->GetScene()->raycastClosestShape( ray , NX_ALL_SHAPES , hit , ( 1<<CPhysX::CG_DEFAULT ) | ( 1<<CPhysX::CG_HEAVY_OBJECT ) | ( 1<<CPhysX::CG_STATIC ) );
	
	if( closestShape )
	{
		m_pHitActor = &hit.shape->getActor();
		NxVec3& worldImpact = hit.worldImpact;
		dist = hit.distance;
		*Distance = hit.distance;

		CRaycastLine rl( ray.orig , worldImpact , 0xffffffff );
		RayPos = rl;

		return true;
	} 
	else 
	{
		dist = 10000;
		*Distance = 10000;
		CRaycastLine rl( ray.orig , dist*dir , 0xffffffff );
		RayPos = rl;
		return false;
	}

}

