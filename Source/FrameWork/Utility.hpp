#pragma once

#include "Input/Keyboard/CKeyboard.hpp"
#include "Debug/CDebug.hpp"
#include "../Application/CApplication.hpp"

/*=================================*/
// �e���v���[�g�̐錾
/*=================================*/

/*==============================================================*/
//! \brief ���S�ȃ������̉��
//! \tparam[in,out]		*&_p	������郁����
/*==============================================================*/
template <typename T>
inline void SafeDelete( T *&_p )
{
	if( _p != nullptr ) 
	{
		delete ( _p );
		( _p ) = nullptr;
	}
}


/*==============================================================*/
//! \brief ���S�ȃ������̉��
//! \tparam[in,out]		*&_p	������郁����
//! \param[in]			_Name	�������ϐ��̕ϐ���
/*==============================================================*/
template <typename T>
inline void SafeDelete( T *&_p , std::string _Name )
{
	if( _p != nullptr ) 
	{
		delete ( _p );
		( _p ) = nullptr;

		std::string String = ( boost::format( "%-25s�͈��S�Ƀ���������������܂���\n" ) % _Name.c_str() ).str();
		DEBUG->PrintConsole( String , CDebug::ConFree );
	}
}

/*===============================================================*/
//! \brief ���S�ȃ������̉��(�z��)
//! \tparam[in,out]		*&_p	������郁����
/*===============================================================*/
template <typename T>
inline void SafeDeleteArray( T *&_p )
{
	if( _p != nullptr ) 
	{
		delete[] ( _p );
		( _p ) = nullptr;
	}
}

/*===============================================================*/
//! \brief ���S�ȃ������̉��(�z��)
//! \tparam[in,out]		*&_p	������郁����
//! \param[in]			_Name	�������ϐ��̕ϐ���
/*===============================================================*/
template <typename T>
inline void SafeDeleteArray( T *&_p , std::string _Name )
{
	if( _p != nullptr ) 
	{
		delete[] ( _p );
		( _p ) = nullptr;

		std::string String = ( boost::format( "%-25s�͈��S�Ƀ���������������܂���\n" ) % _Name.c_str() ).str();
		DEBUG->PrintConsole( String , CDebug::ConFree );
	}
}

/*====================================================================*/
//! \brief ���S�ȃ������̉��(�Q�Ɖ񐔂�1���炷)
//! \tparam[in,out]		*&_p	������郁����
/*====================================================================*/
template <typename T>
inline void SafeRelease( T *&_p )
{
	if( _p != nullptr ) 
	{
		( _p )->Release();
		( _p ) = nullptr;
	}
}

/*====================================================================*/
//! \brief ���S�ȃ������̉��(�Q�Ɖ񐔂�1���炷)
//! \tparam[in,out]		*&_p	������郁����
//! \param[in]			_Name	�������ϐ��̕ϐ���
/*====================================================================*/
template <typename T>
inline void SafeRelease( T *&_p , std::string _Name )
{
	if( _p != nullptr ) 
	{
		( _p )->Release();
		( _p ) = nullptr;
	
		std::string String = ( boost::format( "%-25s�͈��S�Ƀ���������������܂���\n" ) % _Name.c_str() ).str();
		DEBUG->PrintConsole( String , CDebug::ConFree );
	}
}


/*============================================================*/
//! \struct VERTEX
//! \brief <b>�|���S���쐬���ȂǂɎg�p���钸�_���</b>
/*============================================================*/
typedef struct VERTEX
{
	D3DXVECTOR3 vPos;		//!< ���_���W
	DWORD color;			//!< ���_�F
	D3DXVECTOR2 Tex;		//!< �e�N�X�`�����W
}VERTEX;
static const int FVF_Vertex = ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );		//!< ���_�f�[�^�̍\��


/*=========================================================*/
//! \struct CLONEVECTOR
//! \brief <b>�N���[���쐬���Ɏg�p���钸�_���</b>
/*=========================================================*/
typedef struct CLONEVECTOR
{
	D3DXVECTOR3 Pos;		// ���W
	D3DXVECTOR3 Normal;		// �@��
	D3DXVECTOR2 Tex;		// �e�N�X�`��
}CLONEVECTOR;
static const int CustomFVF = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );


