#pragma once


class CDPConverter
{

	/*=====================================================*/
	//!
	//! \file
	//! \class CDPConverter
	//! \brief <b>DirectX,PhysX�Ԃ̃R���o�[�^</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/13
	//! \since 2014/11/13
	//!
	/*=====================================================*/

public:

	/*===========================================*/
	//! \brief NxVec3��D3DXVECTOR3�ɕϊ�
	/*===========================================*/
	static inline D3DXVECTOR3 toDirectX( NxVec3 _Vec ){ return D3DXVECTOR3( _Vec.x , _Vec.y , _Vec.z ); }


	/*===========================================*/
	//! \brief D3DXVECTOR3��NxVec3�ɕϊ�
	/*===========================================*/
	static inline NxVec3 toPhysX( D3DXVECTOR3 _Vec ){ return NxVec3( _Vec.x , _Vec.y , _Vec.z ); }


	
	/*=============================================================*/
	//! \brief PhysX�̍s���DirecX�̍s��ɕϊ�
	//! \param[out]	*_MatWorld		����n�̍s��
	//! \param[in]	*_Actor			�ϊ����ɂȂ�A�N�^�[
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
	//! \brief DirectX�̊p�x����3*3�̍s��ɕϊ�
	//! \param[out]	_Rot	3*3�̊p�x�����i�[
	//! \param[in]	*_mRot	����n�̍s��
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
	//! \brief DirectX(D3DXVECTOR3)�̊p�x��PhysX(NxMat33)�̊p�x�ɕϊ�
	//! \param[out]		*_Mat		�E��n��3*3�̍s��
	//! \param[in]		*_vRot		�p�x�x�N�g��
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