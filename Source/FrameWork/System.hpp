#pragma once

/*======================================================================*/
//!
//! \file
//! \brief <b>変更する可能性のあるアプリケーションデータを宣言</b>
//!
//! \author	Y.Yamamuro \n
//! \date  2014/09/03
//! \since 2014/09/21
//!
/*======================================================================*/

// ゲームタイトル
static const LPCSTR		 WINDOW_NAME = "Window";		//!< ウィンドウ名
static const std::string GAME_TITLE	 = "I think";		//!< ゲーム名

// スクリーンの設定
// Releaseの時は以下に関係なくユーザーに確認を行う
static const bool bFullScreen = false;					//!< フルスクリーンにするかどうかのフラグ

// サウンド
static const bool bSoundPlaying = true;				//!< BGM,SEを再生するかのフラグ

// ウィンドウサイズ
static const int WINDOW_WIDTH  = 800;					//!< ウィンドウ幅
static const int WINDOW_HEIGHT = 600;					//!< ウィンドウ高さ

// ウィンドウの表示位置(中心位置を基準とする)
static const int WINDOW_POSITION_X = 0;					//!< X方向へどれだけずらすか
static const int WINDOW_POSITION_Y = 0;					//!< Y方向へどれだけずらすか

// FPS
static const int FRAME_RATE = 60;						//!< フレームレートの設定
