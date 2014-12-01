#pragma once

#include "Debug\CDebugRender.hpp"
#include "Wrapper\Matrix\CMatrix.hpp"
#include "Direct3D\CDirect3D.hpp"
#include "../Object/ObjectManager/CObjectManager.hpp"

/*===================================*/
//!< �O���錾
/*===================================*/
class ContactCallBack;
class MyJointNotify;


/*================================================================*/
//! \struct CUSTOMVERTEX
//! \brief <b>�f�o�b�O���ɕ\�����郏�C���[�t���[���̒��_���</b>
/*================================================================*/
typedef struct CUSTOMVERTEX
{
    FLOAT x, y, z;
}CUSTOMVERTEX;


/*================================================================*/
//! \struct MATERIAL_DATA
//! \brief <b>�A�N�^�[�̃}�e���A�����쐬����ۂɎg�p</b>
/*================================================================*/
typedef struct MATERIAL_DATA
{

	NxMaterialDesc MaterialDesc;	//!< �}�e���A�����(�ʂ̃A�N�^�[�Ƃ̏Փˎ��̌v�Z�Ȃǂ�ݒ肷��)
	bool isInit;

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	MATERIAL_DATA( void )
		: isInit( false )
	{
		MaterialDesc.setToDefault();
	}

}MATERIAL_DATA;


