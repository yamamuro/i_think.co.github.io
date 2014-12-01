// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//					            PARTICLE SYSTEM CLASS
//
//						  Written by Sam Thompson, 3-17-05
//						    Edited by Bob Schade, 5-1-06
// ===============================================================================

#include "stdafx.hpp" 
#include "../Object/ObjectBase/CObjectBase.hpp"
#include "Direct3D\CDirect3D.hpp"
#include "CParticle.hpp"
#include "CParticleEmitter.hpp"


CParticleEmitter::CParticleEmitter(NxScene* pSceneToSet, NxReal fStartingVelScale, NxVec3 vThermoDynamicAcceleration)
	: m_pd3d( DIRECTX )
	, m_lpTexture( nullptr )
{
	_iHeadIndex = 0;
	_iTailIndex = 0;
	_iParticleCount = 0;
	_pScene = pSceneToSet;
	_fStartingVelScale = (1.0f / NxReal(RAND_MAX)) * fStartingVelScale;
	_fNewParticleTimer = 0.0f;
	_vThermoDynamicAcceleration = vThermoDynamicAcceleration;

	// テクスチャの読み込み
	m_lpTexture = TEXTURE_MANAGER->Load( &std::string( "AGL_Particle_Effect.png" ) );

	CParticle::SetParticleEmitter( this );

	for( auto &x : _aParticleArray )
		x = nullptr;

}

CParticleEmitter::~CParticleEmitter()
{
	removeAllParticles();
}

void CParticleEmitter::removeAllParticles()
{
	while (_iParticleCount > 0)
		removeParticle();
}

void CParticleEmitter::update(NxReal fDeltaTime , NxVec3 Pos , LASER_COLOR _Color , bool Flag )
{

	if( !Flag )
	{
		_fNewParticleTimer -= fDeltaTime;
		if (_fNewParticleTimer < 0.0f)
		{
			CRandomize Rand;
			const int CreateNum = Rand.GetRandomize( 3 , 8 );
			for( int i=0 ; i<CreateNum ; i++ )
			{
				addParticle( Pos , _Color );
			}
			_fNewParticleTimer += cfTimeBetweenParticles;
		}
	}
	else
	{
		// Update all particles
		int iParticlesUpdated = 0;
	
		for (int iParticleIndex = _iTailIndex; iParticlesUpdated < _iParticleCount; iParticleIndex = ((iParticleIndex+1) % ciMaxParticles))
		{
			_aParticleArray[iParticleIndex]->update(fDeltaTime);
			++iParticlesUpdated;
		}
	}
	
}

void CParticleEmitter::addParticle( NxVec3 Pos , LASER_COLOR _Color )
{

	if ((_iTailIndex == _iHeadIndex) && (_iParticleCount != 0)) // FIFO is full
	{
		removeParticle();
		// Now there is a slot free
	}
	
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = 0.2f;
	sphereDesc.group = cParticleCollisionGroup; // this group does not collide with itself
	actorDesc.shapes.pushBack( &sphereDesc );

	actorDesc.body = &bodyDesc;
	actorDesc.density = 10000.0f;

	CRandomize Rand;
	actorDesc.globalPose.t = Pos + NxVec3( (float)Rand.GetRandomize( 0.0f , 1.0f ) , (float)Rand.GetRandomize( 0.0f , 1.0f ) , (float)Rand.GetRandomize( 0.0f , 1.0f ) );

	NxActor* pNewActor =  _pScene->createActor(actorDesc);

	// Give it an initial linear velocity, scaled by _fStartingVelScale
	NxVec3 vRandVec(NxReal(rand()*_fStartingVelScale), NxReal(rand()*_fStartingVelScale), NxReal(rand()*_fStartingVelScale));
	//pNewActor->setLinearVelocity(vRandVec);

	// Turn off gravity for smoke
	pNewActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);

	_aParticleArray[_iHeadIndex] = new CParticle( pNewActor , _vThermoDynamicAcceleration , _Color );

	_iHeadIndex = (_iHeadIndex+1) % ciMaxParticles;
	++_iParticleCount;

}

void CParticleEmitter::removeParticle( void )
{
	// release the actor the tail particle owns
	_pScene->releaseActor(_aParticleArray[_iTailIndex]->getActorReference());
	// delete it
	delete _aParticleArray[_iTailIndex];

	// increment the tail
	_iTailIndex = (_iTailIndex+1) % ciMaxParticles;
	--_iParticleCount;
}


