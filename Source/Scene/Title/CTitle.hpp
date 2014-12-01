#pragma once 

#include "../Scene/SceneBase/CSceneBase.hpp"


class CTitle : public CSceneBase
{

	/*===============================================*/
	//!
	//! \file
	//! \class CTitle
	//! \brief <b>�^�C�g����ʂ��Ǘ�</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/10
	//! \since 2014/09/10
	//!
	/*================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CTitle( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CTitle( void );


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
	std::string GetName( void ){ return "Title"; }


private:

	
	CTexture *m_lpTitleTexture;			//!< ���S�}�[�N
	CTexture *m_lpPPushSpaceTexture;	//!< Push Space�̕`���ꂽ�����e�N�X�`��

	CSprite* m_pSprite;					//!< �X�v���C�g

};