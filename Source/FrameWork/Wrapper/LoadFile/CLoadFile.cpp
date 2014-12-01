#pragma once

#include "stdafx.hpp"

#include "../Application/CApplication.hpp"


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CLoadFile::CLoadFile( void )
{
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CLoadFile::~CLoadFile( void )
{
}


/*============================================================*/
//! \brief ファイル読み込み時のエラーメッセージ
//! \param[in]	_FilePass	ファイルのパス
//! \param[in]	_ErrorPoint	エラーが発生したポイント
/*============================================================*/
void CLoadFile::FileErrorMessage( const std::string _FilePass , const int _ErrorPoint )
{

	std::string sErrorMessage;
	switch( _ErrorPoint )
	{
	case OPEN_ERROR:
		sErrorMessage = _FilePass + "ファイルの読み込みに失敗しました\n" + "ディレクトリに存在するか確認してください";
		ErrorMessage( sErrorMessage );
		break;
	case READ_ERROR:
		sErrorMessage = _FilePass + "のデータに不具合があります\n" + "データの構造をもう一度確認してください";
		ErrorMessage( sErrorMessage );
		break;
	}

}

