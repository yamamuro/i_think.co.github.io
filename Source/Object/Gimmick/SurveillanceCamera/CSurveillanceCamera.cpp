#include "stdafx.hpp"

#include "../Object/Player/Player/CPlayer.hpp"
#include "../Object/Gimmick/GimmickBase/CGimmickBase.hpp"
#include "CSurveillanceCamera.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"

/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CSurveillanceCamera::CSurveillanceCamera( void )
	: m_pPlayer( nullptr )
	, pFuncMode( nullptr )
	, MoveFlag( MoveLeft )
{

	// メッシュの読み込み
	m_XFilePass[0] = m_FilePass + "Object/Gimmick/SurveillanceCamera/SurveillanceCamera_Base.x";
	m_lpXFiles[0] = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_XFilePass[0] );

	m_XFilePass[1] = m_FilePass + "Object/Gimmick/SurveillanceCamera/SurveillanceCamera_1.x";
	m_lpXFiles[1] = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_XFilePass[1] );

	m_XFilePass[2] = m_FilePass + "Object/Gimmick/SurveillanceCamera/SurveillanceCamera_2.x";
	m_lpXFiles[2] = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_XFilePass[2] );

	m_XFilePass[3] = m_FilePass + "Object/Gimmick/SurveillanceCamera/SurveillanceCamera_3.x";
	m_lpXFiles[3] = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_XFilePass[3] );

	m_XFilePass[4] = m_FilePass + "Object/Gimmick/SurveillanceCamera/SurveillanceCamera_4.x";
	m_lpXFiles[4] = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_XFilePass[4] );

	m_XFilePass[5] = m_FilePass + "Object/Gimmick/SurveillanceCamera/SurveillanceCamera.x";
	m_lpXFiles[5] = XFILE_MANAGER->Load( m_pd3d->GetDevice() , m_XFilePass[5] );


	// ジョイントする関節の初期化
	m_JointPos[0] = D3DXVECTOR3( 0.0f , 0.1f ,  -2.0f );
	m_JointPos[1] = D3DXVECTOR3( 0.0f , 0.0f ,  -5.0f );
	m_JointPos[2] = D3DXVECTOR3( 0.0f , 1.3f ,   0.0f );
	m_JointPos[3] = D3DXVECTOR3( 0.0f , 0.0f ,  -4.4f );
	m_JointPos[4] = D3DXVECTOR3( 0.0f , 1.8f,   -0.4f );

	m_Rot[0]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// x(上下)　プレイヤーとの距離で変化
	m_Rot[1]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// m_Rot[0]の逆
	m_Rot[2]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// y(左右)
	m_Rot[3]      = D3DXVECTOR3( -90.0f , 0.0f , 0.0f );	// x
	m_Rot[4]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// y

}

/*===================================*/
//! \brief 個別の初期化
/*===================================*/
void CSurveillanceCamera::DiscreteInit( void )
{
	
	// アクターの作成
	// サイズ
	NxVec3 Dimension[JointNum+1]  = { NxVec3( 2.0f , 2.0f , 0.4f  )
									, NxVec3( 0.2f , 0.5f , 3.0f  )
									, NxVec3( 0.4f , 1.2f , 0.4f  )
									, NxVec3( 0.4f , 0.2f , 2.0f  )
									, NxVec3( 0.2f , 1.0f , 0.2f  )
									, NxVec3( 1.0f , 1.0f , 2.6f  ) };

	// 座標
	NxVec3 LocalPosition[JointNum+1]  = { NxVec3( 0.0f , 0.0f , -0.0f )
										, NxVec3( 0.0f , 0.0f , -2.0f )
										, NxVec3( 0.0f , 0.6f , -0.0f )
										, NxVec3( 0.0f , 0.0f , -2.3f )
										, NxVec3( 0.0f , 1.0f , -0.0f )
										, NxVec3( 0.0f , 1.0f , -1.3f ) };

	// アクターの共通データを設定
	m_ActorData.Type = 'B';
	m_ActorData.FixFlag = false;
	m_ActorData.Density = 100000000000.0f;
	m_ActorData.ObjectName = CObjectManager::OBJ_SURVEILLANCE_CAMERA;
	m_ActorData.pObject = this;
	m_ActorData.SkinWidth = 0.05f;

	for( int i=0 ; i<JointNum+1 ; i++ )
	{
		m_ActorData.Dimension = Dimension[i];
		D3DXVECTOR3 Tmp;
		CMatrix mRot;
		mRot.SetRotate( m_vRot );
		m_ActorData.LocalPosition = LocalPosition[i];
		m_Actors[i] = m_pPhysX->CreateActor( &m_ActorData );
		m_Actors[i]->raiseBodyFlag( NX_BF_KINEMATIC );
	}

	m_FuncFlag = SEARCH_MODE;
	pFuncMode  = &CSurveillanceCamera::SearchMode;

}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CSurveillanceCamera::~CSurveillanceCamera( void )
{
}


