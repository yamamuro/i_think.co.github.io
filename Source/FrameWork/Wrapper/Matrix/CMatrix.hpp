#pragma once

class CMatrix : public D3DXMATRIX
{		
	
	/*=====================================================*/
	//!
	//! \file
	//! \class CMatric
	//! \brief <b>D3DXMATRIX�̃��b�p�[�N���X</b>
	//!
	//! \author �z�z�f�[�^+Y.Yamamuro \n
	//! \date  2014/09/04
	//! \since 2014/10/08
	//!
	/*=====================================================*/

public:		 

	/*===========================================*/
	//! \brief �I�y���[�^�̃I�[�o�[���C�h
	//! \brief �������
	/*===========================================*/
	D3DXMATRIX & operator=( D3DXMATRIX & _cm )	
	{	
		if( this == &_cm ){ return *this; }
		// �h���N���X��operator=�ł͊��N���X��=���Ăяo��
		D3DXMATRIX::operator=( _cm );
		return *this;
	}	

	/*========================================*/
	//! \brief �R�s�[�R���X�g���N�^
	/*========================================*/
	CMatrix( D3DXMATRIX &_m ) : D3DXMATRIX( _m ){}

	/*==================================*/
	//! \brief �P�ʍs��ŏ�����
	/*==================================*/
	CMatrix( void ){ D3DXMatrixIdentity( this ); }
		

	/*============================================*/
	//! \brief �ړ��s��̍쐬
	//! \param[in]	_x		�ړ�������x���W
	//! \param[in]	_y		�ړ�������y���W
	//! \param[in]	_z		�ړ�������z���W
	/*============================================*/
	void SetTrans( float _x , float _y , float _z )
	{
		D3DXMatrixTranslation( this , _x , _y , _z );
	}

	/*============================================*/
	//! \brief �ړ��s��̍쐬
	//! \param[in]	_Pos	�ړ���������W
	/*============================================*/
	void SetTrans( D3DXVECTOR3 _Pos )
	{
		D3DXMatrixTranslation( this , _Pos.x , _Pos.y , _Pos.z );
	}


	/*================================================*/
	//! \brief ��]�s��̍쐬
	//! param[in]	_x		x���̉�]��
	//! param[in]	_y		y���̉�]��
	//! param[in]	_z		z���̉�]��
	/*================================================*/
	void SetRotate( float _x , float _y , float _z )
	{
		D3DXMatrixRotationYawPitchRoll( this , D3DXToRadian( _y ) ,
											   D3DXToRadian( _x ) ,
											   D3DXToRadian( _z ) );
	}

	/*=====================================================*/
	//! \brief ��]�s��̍쐬
	//! param[in]	_RotDegree		�e���̉�]��
	/*=====================================================*/
	void SetRotate( D3DXVECTOR3 RotDegree )
	{
		D3DXMatrixRotationYawPitchRoll( this , D3DXToRadian( RotDegree.y ) ,
											   D3DXToRadian( RotDegree.x ) ,
											   D3DXToRadian( RotDegree.z ) );
	}


	/*===============================================*/
	//! \brief �g��k���s��̍쐬
	//! param[in]	_x		x���̊g�k��
	//! param[in]	_y		y���̊g�k��
	//! param[in]	_z		z���̊g�k��
	/*===============================================*/
	void SetScale( float _x , float _y , float _z )
	{
		D3DXMatrixScaling( this , _x , _y , _z );
	}

	/*===============================================*/
	//! \brief �g��k���s��̍쐬
	//! param[in]	_Scale		x,y,z���̊g�k��
	/*===============================================*/
	void SetScale( float _Scale )
	{
		D3DXMatrixScaling( this , _Scale , _Scale , _Scale );
	}

	/*=====================================================*/
	//! \brief �g��k���s��̍쐬
	//! param[in]	_Scale		�e���̊g�k��
	/*=====================================================*/
	void SetScale( D3DXVECTOR3 _Scale )
	{
		D3DXMatrixScaling( this , _Scale.x , _Scale.y , _Scale.z );
	}


	/*====================================*/
	//! \brief �t�s���Ԃ�
	/*====================================*/
	D3DXMATRIX GetInverse( void ){	
		D3DXMATRIX mOut;
		D3DXMatrixInverse( &mOut , NULL , this );
		return mOut;
	}	


	/*====================================*/
	//! \brief ���W�̎擾(X���W)
	/*====================================*/
	inline float GetX( void ){ return _41; }

	/*====================================*/
	//! \brief ���W�̎擾(Y���W)
	/*====================================*/
	inline float GetY( void ){ return _42; }

	/*====================================*/
	//! \brief ���W�̎擾(Z���W)
	/*====================================*/
	inline float GetZ( void ){ return _43; }

	/*====================================*/
	//! \brief ���W�̎擾(D3DXVECTOR3)
	/*====================================*/
	inline D3DXVECTOR3 GetPos( void ){ return D3DXVECTOR3( _41 , _42 , _43 ); }

};


