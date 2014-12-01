#include "stdafx.hpp"

#include "CDirect3D.hpp" 
#include "CLightManager.hpp"


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CLightManager::CLightManager( void )
	: m_pd3d( nullptr )
	, m_LightNum( 0 )
{
	m_pd3d = DIRECTX;
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CLightManager::~CLightManager( void )
{
}


/*===========================================================*/
//! \brief ���C�g��ǉ�����
//! \param[in]	_Light		�ǉ����郉�C�g
//! \param[in]	_Flag		���C�g��ON,OFF
//! \return �C���f�b�N�X�ԍ�
/*===========================================================*/
int CLightManager::AddLight( D3DLIGHT9 _Light , BOOL _Flag )
{
	m_LightList.push_back( _Light );
	m_pd3d->GetDevice()->SetLight( m_LightNum , &m_LightList[m_LightNum] );
	m_pd3d->GetDevice()->LightEnable( m_LightNum , _Flag );
	// ���̃��C�g�̂��߂ɃC���f�b�N�X�ԍ����X�V
	return m_LightNum++;
}


/*============================================================*/
//! \brief ���C�g�̏�Ԃ��X�V����
//! \param[in]	_Light		�X�V���郉�C�g
//! \param[in]	_Index		�X�V���郉�C�g�̃C���f�b�N�X�ԍ�
//! \param[in]	_Flag		���C�g��ON,OFF
/*============================================================*/
void CLightManager::UpdateLight( D3DLIGHT9 _Light , int _Index , BOOL _Flag )
{
	if( _Index < (int)m_LightList.size() && _Index >= 0 )
	{
		m_LightList[_Index] = _Light;
		m_pd3d->GetDevice()->SetLight( _Index , &m_LightList[_Index] );
		m_pd3d->GetDevice()->LightEnable( _Index , _Flag );
	}
}


/*===============================================================*/
//! \brief ���C�g�̐ݒ��ύX����
//! \param[in]	_Index		�ύX���郉�C�g�̃C���f�b�N�X�ԍ�
//! \param[in]	_Flag		���C�g��ON,OFF
/*===============================================================*/
void CLightManager::SetLightFlag( DWORD _Index , BOOL _Flag )
{
	if( _Index < (int)m_LightList.size() && _Index >= 0 )
	{
		m_pd3d->GetDevice()->LightEnable( _Index , _Flag );
	}
}


/*==============================================================*/
//! \brief ���ׂẴ��C�g�̐ݒ��ύX����
//! \param[in]	_Flag		���C�g��ON,OFF
/*==============================================================*/
void CLightManager::SetAllLightFlag( BOOL _Flag )
{
	for( int i=0 ; i<m_LightNum ; i++ )
		m_pd3d->GetDevice()->LightEnable( i , FALSE );
}


/*========================================*/
//! \brief ���C�g��S�ď���
/*========================================*/
void CLightManager::AllDelete( void )
{
	// ���C�g��S��OFF�ɂ���
	SetAllLightFlag( FALSE );

	m_LightList.clear();

	// �C���f�b�N�X�̏�����
	m_LightNum = 0;
}