/*================================*/
//! \brief 更新
/*================================*/
void CSurveillanceCamera::Exec( void )
{

	if( m_pPlayer == nullptr )
	{
		m_pPlayer = OBJECT_MANAGER->GetCPlayerObject();
		return;
	}


	// 仕事の切り替え//////////////////////////
	// 主人公への方向ベクトル
	D3DXVECTOR3 toPlayerVec = m_pd3d->GetCamPos() - m_vPos;
	// 角度を格納
	D3DXVECTOR3 AngVec;
	CVector::VecToRot( &AngVec , &toPlayerVec );
	float AngDis = AngVec.y - m_Rot[2].y;
	AngDis = (float)( (int)AngDis % 360 );
	if( AngDis <   0 ){ AngDis += 360.0f; }
	if( AngDis > 360 ){ AngDis -= 360.0f; }

	float toPlayerDis = D3DXVec3Length( &toPlayerVec );

	// どれだけ離れていても追尾可能か
	static const float SearchRangeZ = 80.0f;


	//ロックオン状態からサーチへの移行
	if( m_FuncFlag == LOCK_ON_MODE )
	{
		if( abs( AngDis ) >= 20.0f && abs( AngDis ) <= 360.0f-20.0f 
			|| toPlayerDis > SearchRangeZ )
		{
			m_FuncFlag = SEARCH_MODE;
			pFuncMode = &CSurveillanceCamera::SearchMode;
		}
	}
	// サーチ状態からロックオンへの移行
	if( m_FuncFlag == SEARCH_MODE )
	{
		if( ( abs( AngDis ) <= 5.0f || abs( AngDis ) >= 360.0f-5.0f )
			&& toPlayerDis <= SearchRangeZ )
		{
			m_FuncFlag = LOCK_ON_MODE;
			pFuncMode = &CSurveillanceCamera::LockOnMode;
		}
	}
	///////////////////////////////////////////
	

	// 現在割り振られている仕事をする
	(this->*pFuncMode)();
	
}

