#pragma once 

#include "../GimmickBase/CGimmickBase.hpp"


class CSurveillanceCamera : public CGimmickBase
{

	/*===============================================*/
	//!
	//! \file
	//! \class CSurveillanceCamera
	//! \brief <b>監視カメラの管理クラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date 2014/09/16
	//!
	/*===============================================*/

public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CSurveillanceCamera( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CSurveillanceCamera( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


	

	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	/*=============================================*/
	void Draw( CDirect3D & _Direct3D );


private:

	/*========================================================*/
	//! \brief 主人公が見当たらないので辺りを監視中
	/*========================================================*/
	void SearchMode( void );

	/*================================================*/
	//! \brief 主人公をロックオンした状態
	/*================================================*/
	void LockOnMode( void );


	static const int JointNum = 5;			//!< 関節の数

	D3DXVECTOR3 m_JointPos[JointNum];		//!< 次の関節の座標
	D3DXVECTOR3 m_Rot[JointNum];			//!< 各関節の向いている角度

	std::string m_XFilePass[JointNum+1];	//!< XFileのパス
	CXFile *m_lpXFiles[JointNum+1];			//!< XFileのポインタを格納
	NxActor *m_Actors[JointNum+1];			//!< アクターのポインタ

	CPlayer *m_pPlayer;						//!< プレイヤーの座標の確認のためにポインタを持っておく

	void ( CSurveillanceCamera::*pFuncMode )( void );	//!< 現在の行動パターンのメンバ関数ポインタ
	
	/*=================================================*/
	//! \enum SurveillanceCameraFuncType
	//! \brief 監視カメラの行動パターン
	/*=================================================*/
	enum SurveillanceCameraFuncType
	{
		SEARCH_MODE,	//!< 主人公を探し辺りを見渡す
		LOCK_ON_MODE	//!< 主人公を追尾
	};

	SurveillanceCameraFuncType m_FuncFlag;				//!< 現在の行動パターンのフラグ

	static const bool MoveLeft = true;
	static const bool MoveRight = false;
	bool MoveFlag;
};

