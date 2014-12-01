#pragma once

class CVector
{

	/*=========================================================*/
	//!
	//! \file
	//! \class CVector
	//! \brief <b>�x�N�g���̂悭�g���������܂Ƃ߂�����</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/10/10
	//! \since 2014/10/10
	//!
	/*=========================================================*/

public:

	/*===================================================*/
	//! \brief �x�N�g������p�x�𒊏o
	//! \param[out]	*_Rot	���o�����p�x���i�[
	//! \param[in]	*_Vec	�p�x�x�N�g��
	/*===================================================*/
	static void VecToRot( D3DXVECTOR3 *_Rot ,  D3DXVECTOR3 *_Vec )
	{

		D3DXVECTOR3 Origin = *_Vec;
		D3DXVECTOR3 VecX( 1.0f , 0.0f , 0.0f );
		D3DXVECTOR3 VecY( 0.0f , 1.0f , 0.0f );
		D3DXVECTOR3 VecZ( 0.0f , 0.0f , 1.0f );
		D3DXVECTOR3 TmpVec( 0.0f , 0.0f , 0.0f );

		// �����Ƃ��ăx�N�g���𐳋K��
		D3DXVec3Normalize( &Origin , &Origin );

		// x�̊p�x�𒊏o
		TmpVec = Origin;
		TmpVec.z = 0;
		D3DXVec3Normalize( &TmpVec , &TmpVec );
		_Rot->x = D3DXVec3Dot( &VecX , &TmpVec );
		_Rot->x = D3DXToDegree( acos( _Rot->x ) );
		if( TmpVec.y < 0 ){ _Rot->x = -_Rot->x; }

		// y�̊p�x�𒊏o
		TmpVec = Origin;
		TmpVec.x = 0;
		D3DXVec3Normalize( &TmpVec , &TmpVec );
		_Rot->y = D3DXVec3Dot( &VecY , &TmpVec );
		_Rot->y = D3DXToDegree( acos( _Rot->y ) );
		if( TmpVec.z < 0 ){ _Rot->y = -_Rot->y; }

		// z�̊p�x�𒊏o
		TmpVec = Origin;
		TmpVec.y = 0;
		D3DXVec3Normalize( &TmpVec , &TmpVec );
		_Rot->z = D3DXVec3Dot( &VecZ , &TmpVec );
		_Rot->z = D3DXToDegree( acos( _Rot->z ) );
		if( TmpVec.x < 0 ){ _Rot->z = -_Rot->z; }

		TmpVec = *_Rot;
		_Rot->x = TmpVec.y;
		_Rot->y = TmpVec.z;
		_Rot->z = TmpVec.x;

	}

};