/*============================================*/
//! \struct ACTOR_DATA
//! \brief <b>�A�N�^�[�̃f�[�^���Ǘ�</b>
//! \brief ��ɏ������̍ۂɎg�p
/*============================================*/
typedef struct ACTOR_DATA
{

	char Type;						//!< �A�N�^�[�̌^( B , S , C , P , G )
	int ObjectName;					//!< CObjectManager�Œ�`���Ă���萔
	NxVec3 Dimension;				//!< �{�b�N�X�̐��@
	float Radius;					//!< ���̔��a
	float Height;					//!< �J�v�Z���̓���
	bool FixFlag;					//!< �A�N�^�[���Œ肷�邩���̂ɂ��邩�̃t���O
	NxReal AngularDamping;			//!< ��]�ɂ�鐊�ތW�� �]����₷��
	NxReal Density;					//!< ���� �d��

	NxVec3 LocalPosition;			//!< ���[�J�����W
	NxMat33 LocalRotation;			//!< ���[�J���Ȏp��
	NxVec3 Position;				//!< �A�N�^�[�̏����ʒu
	NxMat33 Rotation;				//!< �A�N�^�[�̏����p��

	NxVec3 MeshScale;				//!< ���b�V���̊g��k��
	NxVec3 MeshTranslation;			//!< ���b�V���̕��s�ړ�
	D3DXMATRIX MeshRotation;		//!< ���b�V���̉�]
	CObjectBase* pObject;			//!< ���g���Ǘ�����I�u�W�F�N�g�̃|�C���^

	NxU32 ContactPairFlag;			//!< �A�N�^�[�̏Փ˔���
	NxActor *pContactActor;			//!< �Փ˂����A�N�^�[�̃|�C���^

	MATERIAL_DATA MatDesc;			//!< �}�e���A��
	NxReal SkinWidth;				//!< ���̃A�N�^�[�ɂ߂荞�ޗ�


	/*=====================================*/
	// �f�t�H���g�R���X�g���N�^
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
	//! \brief <b>PhysX�̊Ǘ�</b>
	//!
	//! \author	okawa & Y.Yamamuro \n
	//! \date  2014/09/03
	//! \since 2014/09/18
	//!
	/*================================================*/

public:


	/*=======================================================================================================*/
	//! \enum CollisionGroup
	//! \brief �R���W�����O���[�v
	//! \note �����I�Ƀt���O�Ǘ������̂� | (or)���g�p�����R���W�����̕���������\
	/*=======================================================================================================*/
	enum CollisionGroup
	{
		CG_DEFAULT,			//!< �f�t�H���g�̃R���W�����O���[�v
		CG_HEAVY_OBJECT,	//!< ��l�������d���I�u�W�F�N�g
		CG_STATIC,			//!< �������Ƃ̂Ȃ��I�u�W�F�N�g( �����̃A�N�^�[�Ȃ� )
		CG_PLAYER,			//!< ��l��
		CG_THROUGH,			//!< ���[�U�[�����蔲�������
		CG_PARTICLE			//!< �p�[�e�B�N��
	};
	// CollisionGroup�̒萔���V�t�g����
	static inline int BitShift( CollisionGroup _Bit ){ return ( 1 << _Bit ); }

	//!< CG_THROUGH,CG_PARTICLE�ȊO
	static const unsigned int CG_OBJECT_ALL = ( CG_DEFAULT | CG_HEAVY_OBJECT | CG_STATIC | CG_PLAYER );


	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
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
	//! \brief ������
	//! \return ���� = true 
	//! \return ���s = fasle
	/*==================================*/
	bool Initialize( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CPhysX( void );

	/*==================================*/
	//! \brief �I������
	/*==================================*/
	void Finalize( void );


	/*=================================================================*/
	//! \brirf �A�N�^�[�̓������V�[����ŃV�~�����[�V��������
	/*=================================================================*/
	void Simulation( void );


	/*=====================================================================*/
	//! \brief �R���W�����O���[�v�̃Z�b�g
	//! \param[in]	*_Actor		�Z�b�g����A�N�^�[
	//! \param[in]	_Group		�Z�b�g����O���[�vID
	/*=====================================================================*/
	void SetActorCollisionGroup( NxActor *_Actor , NxCollisionGroup _Group );


	/*==========================================================*/
	//! \brief �R���W�����O���[�v�̏�����
	/*==========================================================*/
	void InitActorCollisionGroups( void );


	/*====================================================*/
	//! \brief ���[�U���|�[�g�̏�����
	/*====================================================*/
	void InitContactReportsPerPair( NxActor *_Actor );


	/*===============================================*/
	//! \brief �A�N�^�[���폜����
	//! \param[in]	*_pActor	�폜����A�N�^�[
	//! \note ������
	/*===============================================*/
	void DeleteActor( NxActor *_pActor );


	/*==============================================*/
	//! \brief ���݂��Ă���A�N�^�[��S�č폜
	/*==============================================*/
	void AllDeleteActor( void );


	/*=========
	�ȈՂ̃{�b�N�X�쐬
	======*/
	NxActor* CreateBox(const NxVec3& pos, const NxVec3& boxDim, const NxReal density);
	/* �J�v�Z�� */
	NxActor* CreateCapsule( const NxVec3 &_Pos , const NxReal _Height , const NxReal _Radius , const NxReal _Density , BOOL flag , int _MaterialIndex = 0 );
	
	/*=======================================================================*/
	//! \brief �A�N�^�[�̒ǉ�
	//! \param[in]	*_pActorData			�쐬����A�N�^�[�̃f�[�^
	//! \return ���� = �ǉ������A�N�^�[
	//! \return ���s = nullptr
	/*=======================================================================*/
	NxActor* CreateActor( ACTOR_DATA *_pActorData );


	/*======================================================================*/
	//! \brief ���b�V������A�N�^�[���쐬����
	//! \param[in]	_lpMesh		���b�V���f�[�^
	//! \return ���� = �ǉ������A�N�^�[
	//! \return ���s = nullptr
	/*======================================================================*/
	NxActor* CreateConvexActor( LPD3DXMESH _lpMesh );


	/*=====================================================================*/
	//! \brief ���[�v���쐬����
	//! \param[out]		**_ropeSegs		���[�v�̃A�N�^�[�̃|�C���^
	//! \param[out]		**_ropeLinks	���[�v�̃W���C���g�����̃|�C���^
	//! \param[in]		_Pos			�z�u������W
	//! \param[in]		_segHeight		1�̃A�N�^�[�̍���
	//! \param[in]		_segRadius		1�̃A�N�^�[�̔��a
	//! \param[in]		_nbSegments		�����̃A�N�^�[���q����
	//! \param[in]		_density		1�̃A�N�^�[�̖��x
	//! \param[in]		_MaterialIndex	�}�e���A���̃C���f�b�N�X�ԍ�
	/*=====================================================================*/
	void CreateRope( NxActor **_ropeSegs , NxSphericalJoint **_ropeLinks ,  const NxVec3& _Pos , const NxReal _segHeight , const NxReal _segRadius , const int _nbSegments , const NxReal _density , int _MaterialIndex = 0 );


	/*================================================================*/
	//! \brief �A�N�^�[���W���C���g����
	//! \param[in]	*_a0			�W���C���g����A�N�^�[
	//! \param[in]	*_a1			�W���C���g����A�N�^�[
	//! \param[in]	&_globalAnchor	�W���C���g������W
	//! \param[in]	&_globalAxis	�W���C���g���鎲
	/*================================================================*/
	NxSphericalJoint* CreateRopeSphericalJoint( NxActor *_a0 , NxActor *_a1 , const NxVec3 &_globalAnchor , const NxVec3 &_globalAxis );

	NxRevoluteJoint* CreateRevoluteJoint(NxActor* a0, NxActor* a1, NxVec3 globalAnchor, NxVec3 globalAxis);
	
	/*============================================================*/
	//! \brief 2�̃A�N�^�[���W���C���g����
	//! \param[in]		�W���C���g����A�N�^�[
	//! \param[in]		�W���C���g����A�N�^�[
	//! \param[in]		�W���C���g������W
	//! \param[in]		�W���C���g���鎲
	/*============================================================*/
	NxFixedJoint* CPhysX::CreateFixedJoint( NxActor *_a0 , NxActor *_a1 , NxVec3 _globalAnchor , NxVec3 _globalAxis );


	/*======================================*/
	//! \brief �V�[����Ԃ�
	/*======================================*/
	inline NxScene* GetScene( void ){ return m_pScene; }


	/*=============================================*/
	//! \brief �f�o�b�O�`��N���X��Ԃ�
	/*=============================================*/
	inline CDebugRender* GetDebugRender( void ){ return m_pDebugRender; }


	/*==========================================================*/
	//! \brief �W���C���g�̃��[�U���|�[�g��Ԃ�
	/*==========================================================*/
	inline MyJointNotify* GetJointNotify( void ){ return m_JointNotify; }


	/*==========================================*/
	//! \brief �f�o�b�O�f�[�^�̃Z�b�g
	//! \return ���� = S_OK
	//! \return ���s = E_FAIL
	/*==========================================*/
	HRESULT CreateDebugBase( void );


	/*===================================================================*/
	//! \brief ���C���[�t���[����`�悷�邩�̃t���O�𔽓]����
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
	//! \brief PhysX�̃f�o�b�O�`��t���O��Ԃ�
	/*============================================================*/
	inline bool GetPhysXDrawFlag( void ){ return m_DrawPhysX; }


	/*============================================================*/
	//! \brief �A�N�^�[�����C���[�t���[���ŕ`�悷��
	/*============================================================*/
	void RenderPhysX( void );


	/*============================================================*/
	//! \brief PhysX��̃{�b�N�X�����C���[�t���[���ŕ`�悷��
	//!	\param[in]	*_Actor			�`�悷��A�N�^�[
	//!	\param[in]	&_MatWorld		�Z�b�g����s��
	/*============================================================*/
	void DrawBox( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld );

	/*============================================================*/
	//! \brief PhysX��̋������C���[�t���[���ŕ`�悷��
	//!	\param[in]	*_Actor			�`�悷��A�N�^�[
	//!	\param[in]	&_MatWorld		�Z�b�g����s��
	/*============================================================*/
	void DrawSphere( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld );

	/*============================================================*/
	//! \brief PhysX��̃J�v�Z�������C���[�t���[���ŕ`�悷��
	//!	\param[in]	*_Actor			�`�悷��A�N�^�[
	//!	\param[in]	&_MatWorld		�Z�b�g����s��
	/*============================================================*/
	void DrawCapsule( NxActor *_Actor , D3DXMATRIXA16 &_MatWorld );

	/*============================================================*/
	//! \brief PhysX��̒n�ʂ����C���[�t���[���ŕ`�悷��
	/*============================================================*/
	void DrawGround( void ); 


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CPhysX( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CPhysX( const CPhysX& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CPhysX& operator = ( const CPhysX& single );


	static const int D3DFVF_CustomVertex = ( D3DFVF_XYZ );		//!< ���_�f�[�^�̍\��

	static const float SIMULATE_STEP;			//!< �V�[���̍X�V�X�s�[�h

	//! \brief �C���X�^���X�̃|�C���^
	static CPhysX *m_pInstance;					//!< CPhysX�̃|�C���^

	NxPhysicsSDK* m_pPhysicsSDK;				//!< PhysX��SDK
	NxScene*	m_pScene;						//!< �V�[�����i�[
	CDebugRender* m_pDebugRender;				//!< �f�o�b�O�`��
	LPDIRECT3DVERTEXBUFFER9 m_lpVB;				//!< ���_�o�b�t�@
	bool m_DrawPhysX;							//!< PhysX�̕`��t���O

	static std::vector< NxActor* > m_Data;		//!< �R���W�����O���[�v�̃��X�g

	ContactCallBack *m_ContactCallBack;			//!< �Փˌ��m�p�̃R�[���o�b�N�N���X
	MyJointNotify *m_JointNotify;				//!< �W���C���g�̃R�[���o�b�N�N���X

};

/*================================*/
//! \def PHYSX
//! \brief �R�[�����Ɏg�p����
/*================================*/
#define PHYSX CPhysX::GetInstance()



class ContactCallBack : public NxUserContactReport
{

	/*================================================*/
	//!
	//! \file
	//! \class ContactCallBack
	//! \brief <b>�R���^�N�g(�Փ�)���̃R�[���o�b�N�N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/22
	//! \since 2014/10/24
	//!
	/*================================================*/


	/*==================================================================================*/
	//! \brief �R�[���o�b�N���Ɏ��s����֐�
	//! \param[in]	&pair		�Փ˂���2�̃A�N�^�[
	//! \param[in]	events		�Փ˒���A�Փ˒��A�ՓˏI���̂ǂꂩ������
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