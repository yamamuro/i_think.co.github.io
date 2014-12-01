#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"

/*===================================*/
//!< �O���錾
/*===================================*/
class CFragment;

class CBreakWall : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CBreakWall
	//!	\brief <b>�d�������悹��Ɖ��鏰</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/21
	//! \since 2014/10/22
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CBreakWall( void );

	/*===================================*/
	//! \brief �ʂ̏�����
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CBreakWall( void );


	/*================================*/
	//! \brief �X�V
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief �`��
	//! \param[in] &_d3d	DirectX�̃f�o�C�X
	/*=============================================*/
	void Draw( CDirect3D &_d3d );


	static const int FragmentType = 3;				//!< �j�Ђ̎��


private:

	
	static const int SetFragmentNum = 100;			//!< �����j�Ђ��΂�T����

	std::string m_BreakWallFilePass;				//!< ����O�̏�Ԃ̃��b�V���̃p�X
	std::string m_BreakedWalldFilePass;				//!< ��ꂽ��Ԃ̃��b�V���̃p�X

	CXFile *m_lpBreakWallXFile;						//!< ����O�̏�Ԃ̃��b�V��
	CXFile *m_lpBreakedWallXFile;					//!< ��ꂽ��Ԃ̃��b�V��

	NxActor *m_BreakWallActor;						//!< �󂳂��O�̃A�N�^�[
	NxActor *m_BreakedWallActor;					//!< �󂳂ꂽ��̃A�N�^�[

	CFragment *m_Fragment[SetFragmentNum];			//!< �j�󂳂ꂽ�Ƃ��ɔ�юU��j��

	bool m_BreakedFlag;								//!< �������Ă��邩�̃t���O

	NxBox m_worldBox;								//!< �I�[�o�[���b�v�Ŏg�p���锻��p�̏����i�[

};