#pragma once

class CSound
{

	/*===================================================*/
	//!
	//! \file
	//! \class CSound
	//! \brief <b>1つのサウンドの管理</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/06
	//! \since 2014/10/06
	//!
	/*===================================================*/

public:

	/*====================================================================*/
	//! \brief コンストラクタ
	//! \param[in]	*_FilePass		Waveファイルのパス
	//! \param[in]	_lpDSound		DirectSoundのインターフェイス
	//! \param[in]	_BuffNum		作成するバッファ数
	/*====================================================================*/
	CSound( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CSound( void );


	/*====================================================================*/
	//! \brief Waveファイルを読み込む
	//! \param[in]	*_FilePass		Waveファイルのパス
	//! \param[in]	_lpDSound		DirectSoundのインターフェイス
	//! \param[in]	_BuffNum		作成するバッファ数
	/*====================================================================*/
	void Load( const std::string *_FilePass , const LPDIRECTSOUND8 _lpDSound , const int _BuffNum );


	/*===================================*/
	//! \brief 音を通常再生する
	/*===================================*/
	void Play( void );


	/*===================================*/
	//! \brief 音をループ再生する
	/*===================================*/
	void RePlay( void );


	/*===================================*/
	//! \brief 再生している音を止める
	/*===================================*/
	void Stop( void );


	/*===================================*/
	//! \brief バッファ数を返す
	/*===================================*/
	inline int GetBuffNum( void ){ return BuffNum; }


private:

	/*===================================================================*/
	//! \brief ここで実際にWaveファイルを読み込む
	//! \param[in]	*_FilePass		Waveファイルのパス
	//! \param[in]	_lpDSound		DirectSoundのインターフェイス
	/*===================================================================*/
	void LoadWAVE( const std::string *_FilePass , const LPDIRECTSOUND8 lpDSound );


	int BuffNum;						//!< 複製するバッファ数
	LPDIRECTSOUNDBUFFER8 *lpSSecond;	//!< セカンダリバッファのポインタを格納
	DWORD Hz;							//!< 初期Hzを格納

};