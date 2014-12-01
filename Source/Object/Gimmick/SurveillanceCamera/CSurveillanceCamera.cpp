#include "stdafx.hpp"

#include "../Object/Player/Player/CPlayer.hpp"
#include "../Object/Gimmick/GimmickBase/CGimmickBase.hpp"
#include "CSurveillanceCamera.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"

/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CSurveillanceCamera::CSurveillanceCamera( void )
	: m_pPlayer( nullptr )
	, pFuncMode( nullptr )
	, MoveFlag( MoveLeft )
{

	// ���b�V���̓ǂݍ���
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


	// �W���C���g����֐߂̏�����
	m_JointPos[0] = D3DXVECTOR3( 0.0f , 0.1f ,  -2.0f );
	m_JointPos[1] = D3DXVECTOR3( 0.0f , 0.0f ,  -5.0f );
	m_JointPos[2] = D3DXVECTOR3( 0.0f , 1.3f ,   0.0f );
	m_JointPos[3] = D3DXVECTOR3( 0.0f , 0.0f ,  -4.4f );
	m_JointPos[4] = D3DXVECTOR3( 0.0f , 1.8f,   -0.4f );

	m_Rot[0]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// x(�㉺)�@�v���C���[�Ƃ̋����ŕω�
	m_Rot[1]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// m_Rot[0]�̋t
	m_Rot[2]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// y(���E)
	m_Rot[3]      = D3DXVECTOR3( -90.0f , 0.0f , 0.0f );	// x
	m_Rot[4]      = D3DXVECTOR3( 0.0f   , 0.0f , 0.0f );	// y

}

