#include "stdafx.hpp"

#include<dsound.h>
#pragma comment( lib , "dsound.lib" )
#include "CSound.hpp"


/*====================================================================*/
//! \brief �R���X�g���N�^
//! \param[in]	*_FilePass		Wave�t�@�C���̃p�X
//! \param[in]	_lpDSound		DirectSound�̃C���^�[�t�F�C�X
//! \param[in]	_BuffNum		�쐬����o�b�t�@��
/*====================================================================*/
CSound::CSound( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum )
	: BuffNum( 1 )
	, lpSSecond( nullptr )
{
	Load( _FilePass , _lpDSound , _BuffNum );
}


/*=====================================*/
//! \brief �f�X�g���N�^
/*=====================================*/
CSound::~CSound( void )
{

	for( int i=0 ; i<BuffNum ; i++ )
	{
		if( lpSSecond != nullptr )
		{
			lpSSecond[i]->Release();
			lpSSecond[i] = nullptr;
		}
	}
	SafeDeleteArray( VARIABLE_DATA( lpSSecond ) );
	
}


/*====================================================================*/
//! \brief Wave�t�@�C����ǂݍ���
//! \param[in]	*_FilePass		Wave�t�@�C���̃p�X
//! \param[in]	_lpDSound		DirectSound�̃C���^�[�t�F�C�X
//! \param[in]	_BuffNum		�쐬����o�b�t�@��
/*====================================================================*/
void CSound::Load( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum )
{

	lpSSecond = new LPDIRECTSOUNDBUFFER8[BuffNum]; 

	LoadWAVE( _FilePass , _lpDSound );

	lpSSecond[0]->GetFrequency( &Hz );

	LPDIRECTSOUNDBUFFER lpSTmp;

	BuffNum = _BuffNum;

	//�T�E���h�o�b�t�@�𕡐�
	for( int i=1 ; i<BuffNum ; i++ ){

		_lpDSound->DuplicateSoundBuffer( lpSSecond[0] , &lpSTmp );
		lpSTmp->QueryInterface( IID_IDirectSoundBuffer8 , (LPVOID*)&lpSSecond[i] );

		lpSTmp->Release();

	}

}


/*===================================================================*/
//! \brief �����Ŏ��ۂ�Wave�t�@�C����ǂݍ���
//! \param[in]	*_FilePass		Wave�t�@�C���̃p�X
//! \param[in]	_lpDSound		DirectSound�̃C���^�[�t�F�C�X
/*===================================================================*/
void CSound::LoadWAVE( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound )
{

	HRESULT hr;

	// WAVE�t�@�C�����J��
	CWaveSoundRead WaveFile;
	if( WaveFile.Open( (CHAR*)_FilePass->c_str() ) ){ return; }

	// �Z�J���_���E�o�b�t�@���쐬����
	// DSBUFFERDESC�\���̂�ݒ�
	DSBUFFERDESC dsbdesc; 
	ZeroMemory( &dsbdesc , sizeof( DSBUFFERDESC ) );
	dsbdesc.dwSize = sizeof( DSBUFFERDESC ); 
	
	dsbdesc.dwFlags = ( DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
						DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY );
	dsbdesc.dwBufferBytes = WaveFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat   = WaveFile.m_pwfx;

	// �o�b�t�@�����
	LPDIRECTSOUNDBUFFER pDSTmp;
	_lpDSound->CreateSoundBuffer( &dsbdesc , &pDSTmp , NULL ); 
	pDSTmp->QueryInterface( IID_IDirectSoundBuffer8 , (LPVOID*)&lpSSecond[0] );
	pDSTmp->Release();

	// �Z�J���_���E�o�b�t�@��Wave�f�[�^����������
	LPVOID lpvPtr1;		// �ŏ��̃u���b�N�̃|�C���^
	DWORD dwBytes1;		// �ŏ��̃u���b�N�̃T�C�Y
	LPVOID lpvPtr2;		// �Q�Ԗڂ̃u���b�N�̃|�C���^
	DWORD dwBytes2;		// �Q�Ԗڂ̃u���b�N�̃T�C�Y

	hr = lpSSecond[0]->Lock( 0 , WaveFile.m_ckIn.cksize , &lpvPtr1 , &dwBytes1 , &lpvPtr2 , &dwBytes2 , 0 ); 

	// DSERR_BUFFERLOST���Ԃ��ꂽ�ꍇ�CRestore���\�b�h���g���ăo�b�t�@�𕜌�����
	if( DSERR_BUFFERLOST == hr )
	{
		lpSSecond[0]->Restore();
		hr = lpSSecond[0]->Lock( 0 , WaveFile.m_ckIn.cksize , &lpvPtr1 , &dwBytes1 , &lpvPtr2 , &dwBytes2 , 0 );
	} 
	if ( SUCCEEDED( hr ) )
	{

		// �����ŁC�o�b�t�@�ɏ�������
		// �o�b�t�@�Ƀf�[�^���R�s�[����
		UINT rsize;
		WaveFile.Read( dwBytes1 , (LPBYTE)lpvPtr1 , &rsize );
		if ( dwBytes2 != 0 )
			WaveFile.Read( dwBytes2 , (LPBYTE)lpvPtr2 , &rsize );

		// �������݂��I������炷����Unlock����D
		hr = lpSSecond[0]->Unlock( lpvPtr1 , dwBytes1 , lpvPtr2 , dwBytes2 ); 
	}

}


/*===================================*/
//! \brief ����ʏ�Đ�����
/*===================================*/
void CSound::Play( void )
{ 
	//if( CAPP->GetCDebug() == nullptr ? 1 : !CAPP->GetCDebug()->GetDebugFlag() 
	//	|| bSoundPlaying )
	//{
	//	lpSSecond[0]->Play( 0 , 0 , 0 );
	//}
}

/*===================================*/
//! \brief �������[�v�Đ�����
/*===================================*/
void CSound::RePlay( void )
{ 
	//if( CAPP->GetCDebug() == nullptr ? 1 : !CAPP->GetCDebug()->GetDebugFlag() 
	//	|| bSoundPlaying )
	//{
	//	DWORD SFlag;
	//	lpSSecond[0]->GetStatus( &SFlag );
	//	if( !( DSBSTATUS_PLAYING &SFlag ) )
	//	{
	//		lpSSecond[0]->Play( 0 , 0 , 0 );
	//	}
	//}
}

/*===================================*/
//! \brief �Đ����Ă��鉹���~�߂�
/*===================================*/
void CSound::Stop( void )
{
	//if( CAPP->GetCDebug() == nullptr ? 1 : !CAPP->GetCDebug()->GetDebugFlag() 
	//	|| bSoundPlaying )
	//{
	//	lpSSecond[0]->Stop();
	//}
}