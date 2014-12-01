#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"
class CLaserManager;


/*==============================================*/
//!
//! \file
//! \class CPlayer
//! \brief <b>�v���[���[�̊Ǘ�</b>
//!
//! \author	Y.Yamamuro \n
//! \date  2014/09/03
//! \since 2041/09/12
//!
/*==============================================*/

class CPlayer : public CObjectBase
{
public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CPlayer( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CPlayer( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	/*=======================================*/
	//! \brief �}�E�X�Ɋ֘A��������
	/*=======================================*/
	void MouseExec( void );


	/*=================================*/
	//! \brief �ړ�
	/*=================================*/
	void Move( void );

	
	/*============================================*/
	//! \brief ���ꔻ��
	//! \return �ڒn�� = true
	//! \return �󒆂ɂ��� = false
	/*============================================*/
	bool CPlayer::CheckFooting( void );


	/*=============================================*/
	//! \brief ���[�U�[�e�̃|�C���^��Ԃ�
	/*=============================================*/
	inline CLaserManager* GetLaserGun( void ){ return m_pLaserGun; }


private:

	CDirect3D* m_pd3d;				//!< Direct3D�̃C���X�^���X���i�[
	CKeyboard* m_pKeyboard;			//!< CKeyboard�̃C���X�^���X���i�[
	CMouse* m_pMouse;				//!< CMouse�̃C���X�^���X���i�[
	POINT m_WindowCenterPos;		//!< �E�B���h�E�̒��S���W

	const float m_MoveVec;			//!< �ړ���


	float m_TotalHeight;			//!< �J�v�Z���̔��a+����+���a

	//CSound* m_pBGM;				//!< ��l���̕�����

	CLaserManager *m_pLaserGun;		//!< ���[�U�[�e�̃|�C���^


};

