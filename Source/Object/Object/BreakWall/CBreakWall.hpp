#pragma once

#include "../Object/ObjectBase/CObjectBase.hpp"

/*===================================*/
//!< 前方宣言
/*===================================*/
class CFragment;

class CBreakWall : public CObjectBase
{

	/*===============================================*/
	//!	
	//! \file
	//! \class CBreakWall
	//!	\brief <b>重い物を乗せると壊れる床</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/21
	//! \since 2014/10/22
	//!
	/*================================================*/

public:
	
	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CBreakWall( void );

	/*===================================*/
	//! \brief 個別の初期化
	/*===================================*/
	void DiscreteInit( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CBreakWall( void );


	/*================================*/
	//! \brief 更新
	/*================================*/
	void Exec( void );


	/*=============================================*/
	//! \brief 描画
	//! \param[in] &_d3d	DirectXのデバイス
	/*=============================================*/
	void Draw( CDirect3D &_d3d );


	static const int FragmentType = 3;				//!< 破片の種類


private:

	
	static const int SetFragmentNum = 100;			//!< いくつ破片をばら撒くか

	std::string m_BreakWallFilePass;				//!< 壊れる前の状態のメッシュのパス
	std::string m_BreakedWalldFilePass;				//!< 壊れた状態のメッシュのパス

	CXFile *m_lpBreakWallXFile;						//!< 壊れる前の状態のメッシュ
	CXFile *m_lpBreakedWallXFile;					//!< 壊れた状態のメッシュ

	NxActor *m_BreakWallActor;						//!< 壊される前のアクター
	NxActor *m_BreakedWallActor;					//!< 壊された後のアクター

	CFragment *m_Fragment[SetFragmentNum];			//!< 破壊されたときに飛び散る破片

	bool m_BreakedFlag;								//!< 床が壊れているかのフラグ

	NxBox m_worldBox;								//!< オーバーラップで使用する判定用の情報を格納

};