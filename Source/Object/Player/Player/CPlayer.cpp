#include "stdafx.hpp"

#include "Wrapper/Vector3/CVector.hpp"
#include "../Scene/SceneManager/CSceneManager.hpp"
#include "../../Object/Box/CBox.hpp"

#include "../LaserGun/LaserBase/CLaserBase.hpp"
#include "../LaserGun/LaserManager/CLaserManager.hpp"

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "Direct3D/CLightManager.hpp"
#include "CPlayer.hpp"


#include "../FrameWork/PhysX/Cloth/BmpLoader.h"
#include "../FrameWork/PhysX/Cloth/MediaPath.h"
#include "../FrameWork/PhysX/Cloth/MyCloth.h"
#include "../FrameWork/PhysX/Cloth/wavefront.h"


NxVec3 BoxPos = NxVec3(0.0f, 12.0f, 0.0f);

// Array of cloth objects
NxArray<MyCloth*> gCloths;

/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CPlayer::CPlayer( void ) 
	: m_pd3d( CAPP->GetCDirect3D() )
	, m_pKeyboard( CAPP->GetCKeyboard() )
	, m_pMouse( CAPP->GetCMouse() )
	, m_MoveVec( 500.0f )
	//, m_pBGM( nullptr )
	, m_pLaserGun( nullptr )
{

	//m_pBGM = SOUND_MANAGER->Load( "walk.wav" , 1 );

	// �}�E�X���\���ɂ���
	m_pMouse->SetCursor( FALSE );

	// �E�B���h�E�̒��S���W
	m_WindowCenterPos = POINT();
	// �ŏ��Ɍ����Ă������
	m_vRot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	// �A�N�^�[�f�[�^
	m_ActorData.Type = 'P';
	m_ActorData.ObjectName = CObjectManager::OBJ_PLAYER;
	m_ActorData.Radius = 2.7f;						// ����
	m_ActorData.Height = 9.0f;						// ����
	m_ActorData.FixFlag = false;					// �ÓI�A�N�^�[��
	m_ActorData.AngularDamping = 50.0f;				// ���C
	m_ActorData.Density = 50000000.0f;				// ���x

	m_TotalHeight = m_ActorData.Radius*2 + m_ActorData.Height;

	//m_ActorData.MatDesc.MaterialDesc.frictionCombineMode = NX_CM_MIN;
	m_ActorData.MatDesc.MaterialDesc.restitution      = 0.0f;
	m_ActorData.MatDesc.MaterialDesc.restitutionCombineMode = NX_CM_MIN;
	m_ActorData.MatDesc.MaterialDesc.staticFriction   = 9.5f;
	m_ActorData.MatDesc.MaterialDesc.dynamicFriction  = 8.5f;
	m_ActorData.MatDesc.isInit = true;

}