void CParticleEmitter::DrawParticle( void )
{

	
	m_pd3d->GetDevice()->SetRenderState( D3DRS_LIGHTING , FALSE );
	// 頂点の構成データのセット
	m_pd3d->GetDevice()->SetFVF( FVF_Vertex );
	// テクスチャを貼り付ける
	m_pd3d->GetDevice()->SetTexture( NULL , m_lpTexture->GetTexture() );


	CMatrix mTrans;
	CMatrix mRot;
	CMatrix mWorld;
	mRot.SetRotate( m_pd3d->GetCamRot() );
	
	// 加算合成開始
	m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	// アルファテストの有効化
	m_pd3d->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pd3d->GetDevice()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	// 不透明にする値の設定
	m_pd3d->GetDevice()->SetRenderState( D3DRS_ALPHAREF, 0x66 );

	int iParticlesUpdated = 0;

	for (int iParticleIndex = _iTailIndex ; iParticlesUpdated < _iParticleCount; iParticleIndex = ((iParticleIndex+1) % ciMaxParticles))
	{
		iParticlesUpdated++;
		NxActor *pActor = _aParticleArray[iParticleIndex]->GetActor();

		NxVec3 Pos = pActor->getGlobalPosition();
		short Alpha = _aParticleArray[iParticleIndex]->GetAlpha();

		//////////////////////////////////////
		// 板ポリゴンの作成
		VERTEX v[4];
		static const float BaseSize = 0.5f;
		float Scale = _aParticleArray[iParticleIndex]->GetScale();
		v[0].vPos = D3DXVECTOR3( -BaseSize ,  BaseSize , 0.0f ) * Scale;
		v[1].vPos = D3DXVECTOR3(  BaseSize ,  BaseSize , 0.0f ) * Scale;
		v[2].vPos = D3DXVECTOR3(  BaseSize , -BaseSize , 0.0f ) * Scale;
		v[3].vPos = D3DXVECTOR3( -BaseSize , -BaseSize , 0.0f ) * Scale;

		static float Animation = 0.0f;
		if( CAPP->GetFrameCounter() % 1200 ){ Animation += 0.1f; }
		if( Animation >= 1.0f ){ Animation = 0.0f; }
		v[0].Tex = D3DXVECTOR2( Animation      , 0.0f );
		v[1].Tex = D3DXVECTOR2( Animation+0.1f , 0.0f );
		v[2].Tex = D3DXVECTOR2( Animation+0.1f , 1.0f );
		v[3].Tex = D3DXVECTOR2( Animation      , 1.0f );

		LASER_COLOR LaserColor = _aParticleArray[iParticleIndex]->GetColor();
		float BaseColor = Alpha / 3.2f;
		v[0].color = D3DCOLOR_ARGB( Alpha , (int)( Alpha * LaserColor.Red + BaseColor ) , (int)( Alpha * LaserColor.Green + BaseColor ) , (int)( Alpha * LaserColor.Blue + BaseColor ) );
		v[1].color = D3DCOLOR_ARGB( Alpha , (int)( Alpha * LaserColor.Red + BaseColor ) , (int)( Alpha * LaserColor.Green + BaseColor ) , (int)( Alpha * LaserColor.Blue + BaseColor ) );
		v[2].color = D3DCOLOR_ARGB( Alpha , (int)( Alpha * LaserColor.Red + BaseColor ) , (int)( Alpha * LaserColor.Green + BaseColor ) , (int)( Alpha * LaserColor.Blue + BaseColor ) );
		v[3].color = D3DCOLOR_ARGB( Alpha , (int)( Alpha * LaserColor.Red + BaseColor ) , (int)( Alpha * LaserColor.Green + BaseColor ) , (int)( Alpha * LaserColor.Blue + BaseColor ) );
		//////////////////////////////////////

		// 移動行列の作成
		mTrans.SetTrans( CDPConverter::toDirectX( Pos ) );
		// 行列の合成
		mWorld = mRot * mTrans;
		// 行列のセット
		m_pd3d->SetWorldTrans( &mWorld );
		// パーティクルを描画
		m_pd3d->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2 , v , sizeof( VERTEX ) );
		
	}

	m_pd3d->GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	// 加算合成終了
	m_pd3d->GetDevice()->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	

	m_pd3d->GetDevice()->SetRenderState( D3DRS_LIGHTING , TRUE );



}