/*========================================================*/
//! \brief 主人公が見当たらないので辺りを監視中
/*========================================================*/
void CSurveillanceCamera::SearchMode( void )
{

	static const float MotionSpeed = 0.35f;

	// 下準備/////////////////////////////////////
	CMatrix mTrans;
	mTrans.SetTrans( m_vPos );
	CMatrix mCamera;
	CMatrix mRot;
	mRot.SetRotate( m_vRot );
	mCamera = mRot * mTrans;
	if( m_Rot[0].x >  360 ){ m_Rot[0].x -= 360; }
	if( m_Rot[0].x < -360 ){ m_Rot[0].x += 360; }
	//////////////////////////////////////////////


	// x
	// 第一関節///////////////////////////////////
	{
		static const float DefaultAng = -45.0f;
		if( false ){}
		else if( m_Rot[0].x > DefaultAng + MotionSpeed ){ m_Rot[0].x -= MotionSpeed; }
		else if( m_Rot[0].x < DefaultAng - MotionSpeed ){ m_Rot[0].x += MotionSpeed; }
		else{    m_Rot[0].x = DefaultAng; }

		// 行列の更新
		mTrans.SetTrans( m_JointPos[0] );
		mRot.SetRotate( m_Rot[0] );
		mCamera = mRot * mTrans * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// 第二関節///////////////////////////////////
	{
		// 行列の更新
		mTrans.SetTrans( m_JointPos[1] );
		m_Rot[1].x = -m_Rot[0].x;
		mRot.SetRotate( m_Rot[1] );
		mCamera = mRot * mTrans *mCamera;
	}
	//////////////////////////////////////////////


	// y
	// 第三関節///////////////////////////////////
	{
		static const int MotionRangeY = 92;
		mTrans.SetTrans( m_JointPos[2] );
		mCamera = mTrans * mCamera;
		
		// 角度を加算
		float AddAng = MotionSpeed * 0.2f;
		if( MoveFlag ){ AddAng =  MotionSpeed; }
		else{           AddAng = -MotionSpeed; }
		m_Rot[2].y += AddAng;

		// 角度の制限
		if( m_Rot[2].y >  MotionRangeY + 180.0f + m_vRot.y )
		{
			// 進行方向の反転
			MoveFlag = !MoveFlag;
			m_Rot[2].y =  MotionRangeY + 180.0f + m_vRot.y;
		}
		if( m_Rot[2].y < -MotionRangeY + 180.0f + m_vRot.y )
		{ 
			// 進行方向の反転
			MoveFlag = !MoveFlag;
			m_Rot[2].y = -MotionRangeY + 180.0f + m_vRot.y;
		}

		// 行列の更新
		D3DXVECTOR3 Tmp = m_Rot[2];
		Tmp.y += 180.0f - m_vRot.y;
		mRot.SetRotate( Tmp );
		mCamera = mRot * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// 第四関節///////////////////////////////////
	{
		static const float DefaultAng = -155.0f;
		mTrans.SetTrans( m_JointPos[3] + m_JointPos[4] );
		mCamera = mTrans * mCamera;
		
		if( false ){}
		else if( m_Rot[3].x > DefaultAng + MotionSpeed ){ m_Rot[3].x -= MotionSpeed; }
		else if( m_Rot[3].x < DefaultAng - MotionSpeed ){ m_Rot[3].x += MotionSpeed; }
		else{    m_Rot[3].x = DefaultAng; }
	}
	//////////////////////////////////////////////

}

/*================================================*/
//! \brief 主人公をロックオンした状態
/*================================================*/
void CSurveillanceCamera::LockOnMode( void )
{
	
	static const float MotionSpeed = 0.65f;

	// 下準備/////////////////////////////////////
	CMatrix mTrans;
	mTrans.SetTrans( m_vPos );
	CMatrix mCamera;
	CMatrix mRot;
	mRot.SetRotate( m_vRot );
	mCamera = mRot * mTrans;
	if( m_Rot[0].x >  360 ){ m_Rot[0].x -= 360; }
	if( m_Rot[0].x < -360 ){ m_Rot[0].x += 360; }
	//////////////////////////////////////////////


	// x
	// 第一関節///////////////////////////////////
	{
		static const int MaxMotionRangeX =  120;
		static const int MinMotionRangeX = -70;
		// 主人公との距離ベクトル
		D3DXVECTOR3 Distance = m_pd3d->GetCamPos() - m_vPos;
		
		// どれだけ離れていても追尾可能か
		static const float SearchRangeZ = 80.0f;
		float Dis = D3DXVec3Length( &Distance );
		// 最大値補正
		if( Dis > SearchRangeZ ){ Dis = SearchRangeZ; }
		
		if( false ){}
		else if( m_Rot[0].x > MaxMotionRangeX - ( Dis / SearchRangeZ ) * ( MaxMotionRangeX + abs( MinMotionRangeX ) ) + 2.0f ){ m_Rot[0].x -= 2.0f; }
		else if( m_Rot[0].x < MaxMotionRangeX - ( Dis / SearchRangeZ ) * ( MaxMotionRangeX + abs( MinMotionRangeX ) ) - 2.0f ){ m_Rot[0].x += 2.0f; }
		else{    m_Rot[0].x = MaxMotionRangeX - ( Dis / SearchRangeZ ) * ( MaxMotionRangeX + abs( MinMotionRangeX ) ); }

		// 行列の更新
		mTrans.SetTrans( m_JointPos[0] );
		mRot.SetRotate( m_Rot[0] );
		mCamera = mRot * mTrans * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// 第二関節///////////////////////////////////
	{
		// 行列の更新
		mTrans.SetTrans( m_JointPos[1] );
		m_Rot[1].x = -m_Rot[0].x;
		mRot.SetRotate( m_Rot[1] );
		mCamera = mRot * mTrans *mCamera;
	}
	//////////////////////////////////////////////


	// y
	// 第三関節///////////////////////////////////
	{
		static const int MotionRangeY = 92;
		mTrans.SetTrans( m_JointPos[2] );
		mCamera = mTrans * mCamera;
		D3DXVECTOR3 Pos = D3DXVECTOR3( mCamera._41 , mCamera._42 , mCamera._43 );
		// 主人公への方向ベクトル
		D3DXVECTOR3 PlayerDirection = m_pd3d->GetCamPos() - Pos;
		// 角度を格納
		D3DXVECTOR3 AngVec;
		CVector::VecToRot( &AngVec , &PlayerDirection );
	
		float toPlayerAng = AngVec.y;
		float AngDistance = AngVec.y - m_Rot[2].y;
		AngDistance = (float)( (int)AngDistance % 360 );

		// どちらの方向へ回転するかを決める
		if( AngDistance >  180.0f ){ AngDistance -= 360.0f; }
		if( AngDistance < -180.0f ){ AngDistance += 360.0f; }

		// 角度を加算
		float AddAng = AngDistance * 0.2f;
		if( AddAng >  MotionSpeed ){ AddAng =  MotionSpeed; }
		if( AddAng < -MotionSpeed ){ AddAng = -MotionSpeed; }
		m_Rot[2].y += AddAng;

		// 角度の制限
		if( m_Rot[2].y >  MotionRangeY + 180.0f + m_vRot.y ){ m_Rot[2].y =  MotionRangeY + 180.0f + m_vRot.y; }
		if( m_Rot[2].y < -MotionRangeY + 180.0f + m_vRot.y ){ m_Rot[2].y = -MotionRangeY + 180.0f + m_vRot.y; }

		// 行列の更新
		D3DXVECTOR3 Tmp = m_Rot[2];
		Tmp.y += 180.0f - m_vRot.y;
		mRot.SetRotate( Tmp );
		mCamera = mRot * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// 第四関節///////////////////////////////////
	{
		static const float MotionRangeX = 85;
		mTrans.SetTrans( m_JointPos[3] + m_JointPos[4] );
		mCamera = mTrans * mCamera;
		D3DXVECTOR3 Pos = D3DXVECTOR3( mCamera._41 , mCamera._42 , mCamera._43 );
		D3DXVECTOR3 PlayerDirection = m_pPlayer->GetPos() - Pos;
		// y回転した分元に戻す
		D3DXVECTOR3 Tmp = m_Rot[2];
		Tmp.y += 180.0f - m_vRot.y;
		D3DXVECTOR3 ReverseAng = -( m_vRot + Tmp );
		mRot.SetRotate( ReverseAng );
		D3DXVec3TransformCoord( &PlayerDirection , &PlayerDirection , &mRot );
		// 角度を格納
		D3DXVECTOR3 AngVec;
		CVector::VecToRot( &AngVec , &PlayerDirection );

		float toPlayerAng = AngVec.x;
		float AngDistance = AngVec.x - m_Rot[3].x;
		AngDistance = (float)( (int)AngDistance % 360 );

		// どちらの方向へ回転するかを決める
		if( AngDistance >  180.0f ){ AngDistance -= 360.0f; }
		if( AngDistance < -180.0f ){ AngDistance += 360.0f; }

		// 角度を加算
		float AddAng = AngDistance * 0.2f;
		if( AddAng >  MotionSpeed ){ AddAng =  MotionSpeed; }
		if( AddAng < -MotionSpeed ){ AddAng = -MotionSpeed; }
		m_Rot[3].x += AddAng;

		// 角度の制限
		if( m_Rot[3].x >  MotionRangeX + 180.0f - m_vRot.y ){ m_Rot[3].x =  MotionRangeX + 180.0f - m_vRot.y; }
		if( m_Rot[3].x < -MotionRangeX - 180.0f - m_vRot.y ){ m_Rot[3].x = -MotionRangeX - 180.0f - m_vRot.y; }
		
	}
	//////////////////////////////////////////////

}


/*=============================================*/
//! \brief 描画
//! \param[in] &_d3d	DirectXのデバイス
/*=============================================*/
void CSurveillanceCamera::Draw( CDirect3D & _Direct3D )
{
	
	if( m_lpXFiles[0] == nullptr ||
		m_lpXFiles[1] == nullptr ||
		m_lpXFiles[2] == nullptr ||
		m_lpXFiles[3] == nullptr ||
		m_lpXFiles[4] == nullptr ){ return; }

	CMatrix mTrans;
	CMatrix mRot;
	CMatrix mWorld;
	CMatrix mTmp;

	mTrans.SetTrans( m_vPos );
	mRot.SetRotate( m_vRot );
	mWorld = mRot * mTrans;


	//m_pd3d->DrawString( std::to_string( m_Rot[0].x ) + "\n" + std::to_string( m_Rot[1].x ) + "\n" + std::to_string( m_Rot[2].y ) + "\n" + std::to_string( m_Rot[3].x ) , 200+num*150 , 300 );
	
	for( int i=0 ; i<JointNum+1 ; i++ )
	{

		// アクターの更新///////////////////////////////
		NxMat34 Mat;
		
		// 座標を格納
		Mat.t = CDPConverter::toPhysX( mWorld.GetPos() );

		// 角度を格納
		NxF64 Rot[3][3];
		CDPConverter::SetMatrix( Rot , &mWorld );
		Mat.M.setColumnMajor( Rot );
		
		// 行列をセットする
		m_Actors[i]->setGlobalPose( Mat );
		//////////////////////////////////////////////// 


		// 行列のセット
		_Direct3D.SetWorldTrans( &mWorld );

		// 描画
		m_lpXFiles[i]->Draw( _Direct3D.GetDevice() );

		// 行列の更新
		D3DXVECTOR3 Tmp = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		i == 2 ? Tmp.y = 180.0f - m_vRot.y : 0 ;
		i == 3 ? Tmp.x = 90.0f : 0 ;
		
		if( i < JointNum )
		{
		
			mRot.SetRotate( m_Rot[i] + Tmp ); 
			mTrans.SetTrans( m_JointPos[i] );

			mWorld = mRot * mTrans * mWorld;
		}
	}

}
