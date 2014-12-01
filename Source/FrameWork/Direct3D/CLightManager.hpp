#pragma once 

class CDirect3D;

class CLightManager
{

	/*===================================================*/
	//!
	//! \file
	//! \class CLightManager
	//! \brief <b>���C�g�̊Ǘ��N���X</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/31
	//! \since 2014/11/03
	//!
	/*===================================================*/

public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CLightManager( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CLightManager( void );


	/*===========================================================*/
	//! \brief ���C�g��ǉ�����
	//! \param[in]	_Light		�ǉ����郉�C�g
	//! \param[in]	_Flag		���C�g��ON,OFF
	//! \return �C���f�b�N�X�ԍ�
	/*===========================================================*/
	int AddLight( D3DLIGHT9 _Light , BOOL _Flag = TRUE );


	/*============================================================*/
	//! \brief ���C�g�̏�Ԃ��X�V����
	//! \param[in]	_Light		�X�V���郉�C�g
	//! \param[in]	_Index		�X�V���郉�C�g�̃C���f�b�N�X�ԍ�
	//! \param[in]	_Flag		���C�g��ON,OFF
	/*============================================================*/
	void UpdateLight( D3DLIGHT9 _Light , int _Index , BOOL _Flag = TRUE );


	/*===============================================================*/
	//! \brief ���C�g�̐ݒ��ύX����
	//! \param[in]	_Index		�ύX���郉�C�g�̃C���f�b�N�X�ԍ�
	//! \param[in]	_Flag		���C�g��ON,OFF
	/*===============================================================*/
	void SetLightFlag( DWORD _Index , BOOL _Flag );

	/*==============================================================*/
	//! \brief ���ׂẴ��C�g�̐ݒ��ύX����
	//! \param[in]	_Flag		���C�g��ON,OFF
	/*==============================================================*/
	void SetAllLightFlag( BOOL _Flag );


	/*========================================*/
	//! \brief ���C�g��S�ď���
	/*========================================*/
	void AllDelete( void );


private:

	int m_LightNum;			//!< ���݊Ǘ����Ă��郉�C�g�̐�

	CDirect3D *m_pd3d;		//!< CDirect3D�̃|�C���^

	std::vector< D3DLIGHT9 > m_LightList;	//!< ���ݎg�p���Ă��郉�C�g�̃��X�g

};