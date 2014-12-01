#pragma once 

#include "../GimmickBase/CGimmickBase.hpp"


class CSurveillanceCamera : public CGimmickBase
{

	/*===============================================*/
	//!
	//! \file
	//! \class CSurveillanceCamera
	//! \brief <b>�Ď��J�����̊Ǘ��N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/16
	//!
	/*===============================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CSurveillanceCamera( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CSurveillanceCamera( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	

	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	void Draw( CDirect3D & _Direct3D );


private:

	/*========================================================*/
	//! \brief ��l������������Ȃ��̂ŕӂ���Ď���
	/*========================================================*/
	void SearchMode( void );

	/*================================================*/
	//! \brief ��l�������b�N�I���������
	/*================================================*/
	void LockOnMode( void );


	static const int JointNum = 5;			//!< �֐߂̐�

	D3DXVECTOR3 m_JointPos[JointNum];		//!< ���̊֐߂̍��W
	D3DXVECTOR3 m_Rot[JointNum];			//!< �e�֐߂̌����Ă���p�x

	std::string m_XFilePass[JointNum+1];	//!< XFile�̃p�X
	CXFile *m_lpXFiles[JointNum+1];			//!< XFile�̃|�C���^���i�[
	NxActor *m_Actors[JointNum+1];			//!< �A�N�^�[�̃|�C���^

	CPlayer *m_pPlayer;						//!< �v���C���[�̍��W�̊m�F�̂��߂Ƀ|�C���^�������Ă���

	void ( CSurveillanceCamera::*pFuncMode )( void );	//!< ���݂̍s���p�^�[���̃����o�֐��|�C���^
	
	/*=================================================*/
	//! \enum SurveillanceCameraFuncType
	//! \brief �Ď��J�����̍s���p�^�[��
	/*=================================================*/
	enum SurveillanceCameraFuncType
	{
		SEARCH_MODE,	//!< ��l����T���ӂ�����n��
		LOCK_ON_MODE	//!< ��l����ǔ�
	};

	SurveillanceCameraFuncType m_FuncFlag;				//!< ���݂̍s���p�^�[���̃t���O

	static const bool MoveLeft = true;
	static const bool MoveRight = false;
	bool MoveFlag;
};

