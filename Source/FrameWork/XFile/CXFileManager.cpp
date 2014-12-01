#include "stdafx.hpp"

#include "CXFileManager.hpp"
#include "CXFile.hpp"


/*=======================================*/
// CXFileManager�̃|�C���^�̒�`
/*=======================================*/
CXFileManager* CXFileManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CXFileManager::CXFileManager( void )
{
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CXFileManager::~CXFileManager( void )
{
	AllClear();
}


/*===============================================================*/
//! \brief CXFile�̓ǂݍ���
//! \param[in]		_lpDirect3D		DirectX�̃f�o�C�X
//! \param[in]		_FilePass		XFile������t�@�C���̃p�X
//! \return XFile�̃|�C���^
/*===============================================================*/
CXFile * CXFileManager::Load( LPDIRECT3DDEVICE9 _lpDirect3D , std::string _FilePass )
{

	// ���ɓǂݍ���ł��Ȃ���map���𑖍�
	if ( m_xmap.find( _FilePass ) != m_xmap.end() )
	{
		return m_xmap[_FilePass];
	}

	// �����ꍇ�V���ɓǂݍ���
	CXFile* lpx = new CXFile( _lpDirect3D , &_FilePass );
	m_xmap.insert( std::pair< std::string, CXFile *>( _FilePass , lpx ) );
	if( lpx == nullptr )
	{ 
		std::string sErrorMessage = _FilePass + "�̓ǂݍ��݂Ɏ��s���܂���\n";
		ErrorMessage( sErrorMessage );
	}
	return lpx;
	
}


/*==================================================*/
//! \brief �f�[�^��map�̒����瑖�����Ԃ�
//! \return ���� = XFile�̃|�C���^
//! \return ���s = nullptr
/*==================================================*/
CXFile * CXFileManager::Get( std::string _FilePass )
{
	if( m_xmap.find( _FilePass ) != m_xmap.end() )
	{
		return m_xmap[_FilePass];
	}
	return nullptr;
}


/*======================================================*/
//! \brief �ǂݍ���ł���CXFile��S�ĉ������
/*======================================================*/
void CXFileManager::AllClear( void )
{

	std::map< std::string, CXFile *>::iterator it;
	it = m_xmap.begin();

	if( m_xmap.size() != 0 )
	{
		DEBUG->PrintConsole( "===Release XFile===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		while( it != m_xmap.end() )
		{
			std::string XfileName = GetFileName( it->first );
			SafeDelete( it->second , XfileName );
			it++;
		}
		m_xmap.clear();
		DEBUG->PrintConsole( "================================================================\n\n" , CDebug::ConFence );
	}

}


/*====================================================*/
//! \brief �t�@�C������Ԃ�
//! \param[in]	_FilePass		�t�@�C���p�X
//! \param[in]	_Split			��؂蕶��
/*====================================================*/
std::string CXFileManager::GetFileName( std::string _FilePass , std::string _Split )
{

	int SeekPos;
	std::string Name = _FilePass;

	while( 1 )
	{

		// ���̋�؂蕶�����ǂ��ɂ���̂������A�Ȃ��ꍇ�����𔲂���
		if( ( SeekPos = Name.find( _Split ) ) == std::string::npos ){ break; }

		// ��؂蕶����ǂݔ�΂�
		Name = Name.substr( SeekPos + _Split.length() );

	}

	return Name;

}