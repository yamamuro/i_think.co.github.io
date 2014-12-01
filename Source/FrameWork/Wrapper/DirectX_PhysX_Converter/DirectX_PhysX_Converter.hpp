#pragma once


class CDPConverter
{

	/*=====================================================*/
	//!
	//! \file
	//! \class CDPConverter
	//! \brief <b>DirectX,PhysX間のコンバータ</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/13
	//! \since 2014/11/13
	//!
	/*=====================================================*/

public:

	/*===========================================*/
	//! \brief NxVec3をD3DXVECTOR3に変換
	/*===========================================*/
	static inline D3DXVECTOR3 toDirectX( NxVec3 _Vec ){ return D3DXVECTOR3( _Vec.x , _Vec.y , _Vec.z ); }


	/*===========================================*/
	//! \brief D3DXVECTOR3をNxVec3に変換
	/*===========================================*/
	static inline NxVec3 toPhysX( D3DXVECTOR3 _Vec ){ return NxVec3( _Vec.x , _Vec.y , _Vec.z ); }


	
	/*=============================================================*/
	//! \brief PhysXの行列をDirecXの行列に変換
	//! \param[out]	*_MatWorld		左手系の行列
	//! \param[in]	*_Actor			変換元になるアクター
	/*=============================================================*/
	static void SetMatrix( D3DXMATRIXA16 *_MatWorld , const NxActor *_Actor )
	{

		float Mat[4*4];

		_Actor->getGlobalPose().getColumnMajor44( Mat );

		_MatWorld->_11 = Mat[0];
		_MatWorld->_12 = Mat[1];
		_MatWorld->_13 = Mat[2];
		_MatWorld->_14 = Mat[3];

		_MatWorld->_21 = Mat[4];
		_MatWorld->_22 = Mat[5];
		_MatWorld->_23 = Mat[6];
		_MatWorld->_24 = Mat[7];

		_MatWorld->_31 = Mat[8];
		_MatWorld->_32 = Mat[9];
		_MatWorld->_33 = Mat[10];
		_MatWorld->_34 = Mat[11];

		_MatWorld->_41 = Mat[12];
		_MatWorld->_42 = Mat[13];
		_MatWorld->_43 = Mat[14];
		_MatWorld->_44 = Mat[15];

	}

	/*======================================================*/
	//! \brief DirectXの角度情報を3*3の行列に変換
	//! \param[out]	_Rot	3*3の角度情報を格納
	//! \param[in]	*_mRot	左手系の行列
	/*======================================================*/
	static void SetMatrix( NxF64 _Rot[3][3] , const D3DXMATRIX *_mRot )
	{

		_Rot[0][0] = _mRot->_11;
		_Rot[0][1] = _mRot->_12;
		_Rot[0][2] = _mRot->_13;

		_Rot[1][0] = _mRot->_21;
		_Rot[1][1] = _mRot->_22;
		_Rot[1][2] = _mRot->_23;

		_Rot[2][0] = _mRot->_31;
		_Rot[2][1] = _mRot->_32;
		_Rot[2][2] = _mRot->_33;

	}

	/*==================================================================================*/
	//! \brief DirectX(D3DXVECTOR3)の角度をPhysX(NxMat33)の角度に変換
	//! \param[out]		*_Mat		右手系の3*3の行列
	//! \param[in]		*_vRot		角度ベクトル
	/*==================================================================================*/
	static void SetRot( NxMat33 *_Mat , const D3DXVECTOR3 *_vRot )
	{

		CMatrix mMat;
		mMat.SetRotate( *_vRot );

		NxF64 Rot[3][3];
		SetMatrix( Rot , &mMat );

		_Mat->setColumnMajor( Rot );

	}


};