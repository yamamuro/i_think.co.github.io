#pragma once

/*===================================*/
//!< �O���錾
/*===================================*/
class CObjectBase;
class CGimmickBase;
class CPlayer;
class CPhysX;


class CObjectManager
{

	/*========================================================*/
	//!
	//! \file
	//!	\class CObjectManager
	//! \brief <b>�S�ẴI�u�W�F�N�g�̊Ǘ����s��</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/14
	//!
	/*========================================================*/

public:

	/*================================================*/
	//! \enum DATA_STRUCTURE
	//! \brief �O���t�@�C���̃f�[�^�̊i�[��
	/*================================================*/
	enum DATA_STRUCTURE
	{

		OBJECT_NAME,		//!< �I�u�W�F�N�g�̖��O
		POS_X,				//!< ����X���W
		POS_Y,				//!< ����Y���W
		POS_Z,				//!< ����Z���W
		ROT_X,				//!< X���̏�����]�p�x
		ROT_Y,				//!< Y���̏�����]�p�x
		ROT_Z,				//!< Z���̏�����]�p�x
		SCALE_X,			//!< X���̊g�嗦
		SCALE_Y,			//!< Y���̊g�嗦
		SCALE_Z,			//!< Z���̊g�嗦
		GIMMICK_ID,			//!< �M�~�b�N�𓮍삳����M�~�b�N��ID
		GIMMICK_RECEVER,	//!< �ǂ̃M�~�b�N�ƘA�����ē��삷��̂�
		DATA_STRUCTURE_MAX	//!< �ő�l

	};

	/*==================================================================================*/
	//! \struct LOAD_OBJECT_DATA
	//! \brief <b>�O���t�@�C������ǂݍ��ރf�[�^���i�[���镶�����Ԃ̍\����</b>
	/*==================================================================================*/
	typedef struct LOAD_OBJECT_DATA
	{
		std::string Str[DATA_STRUCTURE_MAX];		//!< �f�[�^���i�[����z��
	}LOAD_OBJECT_DATA;

	/*====================================================================*/
	//! \struct OBJECT_DATA
	//! \brief <b>�ǂݍ��񂾃f�[�^�����ۂɎg����f�[�^�ɂ���</b>
	/*====================================================================*/
	typedef struct OBJECT_DATA
	{

		std::string Name;			//!< �I�u�W�F�N�g��
		D3DXVECTOR3 Pos;			//!< �������W
		D3DXVECTOR3 Rot;			//!< ��]�p�x
		D3DXVECTOR3 Scaling;		//!< �g�嗦
		int GimmickID;				//!< �M�~�b�N��ID
		int Connectoror;			//!< �ڑ�����M�~�b�N��ID

		/*==============================================================*/
		//! ��brief ������
		//! \param[in]		_Data		���������邽�߂̃f�[�^
		/*==============================================================*/
		bool Init( LOAD_OBJECT_DATA _Data )
		{

			bool InitFlag = true;

			try
			{

				Name = _Data.Str[CObjectManager::OBJECT_NAME];

				Pos = D3DXVECTOR3( std::stof( _Data.Str[CObjectManager::POS_X] )
								 , std::stof( _Data.Str[CObjectManager::POS_Y] )
								 , std::stof( _Data.Str[CObjectManager::POS_Z] ) );

				Rot = D3DXVECTOR3( std::stof( _Data.Str[CObjectManager::ROT_X] )
								 , std::stof( _Data.Str[CObjectManager::ROT_Y] )
								 , std::stof( _Data.Str[CObjectManager::ROT_Z] ) );

				Scaling = D3DXVECTOR3( std::stof( _Data.Str[CObjectManager::SCALE_X] )
									 , std::stof( _Data.Str[CObjectManager::SCALE_Y] )
									 , std::stof( _Data.Str[CObjectManager::SCALE_Z] ) );

				GimmickID = std::atoi( _Data.Str[CObjectManager::GIMMICK_ID].c_str() );
				Connectoror = std::atoi( _Data.Str[CObjectManager::GIMMICK_RECEVER].c_str() );

			}
			catch( std::exception )
			{ 
				InitFlag = false;
			}


			return InitFlag;

		}

	}OBJECT_DATA;


