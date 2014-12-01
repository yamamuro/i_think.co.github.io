#include "stdafx.hpp"

#include "CSoundManager.hpp"


/*==============================================*/
//! CSoundManagerのポインタの定義
/*==============================================*/
CSoundManager* CSoundManager::m_pInstance = nullptr;


/*===============================================*/
//! \brief デフォルトコンストラクタ
/*===============================================*/
CSoundManager::CSoundManager( void )
{
}


/*============================*/
//! \brief 初期化
/*============================*/
void CSoundManager::Initialize( void )
{

	DirectSoundCreate8( NULL , &lpDSound , NULL );

	//協調レベルを設定
	lpDSound->SetCooperativeLevel( CAPP->GetHWND() , DSSCL_PRIORITY );

	// プライマリ・バッファの作成
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc; 
	ZeroMemory( &dsbdesc , sizeof( DSBUFFERDESC ) );
	dsbdesc.dwSize = sizeof( DSBUFFERDESC ); 
	// プライマリ・バッファを指定
	dsbdesc.dwFlags = ( DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER );
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// バッファを作る
	lpDSound->CreateSoundBuffer( &dsbdesc , &lpSPrimary , NULL ); 

	// プライマリ・バッファのWaveフォーマットを設定
	// 優先協調レベル以上の協調レベルが設定されている必要があります．
	WAVEFORMATEX pcmwf; 
	ZeroMemory( &pcmwf , sizeof( WAVEFORMATEX ) ); 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM; 
	pcmwf.nChannels = 2;			// ２チャンネル（ステレオ）
	pcmwf.nSamplesPerSec = 44100;	// サンプリング・レート　44.1kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = 16;		// 16ビット
	lpSPrimary->SetFormat( &pcmwf );

	lpSPrimary->GetVolume( &Vol );
	lpSPrimary->GetPan( &Pan );
	CoInitialize( NULL );

}


/*=====================================*/
//! \brief デストラクタ
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
//! \brief 読み込み
//! \param[in]	_FilePass		Waveファイルのパス
//! \param[in]	_BuffNum		複製するバッファ数
//! \return 作成したCSoundのポインタ
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
// 読み込んでいるファイルを全て解放する
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