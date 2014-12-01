#pragma once

class CSound
{

	/*===================================================*/
	//!
	//! \file
	//! \class CSound
	//! \brief <b>1�̃T�E���h�̊Ǘ�</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/06
	//! \since 2014/10/06
	//!
	/*===================================================*/

public:

	/*====================================================================*/
	//! \brief �R���X�g���N�^
	//! \param[in]	*_FilePass		Wave�t�@�C���̃p�X
	//! \param[in]	_lpDSound		DirectSound�̃C���^�[�t�F�C�X
	//! \param[in]	_BuffNum		�쐬����o�b�t�@��
	/*====================================================================*/
	CSound( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CSound( void );


	/*====================================================================*/
	//! \brief Wave�t�@�C����ǂݍ���
	//! \param[in]	*_FilePass		Wave�t�@�C���̃p�X
	//! \param[in]	_lpDSound		DirectSound�̃C���^�[�t�F�C�X
	//! \param[in]	_BuffNum		�쐬����o�b�t�@��
	/*====================================================================*/
	void Load( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum );


	/*===================================*/
	//! \brief ����ʏ�Đ�����
	/*===================================*/
	void Play( void );


	/*===================================*/
	//! \brief �������[�v�Đ�����
	/*===================================*/
	void RePlay( void );


	/*===================================*/
	//! \brief �Đ����Ă��鉹���~�߂�
	/*===================================*/
	void Stop( void );


	/*===================================*/
	//! \brief �o�b�t�@����Ԃ�
	/*===================================*/
	inline int GetBuffNum( void ){ return BuffNum; }


private:

	/*===================================================================*/
	//! \brief �����Ŏ��ۂ�Wave�t�@�C����ǂݍ���
	//! \param[in]	*_FilePass		Wave�t�@�C���̃p�X
	//! \param[in]	_lpDSound		DirectSound�̃C���^�[�t�F�C�X
	/*===================================================================*/
	void LoadWAVE( const std::string *_FilePass , const LPDIRECTSOUND8 lpDSound );


	int BuffNum;						//!< ��������o�b�t�@��
	LPDIRECTSOUNDBUFFER8 *lpSSecond;	//!< �Z�J���_���o�b�t�@�̃|�C���^���i�[
	DWORD Hz;							//!< ����Hz���i�[

};