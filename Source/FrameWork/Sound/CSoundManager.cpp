#include "stdafx.hpp"

#include "CSoundManager.hpp"


/*==============================================*/
//! CSoundManager�̃|�C���^�̒�`
/*==============================================*/
CSoundManager* CSoundManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief �f�t�H���g�R���X�g���N�^
/*===============================================*/
CSoundManager::CSoundManager( void )
{
}


/*============================*/
//! \brief ������
/*============================*/
void CSoundManager::Initialize( void )
{

	DirectSoundCreate8( NULL , &lpDSound , NULL );

	//�������x����ݒ�
	lpDSound->SetCooperativeLevel( CAPP->GetHWND() , DSSCL_PRIORITY );

	// �v���C�}���E�o�b�t�@�̍쐬
	// DSBUFFERDESC�\���̂�ݒ�
	DSBUFFERDESC dsbdesc; 
	ZeroMemory( &dsbdesc , sizeof( DSBUFFERDESC ) );
	dsbdesc.dwSize = sizeof( DSBUFFERDESC ); 
	// �v���C�}���E�o�b�t�@���w��
	dsbdesc.dwFlags = ( DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER );
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// �o�b�t�@�����
	lpDSound->CreateSoundBuffer( &dsbdesc , &lpSPrimary , NULL ); 

	// �v���C�}���E�o�b�t�@��Wave�t�H�[�}�b�g��ݒ�
	// �D�拦�����x���ȏ�̋������x�����ݒ肳��Ă���K�v������܂��D
	WAVEFORMATEX pcmwf; 
	ZeroMemory( &pcmwf , sizeof( WAVEFORMATEX ) ); 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM; 
	pcmwf.nChannels = 2;			// �Q�`�����l���i�X�e���I�j
	pcmwf.nSamplesPerSec = 44100;	// �T���v�����O�E���[�g�@44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = 16;		// 16�r�b�g
	lpSPrimary->SetFormat( &pcmwf );

	lpSPrimary->GetVolume( &Vol );
	lpSPrimary->GetPan( &Pan );
	CoInitialize( NULL );

}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CSoundManager::~CSoundManager( void )
{
	
	lpSPrimary->SetVolume( Vol );
	lpSPrimary->SetPan( Pan );

	ReleaseWaves();

	lpSPrimary->Release();
	lpDSound->Release();

}


/*===============================================================*/
//! \brief �ǂݍ���
//! \param[in]	_FilePass		Wave�t�@�C���̃p�X
//! \param[in]	_BuffNum		��������o�b�t�@��
//! \return �쐬����CSound�̃|�C���^
/*===============================================================*/
CSound* CSoundManager::Load( std::string _FilePass , int _BuffNum )
{

	//CSound* pTmp;

	//_FilePass = "Resource/Sound/" + _FilePass;
	//pTmp = new CSound( &_FilePass , lpDSound , _BuffNum );

	//m_SoundList.insert( std::make_pair( _FilePass , pTmp ) );

	//return pTmp;

	return nullptr;

}


/*===============================================*/
// �ǂݍ���ł���t�@�C����S�ĉ������
/*===============================================*/
void CSoundManager::ReleaseWaves( void )
{
	if( m_SoundList.size() != 0 )
	{
		DEBUG->PrintConsole( "===Release Sound===\n" , CDebug::ConPreside );
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		for( auto& x : m_SoundList )
		{
			x.second->~CSound();
			SafeDelete( VARIABLE_DATA( x.second ) );
		}
		m_SoundList.clear();
		DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
		DEBUG->PrintConsole( "\n" );
	}
}