	/*======================================*/
	//! \enum OBJECT_LIST
	//! \brief �S�I�u�W�F�N�g���X�g
	/*======================================*/
	enum OBJECT_LIST
	{
		
		// �����l
		OBJ_NONE,					//!< �G���[�`�F�b�N�p
		
		// �v���C���[
		OBJ_PLAYER,					//!< �v���C���[
		OBJ_PLAYER_BULLET,			//!< AGG�̒e

		// �I�u�W�F�N�g
		OBJ_BOX,					//!< �{�b�N�X
		OBJ_CHAIN_BOX,				//!< ���Ɍq���ꂽ�{�b�N�X
		OBJ_FENCE,					//!< �N���֎~�̃t�F���X
		OBJ_BRIDGE,					//!< ��
		OBJ_LIGHT,					//!< �u����
		OBJ_BREAK_WALL,				//!< ���鏰
		
		// �M�~�b�N
		OBJ_SWITCH,					//!< �������ƂŃM�~�b�N�𓮍삳�������
		OBJ_WIREMESH,				//!< ����
		OBJ_LIFT_X,					//!< ���Ɉړ����郊�t�g
		OBJ_LIFT_Y,					//!< �c�Ɉړ����郊�t�g
		OBJ_BALANCE,				//!< �V��
		OBJ_STAIRS,					//!< �X�C�b�`�ŘA�����ē����K�i
		OBJ_CONVEYER,				//!< �{�b�N�X���^�ԃR���x�A
		OBJ_SURVEILLANCE_CAMERA,	//!< �Ď��J����
		OBJ_CLUE_LEVER,				//!< �q���g���o�[

		// �G
		OBJ_FOOL,					//!< �����ɂ����i�߂Ȃ��G
		OBJ_TACKLE,					//!< �ˌ����Ă���G

		// ����
		OBJ_TUTORIAL_ROOM,			//!< �`���[�g���A���X�e�[�W
		OBJ_PASSAGEWAY,				//!< �ʘH
		OBJ_ROOM1,					//!< ����1
		OBJ_ROOM2,					//!< ����2
		OBJ_ROOM3,					//!< ����3
		OBJ_ROOM4,					//!< ����4