/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CSurveillanceCamera::DiscreteInit( void )
{
	
	// �A�N�^�[�̍쐬
	// �T�C�Y
	NxVec3 Dimension[JointNum+1]  = { NxVec3( 2.0f , 2.0f , 0.4f  )
									, NxVec3( 0.2f , 0.5f , 3.0f  )
									, NxVec3( 0.4f , 1.2f , 0.4f  )
									, NxVec3( 0.4f , 0.2f , 2.0f  )
									, NxVec3( 0.2f , 1.0f , 0.2f  )
									, NxVec3( 1.0f , 1.0f , 2.6f  ) };

	// ���W
	NxVec3 LocalPosition[JointNum+1]  = { NxVec3( 0.0f , 0.0f , -0.0f )
										, NxVec3( 0.0f , 0.0f , -2.0f )
										, NxVec3( 0.0f , 0.6f , -0.0f )
										, NxVec3( 0.0f , 0.0f , -2.3f )
										, NxVec3( 0.0f , 1.0f , -0.0f )
										, NxVec3( 0.0f , 1.0f , -1.3f ) };

	// �A�N�^�[�̋��ʃf�[�^��ݒ�
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
//! \brief �f�X�g���N�^
/*=====================================*/
CSurveillanceCamera::~CSurveillanceCamera( void )
{
}


/*================================*/
//! \brief �X�V
/*================================*/
void CSurveillanceCamera::Exec( void )
{

	if( m_pPlayer == nullptr )
	{
		m_pPlayer = OBJECT_MANAGER->GetCPlayerObject();
		return;
	}


	// �d���̐؂�ւ�//////////////////////////
	// ��l���ւ̕����x�N�g��
	D3DXVECTOR3 toPlayerVec = m_pd3d->GetCamPos() - m_vPos;
	// �p�x���i�[
	D3DXVECTOR3 AngVec;
	CVector::VecToRot( &AngVec , &toPlayerVec );
	float AngDis = AngVec.y - m_Rot[2].y;
	AngDis = (float)( (int)AngDis % 360 );
	if( AngDis <   0 ){ AngDis += 360.0f; }
	if( AngDis > 360 ){ AngDis -= 360.0f; }

	float toPlayerDis = D3DXVec3Length( &toPlayerVec );

	// �ǂꂾ������Ă��Ă��ǔ��\��
	static const float SearchRangeZ = 80.0f;


	//���b�N�I����Ԃ���T�[�`�ւ̈ڍs
	if( m_FuncFlag == LOCK_ON_MODE )
	{
		if( abs( AngDis ) >= 20.0f && abs( AngDis ) <= 360.0f-20.0f 
			|| toPlayerDis > SearchRangeZ )
		{
			m_FuncFlag = SEARCH_MODE;
			pFuncMode = &CSurveillanceCamera::SearchMode;
		}
	}
	// �T�[�`��Ԃ��烍�b�N�I���ւ̈ڍs
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
	

	// ���݊���U���Ă���d��������
	(this->*pFuncMode)();
	
}

/*========================================================*/
//! \brief ��l������������Ȃ��̂ŕӂ���Ď���
/*========================================================*/
void CSurveillanceCamera::SearchMode( void )
{

	static const float MotionSpeed = 0.35f;

	// ������/////////////////////////////////////
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
	// ���֐�///////////////////////////////////
	{
		static const float DefaultAng = -45.0f;
		if( false ){}
		else if( m_Rot[0].x > DefaultAng + MotionSpeed ){ m_Rot[0].x -= MotionSpeed; }
		else if( m_Rot[0].x < DefaultAng - MotionSpeed ){ m_Rot[0].x += MotionSpeed; }
		else{    m_Rot[0].x = DefaultAng; }

		// �s��̍X�V
		mTrans.SetTrans( m_JointPos[0] );
		mRot.SetRotate( m_Rot[0] );
		mCamera = mRot * mTrans * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// ���֐�///////////////////////////////////
	{
		// �s��̍X�V
		mTrans.SetTrans( m_JointPos[1] );
		m_Rot[1].x = -m_Rot[0].x;
		mRot.SetRotate( m_Rot[1] );
		mCamera = mRot * mTrans *mCamera;
	}
	//////////////////////////////////////////////


	// y
	// ��O�֐�///////////////////////////////////
	{
		static const int MotionRangeY = 92;
		mTrans.SetTrans( m_JointPos[2] );
		mCamera = mTrans * mCamera;
		
		// �p�x�����Z
		float AddAng = MotionSpeed * 0.2f;
		if( MoveFlag ){ AddAng =  MotionSpeed; }
		else{           AddAng = -MotionSpeed; }
		m_Rot[2].y += AddAng;

		// �p�x�̐���
		if( m_Rot[2].y >  MotionRangeY + 180.0f + m_vRot.y )
		{
			// �i�s�����̔��]
			MoveFlag = !MoveFlag;
			m_Rot[2].y =  MotionRangeY + 180.0f + m_vRot.y;
		}
		if( m_Rot[2].y < -MotionRangeY + 180.0f + m_vRot.y )
		{ 
			// �i�s�����̔��]
			MoveFlag = !MoveFlag;
			m_Rot[2].y = -MotionRangeY + 180.0f + m_vRot.y;
		}

		// �s��̍X�V
		D3DXVECTOR3 Tmp = m_Rot[2];
		Tmp.y += 180.0f - m_vRot.y;
		mRot.SetRotate( Tmp );
		mCamera = mRot * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// ��l�֐�///////////////////////////////////
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
//! \brief ��l�������b�N�I���������
/*================================================*/
void CSurveillanceCamera::LockOnMode( void )
{
	
	static const float MotionSpeed = 0.65f;

	// ������/////////////////////////////////////
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
	// ���֐�///////////////////////////////////
	{
		static const int MaxMotionRangeX =  120;
		static const int MinMotionRangeX = -70;
		// ��l���Ƃ̋����x�N�g��
		D3DXVECTOR3 Distance = m_pd3d->GetCamPos() - m_vPos;
		
		// �ǂꂾ������Ă��Ă��ǔ��\��
		static const float SearchRangeZ = 80.0f;
		float Dis = D3DXVec3Length( &Distance );
		// �ő�l�␳
		if( Dis > SearchRangeZ ){ Dis = SearchRangeZ; }
		
		if( false ){}
		else if( m_Rot[0].x > MaxMotionRangeX - ( Dis / SearchRangeZ ) * ( MaxMotionRangeX + abs( MinMotionRangeX ) ) + 2.0f ){ m_Rot[0].x -= 2.0f; }
		else if( m_Rot[0].x < MaxMotionRangeX - ( Dis / SearchRangeZ ) * ( MaxMotionRangeX + abs( MinMotionRangeX ) ) - 2.0f ){ m_Rot[0].x += 2.0f; }
		else{    m_Rot[0].x = MaxMotionRangeX - ( Dis / SearchRangeZ ) * ( MaxMotionRangeX + abs( MinMotionRangeX ) ); }

		// �s��̍X�V
		mTrans.SetTrans( m_JointPos[0] );
		mRot.SetRotate( m_Rot[0] );
		mCamera = mRot * mTrans * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// ���֐�///////////////////////////////////
	{
		// �s��̍X�V
		mTrans.SetTrans( m_JointPos[1] );
		m_Rot[1].x = -m_Rot[0].x;
		mRot.SetRotate( m_Rot[1] );
		mCamera = mRot * mTrans *mCamera;
	}
	//////////////////////////////////////////////


	// y
	// ��O�֐�///////////////////////////////////
	{
		static const int MotionRangeY = 92;
		mTrans.SetTrans( m_JointPos[2] );
		mCamera = mTrans * mCamera;
		D3DXVECTOR3 Pos = D3DXVECTOR3( mCamera._41 , mCamera._42 , mCamera._43 );
		// ��l���ւ̕����x�N�g��
		D3DXVECTOR3 PlayerDirection = m_pd3d->GetCamPos() - Pos;
		// �p�x���i�[
		D3DXVECTOR3 AngVec;
		CVector::VecToRot( &AngVec , &PlayerDirection );
	
		float toPlayerAng = AngVec.y;
		float AngDistance = AngVec.y - m_Rot[2].y;
		AngDistance = (float)( (int)AngDistance % 360 );

		// �ǂ���̕����։�]���邩�����߂�
		if( AngDistance >  180.0f ){ AngDistance -= 360.0f; }
		if( AngDistance < -180.0f ){ AngDistance += 360.0f; }

		// �p�x�����Z
		float AddAng = AngDistance * 0.2f;
		if( AddAng >  MotionSpeed ){ AddAng =  MotionSpeed; }
		if( AddAng < -MotionSpeed ){ AddAng = -MotionSpeed; }
		m_Rot[2].y += AddAng;

		// �p�x�̐���
		if( m_Rot[2].y >  MotionRangeY + 180.0f + m_vRot.y ){ m_Rot[2].y =  MotionRangeY + 180.0f + m_vRot.y; }
		if( m_Rot[2].y < -MotionRangeY + 180.0f + m_vRot.y ){ m_Rot[2].y = -MotionRangeY + 180.0f + m_vRot.y; }

		// �s��̍X�V
		D3DXVECTOR3 Tmp = m_Rot[2];
		Tmp.y += 180.0f - m_vRot.y;
		mRot.SetRotate( Tmp );
		mCamera = mRot * mCamera;
	}
	//////////////////////////////////////////////


	// x
	// ��l�֐�///////////////////////////////////
	{
		static const float MotionRangeX = 85;
		mTrans.SetTrans( m_JointPos[3] + m_JointPos[4] );
		mCamera = mTrans * mCamera;
		D3DXVECTOR3 Pos = D3DXVECTOR3( mCamera._41 , mCamera._42 , mCamera._43 );
		D3DXVECTOR3 PlayerDirection = m_pPlayer->GetPos() - Pos;
		// y��]���������ɖ߂�
		D3DXVECTOR3 Tmp = m_Rot[2];
		Tmp.y += 180.0f - m_vRot.y;
		D3DXVECTOR3 ReverseAng = -( m_vRot + Tmp );
		mRot.SetRotate( ReverseAng );
		D3DXVec3TransformCoord( &PlayerDirection , &PlayerDirection , &mRot );
		// �p�x���i�[
		D3DXVECTOR3 AngVec;
		CVector::VecToRot( &AngVec , &PlayerDirection );

		float toPlayerAng = AngVec.x;
		float AngDistance = AngVec.x - m_Rot[3].x;
		AngDistance = (float)( (int)AngDistance % 360 );

		// �ǂ���̕����։�]���邩�����߂�
		if( AngDistance >  180.0f ){ AngDistance -= 360.0f; }
		if( AngDistance < -180.0f ){ AngDistance += 360.0f; }

		// �p�x�����Z
		float AddAng = AngDistance * 0.2f;
		if( AddAng >  MotionSpeed ){ AddAng =  MotionSpeed; }
		if( AddAng < -MotionSpeed ){ AddAng = -MotionSpeed; }
		m_Rot[3].x += AddAng;

		// �p�x�̐���
		if( m_Rot[3].x >  MotionRangeX + 180.0f - m_vRot.y ){ m_Rot[3].x =  MotionRangeX + 180.0f - m_vRot.y; }
		if( m_Rot[3].x < -MotionRangeX - 180.0f - m_vRot.y ){ m_Rot[3].x = -MotionRangeX - 180.0f - m_vRot.y; }
		
	}
	//////////////////////////////////////////////

}


/*=============================================*/
//! \brief �`��
//! \param[in] &_d3d	DirectX�̃f�o�C�X
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

		// �A�N�^�[�̍X�V///////////////////////////////
		NxMat34 Mat;
		
		// ���W���i�[
		Mat.t = CDPConverter::toPhysX( mWorld.GetPos() );

		// �p�x���i�[
		NxF64 Rot[3][3];
		CDPConverter::SetMatrix( Rot , &mWorld );
		Mat.M.setColumnMajor( Rot );
		
		// �s����Z�b�g����
		m_Actors[i]->setGlobalPose( Mat );
		//////////////////////////////////////////////// 


		// �s��̃Z�b�g
		_Direct3D.SetWorldTrans( &mWorld );

		// �`��
		m_lpXFiles[i]->Draw( _Direct3D.GetDevice() );

		// �s��̍X�V
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