/*===================================*/
//! \brief �ʂ̏�����
/*===================================*/
void CPlayer::DiscreteInit( void )
{


	// �E�B���h�E�̒��S���i�[
	m_WindowCenterPos.x = CAPP->GetWindowSize().cx / 2;
	m_WindowCenterPos.y = CAPP->GetWindowSize().cy / 2;
	m_pMouse->SetPos( &m_WindowCenterPos );

	m_ActorData.Position = NxVec3( m_vPos.x , m_vPos.y , m_vPos.z );
	NxMat33 Rot;
	Rot.rotX( m_vRot.x );
	Rot.rotY( m_vRot.y );
	Rot.rotZ( m_vRot.z );
	m_ActorData.Rotation = Rot;


	// ���[�U�[�e�̃C���X�^���X��
	m_pLaserGun = new CLaserManager( this );

	m_pPhysX->SetActorCollisionGroup( m_Actor , CPhysX::CG_PLAYER );
	m_Actor->raiseBodyFlag( NX_BF_FROZEN_ROT );
	//m_Actor->raiseActorFlag
	
	// AGL�A���̃p�[�e�B�N���͈�ԍŌ�ɕ`�悷��

	
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CPlayer::~CPlayer( void )
{
	// �}�E�X��\������
	m_pMouse->SetCursor( TRUE );

	SafeDelete( VARIABLE_DATA( m_pLaserGun ) );
}

//! \brief �X�V
void CPlayer::Exec( void )
{

	// �}�E�X�Ɋ֘A��������
	MouseExec();
	
	// �ړ�
	Move();

	// ���[�U�[�e�̍X�V
	m_pLaserGun->Exec();

#ifdef _DEBUG
	if( m_pKeyboard->GetState( CKeyboard::KEY_I ) == CKeyboard::KST_HOLD )
	{
		OBJECT_MANAGER->AddObject( "Box" , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	}
#endif

}


//! \brief �}�E�X�Ɋ֘A��������
void CPlayer::MouseExec( void )
{

	// �E�B���h�E�̒��S���i�[
	m_WindowCenterPos.x = CAPP->GetWindowSize().cx / 2;
	m_WindowCenterPos.y = CAPP->GetWindowSize().cy / 2;
	
	// Shift�������Ă��鎞�̓}�E�X��\������
	if( m_pKeyboard->GetState( CKeyboard::KEY_SHIFT ) == CKeyboard::KST_PUSH )
	{
		// �}�E�X��\������
		m_pMouse->SetCursor( TRUE );
	}
	else if( m_pKeyboard->GetState( CKeyboard::KEY_SHIFT ) == CKeyboard::KST_PULL )
	{
		// �}�E�X���\���ɂ���
		m_pMouse->SetCursor( FALSE );
	}
	else if( m_pKeyboard->GetState( CKeyboard::KEY_SHIFT ) == CKeyboard::KST_FREE )
	{
		// �J�����̕������X�V
		AddRot( D3DXVECTOR3( ( m_pMouse->GetPos().y - m_WindowCenterPos.y ) / 4.7f , ( m_pMouse->GetPos().x - m_WindowCenterPos.x ) / 4.7f , 0.0f ) );
		D3DXVECTOR3 CamAng = GetRot();
		if( CamAng.x >  85 ){ CamAng.x  = 85; }// ������
		if( CamAng.x < -85 ){ CamAng.x = -85; }// �����
		m_vRot = D3DXVECTOR3( CamAng );
		DIRECTX->SetZoom( -2.0f );
		DIRECTX->SetCamRot( CamAng );
		m_pMouse->SetPos( &m_WindowCenterPos );
	}

}

// \brief �ړ�
void CPlayer::Move( void )
{

	// �f�o�b�O�@���V
	if( m_pKeyboard->GetState( CKeyboard::KEY_Z ) == CKeyboard::KST_HOLD ) 
	{
		m_Actor->setLinearVelocity( NxVec3( 0.0f, 18.0f , 0.0f ) );
	}

	
	D3DXVECTOR3 vVec( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vTmp;
	CMatrix mRot;
	
	mRot.SetRotate( GetRot() );

	//! \note �{���ł���� W = flont , A = Left...�ƂȂ邪
	//! \note setAngularVelocity���g�p����̂ł�����l������
	if( m_pKeyboard->GetState( CKeyboard::KEY_A ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_S ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( -1.0f , 0.0f , 0.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_W ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}
	if( m_pKeyboard->GetState( CKeyboard::KEY_D ) == CKeyboard::KST_HOLD )
	{
		D3DXVec3TransformCoord( &vTmp , &D3DXVECTOR3( 0.0f , 0.0f , -1.0f ) , &mRot );
		vTmp.y = 0;
		vVec += vTmp;
		//m_pBGM->RePlay();
	}

	// �W�����v����
	static NxVec3 JumpVec = NxVec3( 0.0f, 0.0f , 0.0f );
	static int Count = 0;
	static const int MaxCount = 10;
	if( KEYBOARD->GetState( CKeyboard::KEY_SPACE ) == CKeyboard::KST_PUSH && CheckFooting() )
	{
		Count = 0;
		JumpVec = NxVec3( 0.0f, 25.0f , 0.0f );
	}
	// ����������Ƃ�荂�����
	if( KEYBOARD->GetState( CKeyboard::KEY_SPACE ) == CKeyboard::KST_HOLD )
	{
		if( Count < MaxCount )
		{
			JumpVec += NxVec3( 0.0f , 5.7f , 0.0f );
		}
		Count++;
	}else
	{
		Count--;
		if( Count < 0 ){ Count = 0; }
	}


	 
	// �ړ������𐳋K��
	D3DXVec3Normalize( &vVec , &vVec );

	// �i�s�����֗͂�������
	// �ړ�
	D3DXVECTOR3 MoveVec( vVec * m_MoveVec );
	
	m_Actor->setAngularVelocity( NxVec3( MoveVec ) );

	// �W�����v
	CMatrix mTmpRot;
	// �W�����v���ł���Έړ��x�N�g���𐳂����l�ɕ␳����
	mTmpRot.SetRotate( D3DXVECTOR3( 0.0f, -90.0f , 0.0f ) );
	if( JumpVec.y > 0.0001f )
		D3DXVec3TransformCoord( &MoveVec , &MoveVec , &mTmpRot );
	// �����łȂ��Ȃ��0.0f����
	else 
		MoveVec = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//                        �W�����v���̈ړ��@�@�@�@�@�@�@�@�W�����v�ɂ���ւ̃x�N�g��
	m_Actor->addForce( NxVec3( MoveVec ) * 5000000000.0f + ( JumpVec ) * 200000000000.0f );

	// �W�����v�̏�����ւ̃x�N�g�������Z���Ă���
	JumpVec.y -= 5.0f; 
	if( JumpVec.y < 0.0f ){ JumpVec.y = 0.0f; }

	// �n�ʂɉ���������͂�������
	m_Actor->addForce( NxVec3( 0.0f , -800000000000.0f , 0.0f ) );

	
	// ���W�̍X�V
	D3DXMATRIXA16 mWorld;
	CDPConverter::SetMatrix( &mWorld , m_Actor );
	// ���W�̍X�V
	m_vPos.x = mWorld._41;
	m_vPos.y = mWorld._42 + ( m_TotalHeight/2 ) * 0.7f;
	m_vPos.z = mWorld._43;

	DIRECTX->SetCamPos( m_vPos );

}

/*============================================*/
//! \brief ���ꔻ��
//! \return �ڒn�� = true
//! \return �󒆂ɂ��� = false
/*============================================*/
bool CPlayer::CheckFooting( void )
{

	// ���C���΂��ꏊ
	NxVec3 orig = m_vPos;

	// ���C�̔�΂�����
	NxVec3 dir = NxVec3( 0.0f , -1.0f , 0.0f );

	NxRay ray( orig , dir );
	NxRaycastHit hit;
	
	// ���C����
	NxShape* closestShape = m_pPhysX->GetScene()->raycastClosestShape( ray , NX_ALL_SHAPES , hit , ~CPhysX::BitShift( CPhysX::CG_PLAYER ) );
	// ������Ȃ������ꍇNULL
	if( closestShape )
	{
		if( hit.distance < m_TotalHeight ) 
		{
			return true;
		}
		return false; 
	} 
	else 
	{
		return false;
	}
}