		OBJ_MAX						//!< �I�u�W�F�N�g�̍ő吔

	};


	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
	/*==============================================*/
	static CObjectManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CObjectManager();
		}
		return m_pInstance;
	}

	
	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CObjectManager( void );


	/*============================*/
	//! \brief �X�V
	/*============================*/
	void Updata( void );


	/*============================*/
	//! \brief �`��
	/*============================*/
	void Draw( void );


	/*=======================================================================================*/
	//! \brief �I�u�W�F�N�g��ǂݍ���
	//! \param[in]		_FilePass		�ǂݍ��ރf�[�^��ۑ����Ă���csv�t�@�C���̃p�X
	//! \return ���� = true
	//! \return ���s = false
	/*=======================================================================================*/
	bool LoadObject( const std::string _FilePass );


	/*========================================================*/
	//! \brief �I�u�W�F�N�g��ǉ�������
	//! \param[in]	_Name	�ǉ�����I�u�W�F�N�g�̖��O
	//! \param[in]	_Pos	�ǉ�����ۂ̏������W
	/*========================================================*/
	CObjectBase* AddObject( const std::string _Name , const D3DXVECTOR3 _Pos );

	/*========================================================*/
	//! \brief �I�u�W�F�N�g��ǉ�������
	//! \param[in]	*_Object	�ǉ�����I�u�W�F�N�g
	/*========================================================*/
	void AddObject( CObjectBase *_Object );


	/*=======================================================*/
	//! \brief �m�ۂ��Ă���S�I�u�W�F�N�g���������
	/*=======================================================*/
	void AllDeleteObject( void );


	/*================================================*/
	//! \brief �v���C���[�̃|�C���^��Ԃ�
	/*================================================*/
	CPlayer* GetCPlayerObject( void ){ return m_pPlayer; }



	/*==========================================================*/
	// �w�肵��ID�̃M�~�b�N��T���A���삵�Ă���̂��Ԃ�
	//! \param[in]	_Connector	�T�������M�~�b�N��ID
	//! \return ���쒆 = true�@
	//! \return ���삵�Ă��Ȃ� = false
	/*==========================================================*/
	bool GetWorkingFlag( const int _Connector );


	/*============================================================*/
	//! \brief �w�肵���I�u�W�F�N�g�����V�������ׂ�
	//! \param[in]	*_Actor		�T���I�u�W�F�N�g�̃A�N�^�[
	//! \return ���V�� = true
	//! \return ���V���Ă��Ȃ� = false 
	/*============================================================*/
	bool GetAntiGravityFlag( const NxActor *_Actor );


	/*============================================================*/
	//! \brief �A�N�^�[����I�u�W�F�N�g��������
	//! \param[in]	*_Actor		�T���I�u�W�F�N�g�̃A�N�^�[
	//! \return �I�u�W�F�N�g�̃|�C���^
	/*============================================================*/
	CObjectBase* CObjectManager::GetActorToObject( const NxActor *_Actor );


	/*=======================================================*/
	//! \brief �I�u�W�F�N�g�̕��V�\���Ԃ�0�ɂ���
	/*=======================================================*/
	void ResetActiGravity( void );


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CObjectManager( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CObjectManager( const CObjectManager& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CObjectManager& operator = ( const CObjectManager& single );


	/*=================================================================================================*/
	//! \brief �����őS�ẴI�u�W�F�N�g�𐶐�����
	//! \brief �O���t�@�C������ǂݍ���
	//! \param[in]	_ObjectData		�I�u�W�F�N�g���\������f�[�^
	//! \param[in]	_FilePass		�G���[���ɏo�͂Ŏg�p����s��̂���t�@�C���̃t�@�C����
	//! \return ���������I�u�W�F�N�g
	/*=================================================================================================*/
	CObjectBase* MakeObject( const OBJECT_DATA _ObjectData , const std::string _FilePass );


	//! \brief �C���X�^���X�̃|�C���^
	static CObjectManager *m_pInstance;		//!< CObjectManager�̃|�C���^

	//! \brief �e�������ɉ���A�ǂݍ��݂��s��
	//! \note �����ɕ����̍\���ɕK�v�ȃI�u�W�F�N�g���S�Ċi�[�����
	static std::vector< CObjectBase*  > m_ObjectList;			//!< �ǂݍ��ݎ��ɍ쐬���ꂽ�I�u�W�F�N�g���i�[
	static std::vector< CGimmickBase* > m_GimmickList;			//!< �ǂݍ��ݎ��ɍ쐬���ꂽ�M�~�b�N���i�[
	static std::vector< CObjectBase*  > m_AddObjectList;		//!< �Q�[���v���C���ɒǉ����ꂽ�I�u�W�F�N�g���i�[

	CDirect3D *m_pD3D;				//!< CDirect3D�̃|�C���^
	CPhysX *m_pPhysX;				//!< CPhysX�̃|�C���^
	int m_ErrorCheckCounter;		//!< �����̕����f�[�^��ǂݍ������Ƃ��Ă��Ȃ����̃`�F�b�N

	//! \brief ���ʂɃv���C���[�̂݃|�C���^�������Ă���
	CPlayer* m_pPlayer;				//!< �v���C���[�̃|�C���^

};

/*==========================================*/
//! \def OBJECT_MANAGER
//! \brief �R�[�����Ɏg�p����}�N��
/*==========================================*/
#define OBJECT_MANAGER CObjectManager::GetInstance()