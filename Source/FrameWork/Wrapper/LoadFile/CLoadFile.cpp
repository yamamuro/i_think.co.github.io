#pragma once

#include "stdafx.hpp"

#include "../Application/CApplication.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CLoadFile::CLoadFile( void )
{
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CLoadFile::~CLoadFile( void )
{
}


/*============================================================*/
//! \brief �t�@�C���ǂݍ��ݎ��̃G���[���b�Z�[�W
//! \param[in]	_FilePass	�t�@�C���̃p�X
//! \param[in]	_ErrorPoint	�G���[�����������|�C���g
/*============================================================*/
void CLoadFile::FileErrorMessage( const std::string _FilePass , const int _ErrorPoint )
{

	std::string sErrorMessage;
	switch( _ErrorPoint )
	{
	case OPEN_ERROR:
		sErrorMessage = _FilePass + "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n" + "�f�B���N�g���ɑ��݂��邩�m�F���Ă�������";
		ErrorMessage( sErrorMessage );
		break;
	case READ_ERROR:
		sErrorMessage = _FilePass + "�̃f�[�^�ɕs�������܂�\n" + "�f�[�^�̍\����������x�m�F���Ă�������";
		ErrorMessage( sErrorMessage );
		break;
	}

}

