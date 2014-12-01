#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"
#include "../Application/CApplication.hpp"


class CSceneBase
{

	/*================================================*/
	//!
	//! \file
	//! \class CSceneBase
	//! \brief <b>�e�V�[���̊��N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/10
	//! \since 2014/09/12
	//!
	/*================================================*/

public:


	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CSceneBase( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	virtual ~CSceneBase( void );


	/*=================================*/
	//! \brief �X�V
	//! \return �V�[���̕ύX = true
	//! \return �p�� �@�@�@�@= false
	/*=================================*/
	virtual bool Updata( void ) = 0;


	/*=================================*/
	//! \brief ��ʕ\��
	/*=================================*/
	virtual void Draw( void ) = 0;


	/*=======================================*/
	//! \brief �V�[���̖��O��Ԃ�
	/*=======================================*/
	virtual std::string GetName( void ) = 0;


protected:

	std::string m_FilePass;
	CDirect3D *m_pd3d;					//!< CDirect3D�̃|�C���^
	CPhysX *m_pPhysX;					//!< CPhysX�̃|�C���^

	CKeyboard *m_pKeyboard;				//!< CKeyboard�̃|�C���^
	CDebug *m_pDebug;					//!< FPS��\�����邽�߂ɃC���X�^���X�������Ă���
	
	CObjectManager *m_pObjectManager;	//!< �I�u�W�F�N�g�}�l�[�W��
	CSceneManager *m_pSceneManager;		//!< �V�[���}�l�[�W��

};