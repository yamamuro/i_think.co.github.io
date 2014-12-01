#include "stdafx.hpp"

#include "Texture/CTexture.hpp"
#include "CXFile.hpp"
#pragma warning( disable : 4995 )


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CXFile::CXFile( void )
{
	Clear();
}

/*===========================================================*/
//! \brief 読み込みを同時に行うコンストラクタ
//! \param[in]	lpD3DDevice		DirectXのデバイス
//! \param[in]	*_FilePass		読み込むXFileのパス
/*===========================================================*/
CXFile::CXFile( LPDIRECT3DDEVICE9 lpD3DDevice , std::string *_FilePass )
{
	Clear();
	Load( lpD3DDevice , *_FilePass );
}

/*================================*/
//! \brief 初期化
/*================================*/
void CXFile::Clear( void )
{
	xData.lpMesh		= nullptr;
	xData.NumMaterial	= 0;
	xData.Mat			= nullptr;
	xData.Tex			= nullptr;
}


/*=====================================*/
//! \brief デストラクタ
/*=====================================*/
CXFile::~CXFile( void )
{
	Delete(); // XFileの解放
}


/*=============================================================*/
//! \brief XFileの読み込みを行う
//! \param[in]		lpD3DDevice		DirectXのデバイス
//! \param[in]		_FilePass		ファイルパス
//! \return 成功 = true
//! \return 失敗 = false
/*=============================================================*/
bool CXFile::Load( LPDIRECT3DDEVICE9 lpD3DDevice , std::string _FilePass )
{
	m_FullFileName = _FilePass; // フルパスをstringに起こしておく

	LPD3DXBUFFER lpD3DXBuffer;

	//lpTmpMeshにメッシュをロード
	LPD3DXBUFFER lpAdjacency;
	LPD3DXMESH lpTmpMesh = nullptr;
	D3DXLoadMeshFromX( _FilePass.c_str() , D3DXMESH_MANAGED , lpD3DDevice , &lpAdjacency , &lpD3DXBuffer , NULL , &(xData.NumMaterial) , &lpTmpMesh );
	D3DERR_INVALIDCALL;

	//lpTmpMeshをOptimize(最適化)してxData.lpMeshへ作成
	if( lpTmpMesh != nullptr )
	{
		HRESULT HrOpt;
		HrOpt = lpTmpMesh->Optimize( D3DXMESH_MANAGED | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE , (DWORD*)lpAdjacency->GetBufferPointer() , NULL , NULL , NULL , &(xData.lpMesh) );
		if( SUCCEEDED( HrOpt ) ) 
		{
			//lpTmpMeshを解放
			lpTmpMesh->Release();
		}
		else
		{
			//最適化に失敗したら最適化前のものを使用する
			xData.lpMesh=lpTmpMesh;
		}

		//lpAdjacencyを解放
		lpAdjacency->Release();

	}
	else
	{
		std::string sErrorMessage = "メッシュの読み込み時にエラーが発生しました。\n" + _FilePass + "のXFileをご確認ください";
		ErrorMessage( sErrorMessage );
		PostQuitMessage( 0 );
		return false;

	}

//	Direct3DXLoadMeshFromX(FName,Direct3DXMESH_MANAGED,lpD3DDevice,NULL,&lpDirect3DXBuffer,NULL,&(xData.NumMaterial),&(xData.lpMesh));

	xData.Mat = new D3DMATERIAL9[xData.NumMaterial];
	xData.Tex = new CTexture*[xData.NumMaterial];

	D3DXMATERIAL* D3DXMat = (D3DXMATERIAL*)lpD3DXBuffer->GetBufferPointer();

	DWORD i;
	for( i=0 ; i<xData.NumMaterial ; i++ ){

		// マテリアル情報を付加
		xData.Mat[i] = D3DXMat[i].MatD3D;
		xData.Mat[i].Ambient = xData.Mat[i].Diffuse;
		xData.Mat[i].Emissive.r /= 3;
		xData.Mat[i].Emissive.g /= 3;
		xData.Mat[i].Emissive.b /= 3;

		// テクスチャの読み込み
		std::string FilePass;
		if( D3DXMat[i].pTextureFilename != nullptr )
			 FilePass = D3DXMat[i].pTextureFilename;
		xData.Tex[i] = TEXTURE_MANAGER->Load( &FilePass );

	}

	lpD3DXBuffer->Release();
	return true;

}


/*=================================*/
//! \brief XFileの描画
/*=================================*/
void CXFile::Draw( LPDIRECT3DDEVICE9 lpD3DDevice )
{
	DWORD i;

	for( i=0 ; i<xData.NumMaterial ; i++ ){

		// マテリアルのセット
		lpD3DDevice->SetMaterial( &(xData.Mat[i]) );

		// テクスチャの登録,nullptrの場合はテクスチャなしと判断する
		lpD3DDevice->SetTexture( 0 , xData.Tex[i]->GetTexture() );

		// テクスチャの描画
		xData.lpMesh->DrawSubset( i );

	}

}


/*=================================*/
//! \brief XFileの解放
/*=================================*/
void CXFile::Delete( void )
{
	if( xData.lpMesh != nullptr )
	{
		SafeDeleteArray( xData.Mat );
		SafeDeleteArray( xData.Tex );
		SafeRelease( xData.lpMesh );
	}
}

