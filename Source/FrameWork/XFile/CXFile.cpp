#include "stdafx.hpp"

#include "Texture/CTexture.hpp"
#include "CXFile.hpp"
#pragma warning( disable : 4995 )


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CXFile::CXFile( void )
{
	Clear();
}

/*===========================================================*/
//! \brief �ǂݍ��݂𓯎��ɍs���R���X�g���N�^
//! \param[in]	lpD3DDevice		DirectX�̃f�o�C�X
//! \param[in]	*_FilePass		�ǂݍ���XFile�̃p�X
/*===========================================================*/
CXFile::CXFile( LPDIRECT3DDEVICE9 lpD3DDevice , std::string *_FilePass )
{
	Clear();
	Load( lpD3DDevice , *_FilePass );
}

/*================================*/
//! \brief ������
/*================================*/
void CXFile::Clear( void )
{
	xData.lpMesh		= nullptr;
	xData.NumMaterial	= 0;
	xData.Mat			= nullptr;
	xData.Tex			= nullptr;
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CXFile::~CXFile( void )
{
	Delete(); // XFile�̉��
}


/*=============================================================*/
//! \brief XFile�̓ǂݍ��݂��s��
//! \param[in]		lpD3DDevice		DirectX�̃f�o�C�X
//! \param[in]		_FilePass		�t�@�C���p�X
//! \return ���� = true
//! \return ���s = false
/*=============================================================*/
bool CXFile::Load( LPDIRECT3DDEVICE9 lpD3DDevice , std::string _FilePass )
{
	m_FullFileName = _FilePass; // �t���p�X��string�ɋN�����Ă���

	LPD3DXBUFFER lpD3DXBuffer;

	//lpTmpMesh�Ƀ��b�V�������[�h
	LPD3DXBUFFER lpAdjacency;
	LPD3DXMESH lpTmpMesh = nullptr;
	D3DXLoadMeshFromX( _FilePass.c_str() , D3DXMESH_MANAGED , lpD3DDevice , &lpAdjacency , &lpD3DXBuffer , NULL , &(xData.NumMaterial) , &lpTmpMesh );
	D3DERR_INVALIDCALL;

	//lpTmpMesh��Optimize(�œK��)����xData.lpMesh�֍쐬
	if( lpTmpMesh != nullptr )
	{
		HRESULT HrOpt;
		HrOpt = lpTmpMesh->Optimize( D3DXMESH_MANAGED | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE , (DWORD*)lpAdjacency->GetBufferPointer() , NULL , NULL , NULL , &(xData.lpMesh) );
		if( SUCCEEDED( HrOpt ) ) 
		{
			//lpTmpMesh�����
			lpTmpMesh->Release();
		}
		else
		{
			//�œK���Ɏ��s������œK���O�̂��̂��g�p����
			xData.lpMesh=lpTmpMesh;
		}

		//lpAdjacency�����
		lpAdjacency->Release();

	}
	else
	{
		std::string sErrorMessage = "���b�V���̓ǂݍ��ݎ��ɃG���[���������܂����B\n" + _FilePass + "��XFile�����m�F��������";
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

		// �}�e���A������t��
		xData.Mat[i] = D3DXMat[i].MatD3D;
		xData.Mat[i].Ambient = xData.Mat[i].Diffuse;
		xData.Mat[i].Emissive.r /= 3;
		xData.Mat[i].Emissive.g /= 3;
		xData.Mat[i].Emissive.b /= 3;

		// �e�N�X�`���̓ǂݍ���
		std::string FilePass;
		if( D3DXMat[i].pTextureFilename != nullptr )
			 FilePass = D3DXMat[i].pTextureFilename;
		xData.Tex[i] = TEXTURE_MANAGER->Load( &FilePass );

	}

	lpD3DXBuffer->Release();
	return true;

}


/*=================================*/
//! \brief XFile�̕`��
/*=================================*/
void CXFile::Draw( LPDIRECT3DDEVICE9 lpD3DDevice )
{
	DWORD i;

	for( i=0 ; i<xData.NumMaterial ; i++ ){

		// �}�e���A���̃Z�b�g
		lpD3DDevice->SetMaterial( &(xData.Mat[i]) );

		// �e�N�X�`���̓o�^,nullptr�̏ꍇ�̓e�N�X�`���Ȃ��Ɣ��f����
		lpD3DDevice->SetTexture( 0 , xData.Tex[i]->GetTexture() );

		// �e�N�X�`���̕`��
		xData.lpMesh->DrawSubset( i );

	}

}


/*=================================*/
//! \brief XFile�̉��
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

