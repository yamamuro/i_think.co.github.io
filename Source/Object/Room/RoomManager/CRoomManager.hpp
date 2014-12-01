#pragma once

#include "../Scene/Game/CGame.hpp"
#include "../Object/ObjectBase/CObjectBase.hpp"


class CRoomManager : public CObjectBase
{

	/*=========================================================================================*/
	//! 
	//! \file
	//! \class CRoomManager
	//! \brief <b>�����̊Ǘ��N���X</b>
	//! 
	//! \author Y.Yamamuro \n
	//! \date  2014/09/28
	//! \since 2014/10/02
	//! \since 2014/11/21 �����̍쐬���@���R���x�b�N�X���b�V�����g�p�����쐬���@�ɕύX
	//! 
	/*=========================================================================================*/

public:

	
	/*=====================================================*/
	//! \brief �R���X�g���N�^
	//! \param[in]	_Name	�ǂݍ��ޕ����̖��O
	/*=====================================================*/
	CRoomManager( const int _Name );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CRoomManager( void );


protected:

	std::string m_RoomXFilePass[CGame::ROOM_MAX];	//!< �����̃��b�V�����i�[����Ă���ꏊ�ւ̃p�X

	D3DLIGHT9 m_Light;								//!< �����̃f�B���N�V���i�����C�g

};