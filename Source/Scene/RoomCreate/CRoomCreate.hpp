#pragma once

#include "../SceneManager/CSceneManager.hpp"
#include "../Scene/SceneBase/CSceneBase.hpp"


class CRoomCreate : public CSceneBase
{

	/*============================================================*/
	//!
	//! \file
	//! \class CRoomCreate
	//! \brief <b>�����쐬�N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/11/05
	//! \since 2014/11/05
	//!
	/*============================================================*/

public:


	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CRoomCreate( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CRoomCreate( void );


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
	std::string GetName( void ){ return "RoomCreate"; }


private:


};
