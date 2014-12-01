#include "stdafx.hpp"

#include<dsound.h>
#pragma comment( lib , "dsound.lib" )
#include "CSound.hpp"


/*====================================================================*/
//! \brief コンストラクタ
//! \param[in]	*_FilePass		Waveファイルのパス
//! \param[in]	_lpDSound		DirectSoundのインターフェイス
//! \param[in]	_BuffNum		作成するバッファ数
/*====================================================================*/
CSound::CSound( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum )
	: BuffNum( 1 )
	, lpSSecond( nullptr )
{
	Load( _FilePass , _lpDSound , _BuffNum );
}


/*=====================================*/
//! \brief デストラクタ
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
//! \brief Waveファイルを読み込む
//! \param[in]	*_FilePass		Waveファイルのパス
//! \param[in]	_lpDSound		DirectSoundのインターフェイス
//! \param[in]	_BuffNum		作成するバッファ数
/*====================================================================*/
void CSound::Load( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum )
{

	lpSSecond = new LPDIRECTSOUNDBUFFER8[BuffNum]; 

	LoadWAVE( _FilePass , _lpDSound );

	lpSSecond[0]->GetFrequency( &Hz );

	LPDIRECTSOUNDBUFFER lpSTmp;

	BuffNum = _BuffNum;

	//サウンドバッファを複製
	for( int i=1 ; i<BuffNum ; i++ ){

		_lpDSound->DuplicateSoundBuffer( lpSSecond[0] , &lpSTmp );
		lpSTmp->QueryInterface( IID_IDirectSoundBuffer8 , (LPVOID*)&lpSSecond[i] );

		lpSTmp->Release();

	}

}


/*===================================================================*/
//! \brief ここで実際にWaveファイルを読み込む
//! \param[in]	*_FilePass		Waveファイルのパス
//! \param[in]	_lpDSound		DirectSoundのインターフェイス
/*===================================================================*/
void CSound::LoadWAVE( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound )
{

	HRESULT hr;

	// WAVEファイルを開く
	CWaveSoundRead WaveFile;
	if( WaveFile.Open( (CHAR*)_FilePass->c_str() ) ){ return; }

	// セカンダリ・バッファを作成する
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc; 
	ZeroMemory( &dsbdesc , sizeof( DSBUFFERDESC ) );
	dsbdesc.dwSize = sizeof( DSBUFFERDESC ); 
	
	dsbdesc.dwFlags = ( DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
						DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY );
	dsbdesc.dwBufferBytes = WaveFile.m_ckIn.cksize;
	dsbdesc.lpwfxFormat   = WaveFile.m_pwfx;

	// バッファを作る
	LPDIRECTSOUNDBUFFER pDSTmp;
	_lpDSound->CreateSoundBuffer( &dsbdesc , &pDSTmp , NULL ); 
	pDSTmp->QueryInterface( IID_IDirectSoundBuffer8 , (LPVOID*)&lpSSecond[0] );
	pDSTmp->Release();

	// セカンダリ・バッファにWaveデータを書き込む
	LPVOID lpvPtr1;		// 最初のブロックのポインタ
	DWORD dwBytes1;		// 最初のブロックのサイズ
	LPVOID lpvPtr2;		// ２番目のブロックのポインタ
	DWORD dwBytes2;		// ２番目のブロックのサイズ

	hr = lpSSecond[0]->Lock( 0 , WaveFile.m_ckIn.cksize , &lpvPtr1 , &dwBytes1 , &lpvPtr2 , &dwBytes2 , 0 ); 

	// DSERR_BUFFERLOSTが返された場合，Restoreメソッドを使ってバッファを復元する
	if( DSERR_BUFFERLOST == hr )
	{
		lpSSecond[0]->Restore();
		hr = lpSSecond[0]->Lock( 0 , WaveFile.m_ckIn.cksize , &lpvPtr1 , &dwBytes1 , &lpvPtr2 , &dwBytes2 , 0 );
	} 
	if ( SUCCEEDED( hr ) )
	{

		// ここで，バッファに書き込む
		// バッファにデータをコピーする
		UINT rsize;
		WaveFile.Read( dwBytes1 , (LPBYTE)lpvPtr1 , &rsize );
		if ( dwBytes2 != 0 )
			WaveFile.Read( dwBytes2 , (LPBYTE)lpvPtr2 , &rsize );

		// 書き込みが終わったらすぐにUnlockする．
		hr = lpSSecond[0]->Unlock( lpvPtr1 , dwBytes1 , lpvPtr2 , dwBytes2 ); 
	}

}


/*===================================*/
//! \brief 音を通常再生する
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
//! \brief 音をループ再生する
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
//! \brief 再生している音を止める
/*===================================*/
void CSound::Stop( void )
{
	//if( CAPP->GetCDebug() == nullptr ? 1 : !CAPP->GetCDebug()->GetDebugFlag() 
	//	|| bSoundPlaying )
	//{
	//	lpSSecond[0]->Stop();
	//}
}