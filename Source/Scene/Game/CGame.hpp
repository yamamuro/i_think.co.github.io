#pragma once

#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"


class CGame : public CSceneBase
{

	/*============================================================*/
	//!
	//! \file
	//! \class CGame
	//! \brief <b>�Q�[���V�[���N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/18
	//!
	/*============================================================*/

public:

	/*=================================*/
	//! \enum ROOM_NAME
	//! \brief �������ʗp
	/*=================================*/
	enum ROOM_NAME
	{
		TUTORIAL_ROOM,		//!< �`���[�g���A������
		PASSAGEWAY,			//!< �S�Ă̕����Ɍq����ʘH
		ROOM1,				//!< ����1
		ROOM2,				//!< ����2
		ROOM3,				//!< ����3
		ROOM4,				//!< ����4
		ROOM_MAX			//!< �����̍ő吔
	};


	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CGame( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CGame( void );


	/*===========================================*/
	//! \brief �X�V
	//! \return �V�[���̕ύX = true
	//! \return �p�� �@�@�@�@= false
	/*===========================================*/
	bool Updata( void );


	/*=================================*/
	//! \brief ��ʕ\��
	/*=================================*/
	void Draw( void );


	/*=======================================*/
	//! \brief �V�[���̖��O��Ԃ�
	/*=======================================*/
	std::string GetName( void ){ return "Game"; }


	/*======================================*/
	//! \brief ������ID��Ԃ�
	/*======================================*/
	inline int GetRoomID( void ){ return m_RoomID; }


	/*======================================*/
	//! \brief �����̃p�X��Ԃ�
	/*======================================*/
	inline std::string GetRoomDataPass( void ){ return m_RoomDataPass; }


private:

	/*======================================*/
	//! \brief ������ID���Z�b�g
	/*======================================*/
	inline void SetRoomID( const int _ID ){ m_RoomID = _ID; }		


	/*======================================*/
	//! \brief �����̃p�X���Z�b�g
	/*======================================*/
	inline void SetRoomPass( const  std::string _RoomDataPass ){ m_RoomDataPass = _RoomDataPass; }


	/*=================================*/
	//! \brief ������؂�ւ���
	/*=================================*/
	void ChangeRoom( const int _ID );

	
	std::string m_RoomsDataPass[ROOM_MAX];	//!< �����̃p�X
	std::string m_Name[ROOM_MAX];			//!< �f�o�b�O�\���Ɏg�p���镔����

	std::string m_RoomDataPass;				//!< �A�N�e�B�u�ȃ��[���f�[�^�ւ̃p�X
	int m_RoomID;							//!< �A�N�e�B�u�ȕ�������ID
	
	CSound* m_pBGM;							//!< ������BGM

};
