#pragma once

/*=====================================================================*/
//!
//! \file
//! \brief <b>頻繁に使用するものをまとめプリコンパイルしたもの</b>
//!
//! \author Y.Yamamuro \n
//! \date  2014/09/03
//! \since 2014/09/26
//!
/*=====================================================================*/


/*=================================*/
// インクルード
/*=================================*/

/*===========================*/
// 標準ライブラリ
/*===========================*/
#define NOMINMAX
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <random>


/*===========================*/
// WindowsAPI
/*===========================*/
#include <Windows.h>


/*===========================*/
// STL
/*===========================*/
#include <vector>
#include <map>


/*===============================*/
// Boost
/*===============================*/
#include <boost/thread.hpp>
#include <boost/format.hpp>


/*===============================*/
// DirectX
/*===============================*/
#include <d3dx9.h>
#include<dsound.h>
#pragma comment( lib , "dsound.lib" )
#pragma comment( lib , "d3d9.lib"   )
#pragma comment( lib , "d3dx9.lib"  )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "winmm.lib"  )


/*===============================*/
// PhysX
/*===============================*/
#pragma warning( disable : 4996 )
#pragma warning( disable : 4995 )
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "NxPhysics.h"


/*===============================*/
// フレームワーク
/*===============================*/
#include "Utility.hpp"
#include "System.hpp"
#include "Wrapper/Matrix/CMatrix.hpp"
#include "Wrapper/DirectX_PhysX_Converter/DirectX_PhysX_Converter.hpp"
#include "Wrapper/Random/CRandomize.hpp"
#include "Wrapper/Vector3/CVector.hpp"
#include "Wrapper/LoadFile/CLoadFile.hpp"
#include "Debug/CDebug.hpp"
#include "Debug/CDebugRender.hpp"
#include "Input/Keyboard/CKeyboard.hpp"
#include "Input/Mouse/CMouse.hpp"
#include "Texture/CTexture.hpp"
#include "Texture/CTextureManager.hpp"
#include "XFile/CXFile.hpp"
#include "XFile/CXFileManager.hpp"
#include "PhysX/CPhysX.hpp"
#include "Sound/CSoundManager.hpp"
#include "Sound/CSound.hpp"
#include "Sound/wavread.h"
#include "Direct3D/CSprite.hpp"
#include "Direct3D/CDirect3D.hpp"
#include "Direct3D/CLightManager.hpp"
#include "../Application/CApplication.hpp"