/*======================================================*/
//! \struct SORT_STRUCT
//! \brief <b>�`��\�[�g���Ɏg�p����\����</b>
/*======================================================*/
typedef struct SORT_STRUCT
{

	int ElementNum;		//!< vector�̓Y����
	float Distance;		//!< �J��������̋���

	bool operator<( const SORT_STRUCT &right ) const {
		return Distance == right.Distance ? ElementNum < right.ElementNum : Distance < right.Distance;
	}
	bool operator>( const SORT_STRUCT &left ) const {
		return Distance == left.Distance ? ElementNum > left.ElementNum : Distance > left.Distance;
	}

}SORT_STRUCT;


/*===========================================*/
//! \struct LASER_COLOR
//! \brief <b>���[�U�[�̐F���Ǘ�</b>
/*===========================================*/
typedef struct LASER_COLOR
{

	float Red;		//!< �ԗv�f
	float Green;	//!< �Ηv�f
	float Blue;		//!< �v�f
		
	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	LASER_COLOR( void )
	{
		Red   = 0.0f;
		Green = 0.0f;
		Blue  = 0.0f;
	}

	/*=======================================*/
	//! \brief �R���X�g���N�^
	/*=======================================*/
	LASER_COLOR( float _Red , float _Green , float _Blue )
	{
		Red   = _Red;
		Green = _Green;
		Blue  = _Blue;
	}

	LASER_COLOR operator-( const LASER_COLOR &_Color ) const {
		LASER_COLOR Tmp;
		Tmp.Red   = Red   - _Color.Red;
		Tmp.Green = Green - _Color.Green;
		Tmp.Blue  = Blue  - _Color.Blue;
		return Tmp;
	}

	template <typename T>
	LASER_COLOR operator/( const T &_div ) const {
		LASER_COLOR Tmp;
		Tmp.Red   = Red   / _div;
		Tmp.Green = Green / _div;
		Tmp.Blue  = Blue  / _div;
		return Tmp;
	}

	void operator+=( const LASER_COLOR &_Color ){
		LASER_COLOR Tmp;
		Red   += _Color.Red;
		Green += _Color.Green;
		Blue  += _Color.Blue;
	}

}LASER_COLOR;


/*===============================*/
// ���������[�N�̊Ď�
/*===============================*/
#ifdef _DEBUG
#include <cstdlib>
#include <new>
#include <memory>
using namespace std;
#define _CRTDBG_MAP_ALLOC
#define new ::new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif


/*===============================*/
// �f�o�b�O�A�G���[�֘A
/*===============================*/

/*==========================================*/
//! \def VARIABLE_DATA
//! \brief ������������Ɏg�p
/*==========================================*/
#define VARIABLE_DATA( _Var ) _Var , #_Var


/*===========================================*/
//! \brief �ȈՂ̈ꎞ��~
/*===========================================*/
inline void SimplifyStop( void )
{
	DEBUG->PrintConsole( "�m�F�̂��߈ꎞ�I�ɒ�~���܂�\n" , CDebug::ConWarning );
	DEBUG->PrintConsole( "Push Space.\n" , CDebug::ConWarning );
	while( !KEYBOARD->GetSimplifyState( VK_SPACE ) );
}


/*====================================================*/
//! \brief �G���[���b�Z�[�W�̕\��
//! \param[in]	_String		�\�����镶����
/*====================================================*/
inline void ErrorMessage( std::string _String )
{
#ifdef _DEBUG
		DEBUG->PrintConsole( _String , CDebug::ConWarning );
		SimplifyStop();
#else
		MessageBox( CAPP->GetHWND() , _String.c_str() , "�G���[" , MB_OK | MB_ICONWARNING | MB_TOPMOST );
#endif
}


#ifndef _DEBUG
#define MESSAGE(str) MessageBox( CAPP->GetHWND() , str , "�m�F"   , MB_YESNO | MB_TOPMOST )
#endif


/*================================*/
//! \brief �ً}�I������
/*================================*/
inline void SafeExit( void )
{

	// �C���X�^���X�𐶐����Ă��Ȃ��ꍇ�����𔲂���
	if( CApplication::GetObjectExist() == false ){ return; } 

	CAPP->Delete();
	ErrorMessage( "\n\n���S�ɃQ�[���𒆒f���܂���\n" );

}

