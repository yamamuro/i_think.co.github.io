#pragma once

#include "Input/Keyboard/CKeyboard.hpp"
#include "Debug/CDebug.hpp"
#include "../Application/CApplication.hpp"

/*=================================*/
// テンプレートの宣言
/*=================================*/

/*==============================================================*/
//! \brief 安全なメモリの解放
//! \tparam[in,out]		*&_p	解放するメモリ
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
//! \brief 安全なメモリの解放
//! \tparam[in,out]		*&_p	解放するメモリ
//! \param[in]			_Name	解放する変数の変数名
/*==============================================================*/
template <typename T>
inline void SafeDelete( T *&_p , std::string _Name )
{
	if( _p != nullptr ) 
	{
		delete ( _p );
		( _p ) = nullptr;

		std::string String = ( boost::format( "%-25sは安全にメモリから解放されました\n" ) % _Name.c_str() ).str();
		DEBUG->PrintConsole( String , CDebug::ConFree );
	}
}

/*===============================================================*/
//! \brief 安全なメモリの解放(配列)
//! \tparam[in,out]		*&_p	解放するメモリ
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
//! \brief 安全なメモリの解放(配列)
//! \tparam[in,out]		*&_p	解放するメモリ
//! \param[in]			_Name	解放する変数の変数名
/*===============================================================*/
template <typename T>
inline void SafeDeleteArray( T *&_p , std::string _Name )
{
	if( _p != nullptr ) 
	{
		delete[] ( _p );
		( _p ) = nullptr;

		std::string String = ( boost::format( "%-25sは安全にメモリから解放されました\n" ) % _Name.c_str() ).str();
		DEBUG->PrintConsole( String , CDebug::ConFree );
	}
}

/*====================================================================*/
//! \brief 安全なメモリの解放(参照回数を1減らす)
//! \tparam[in,out]		*&_p	解放するメモリ
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
//! \brief 安全なメモリの解放(参照回数を1減らす)
//! \tparam[in,out]		*&_p	解放するメモリ
//! \param[in]			_Name	解放する変数の変数名
/*====================================================================*/
template <typename T>
inline void SafeRelease( T *&_p , std::string _Name )
{
	if( _p != nullptr ) 
	{
		( _p )->Release();
		( _p ) = nullptr;
	
		std::string String = ( boost::format( "%-25sは安全にメモリから解放されました\n" ) % _Name.c_str() ).str();
		DEBUG->PrintConsole( String , CDebug::ConFree );
	}
}


/*============================================================*/
//! \struct VERTEX
//! \brief <b>板ポリゴン作成時などに使用する頂点情報</b>
/*============================================================*/
typedef struct VERTEX
{
	D3DXVECTOR3 vPos;		//!< 頂点座標
	DWORD color;			//!< 頂点色
	D3DXVECTOR2 Tex;		//!< テクスチャ座標
}VERTEX;
static const int FVF_Vertex = ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );		//!< 頂点データの構成


/*=========================================================*/
//! \struct CLONEVECTOR
//! \brief <b>クローン作成時に使用する頂点情報</b>
/*=========================================================*/
typedef struct CLONEVECTOR
{
	D3DXVECTOR3 Pos;		// 座標
	D3DXVECTOR3 Normal;		// 法線
	D3DXVECTOR2 Tex;		// テクスチャ
}CLONEVECTOR;
static const int CustomFVF = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );


/*======================================================*/
//! \struct SORT_STRUCT
//! \brief <b>描画ソート時に使用する構造体</b>
/*======================================================*/
typedef struct SORT_STRUCT
{

	int ElementNum;		//!< vectorの添え字
	float Distance;		//!< カメラからの距離

	bool operator<( const SORT_STRUCT &right ) const {
		return Distance == right.Distance ? ElementNum < right.ElementNum : Distance < right.Distance;
	}
	bool operator>( const SORT_STRUCT &left ) const {
		return Distance == left.Distance ? ElementNum > left.ElementNum : Distance > left.Distance;
	}

}SORT_STRUCT;


/*===========================================*/
//! \struct LASER_COLOR
//! \brief <b>レーザーの色を管理</b>
/*===========================================*/
typedef struct LASER_COLOR
{

	float Red;		//!< 赤要素
	float Green;	//!< 緑要素
	float Blue;		//!< 青要素
		
	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	LASER_COLOR( void )
	{
		Red   = 0.0f;
		Green = 0.0f;
		Blue  = 0.0f;
	}

	/*=======================================*/
	//! \brief コンストラクタ
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
// メモリリークの監視
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
// デバッグ、エラー関連
/*===============================*/

/*==========================================*/
//! \def VARIABLE_DATA
//! \brief メモリ解放時に使用
/*==========================================*/
#define VARIABLE_DATA( _Var ) _Var , #_Var


/*===========================================*/
//! \brief 簡易の一時停止
/*===========================================*/
inline void SimplifyStop( void )
{
	DEBUG->PrintConsole( "確認のため一時的に停止します\n" , CDebug::ConWarning );
	DEBUG->PrintConsole( "Push Space.\n" , CDebug::ConWarning );
	while( !KEYBOARD->GetSimplifyState( VK_SPACE ) );
}


/*====================================================*/
//! \brief エラーメッセージの表示
//! \param[in]	_String		表示する文字列
/*====================================================*/
inline void ErrorMessage( std::string _String )
{
#ifdef _DEBUG
		DEBUG->PrintConsole( _String , CDebug::ConWarning );
		SimplifyStop();
#else
		MessageBox( CAPP->GetHWND() , _String.c_str() , "エラー" , MB_OK | MB_ICONWARNING | MB_TOPMOST );
#endif
}


#ifndef _DEBUG
#define MESSAGE(str) MessageBox( CAPP->GetHWND() , str , "確認"   , MB_YESNO | MB_TOPMOST )
#endif


/*================================*/
//! \brief 緊急終了処理
/*================================*/
inline void SafeExit( void )
{

	// インスタンスを生成していない場合処理を抜ける
	if( CApplication::GetObjectExist() == false ){ return; } 

	CAPP->Delete();
	ErrorMessage( "\n\n安全にゲームを中断しました\n" );

}

