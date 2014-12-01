#pragma once

class CMatrix : public D3DXMATRIX
{		
	
	/*=====================================================*/
	//!
	//! \file
	//! \class CMatric
	//! \brief <b>D3DXMATRIXのラッパークラス</b>
	//!
	//! \author 配布データ+Y.Yamamuro \n
	//! \date  2014/09/04
	//! \since 2014/10/08
	//!
	/*=====================================================*/

public:		 

	/*===========================================*/
	//! \brief オペレータのオーバーライド
	//! \brief 代入処理
	/*===========================================*/
	D3DXMATRIX & operator=( D3DXMATRIX & _cm )	
	{	
		if( this == &_cm ){ return *this; }
		// 派生クラスのoperator=では基底クラスの=も呼び出す
		D3DXMATRIX::operator=( _cm );
		return *this;
	}	

	/*========================================*/
	//! \brief コピーコンストラクタ
	/*========================================*/
	CMatrix( D3DXMATRIX &_m ) : D3DXMATRIX( _m ){}

	/*==================================*/
	//! \brief 単位行列で初期化
	/*==================================*/
	CMatrix( void ){ D3DXMatrixIdentity( this ); }
		

	/*============================================*/
	//! \brief 移動行列の作成
	//! \param[in]	_x		移動させるx座標
	//! \param[in]	_y		移動させるy座標
	//! \param[in]	_z		移動させるz座標
	/*============================================*/
	void SetTrans( float _x , float _y , float _z )
	{
		D3DXMatrixTranslation( this , _x , _y , _z );
	}

	/*============================================*/
	//! \brief 移動行列の作成
	//! \param[in]	_Pos	移動させる座標
	/*============================================*/
	void SetTrans( D3DXVECTOR3 _Pos )
	{
		D3DXMatrixTranslation( this , _Pos.x , _Pos.y , _Pos.z );
	}


	/*================================================*/
	//! \brief 回転行列の作成
	//! param[in]	_x		x軸の回転量
	//! param[in]	_y		y軸の回転量
	//! param[in]	_z		z軸の回転量
	/*================================================*/
	void SetRotate( float _x , float _y , float _z )
	{
		D3DXMatrixRotationYawPitchRoll( this , D3DXToRadian( _y ) ,
											   D3DXToRadian( _x ) ,
											   D3DXToRadian( _z ) );
	}

	/*=====================================================*/
	//! \brief 回転行列の作成
	//! param[in]	_RotDegree		各軸の回転量
	/*=====================================================*/
	void SetRotate( D3DXVECTOR3 RotDegree )
	{
		D3DXMatrixRotationYawPitchRoll( this , D3DXToRadian( RotDegree.y ) ,
											   D3DXToRadian( RotDegree.x ) ,
											   D3DXToRadian( RotDegree.z ) );
	}


	/*===============================================*/
	//! \brief 拡大縮小行列の作成
	//! param[in]	_x		x軸の拡縮量
	//! param[in]	_y		y軸の拡縮量
	//! param[in]	_z		z軸の拡縮量
	/*===============================================*/
	void SetScale( float _x , float _y , float _z )
	{
		D3DXMatrixScaling( this , _x , _y , _z );
	}

	/*===============================================*/
	//! \brief 拡大縮小行列の作成
	//! param[in]	_Scale		x,y,z軸の拡縮量
	/*===============================================*/
	void SetScale( float _Scale )
	{
		D3DXMatrixScaling( this , _Scale , _Scale , _Scale );
	}

	/*=====================================================*/
	//! \brief 拡大縮小行列の作成
	//! param[in]	_Scale		各軸の拡縮量
	/*=====================================================*/
	void SetScale( D3DXVECTOR3 _Scale )
	{
		D3DXMatrixScaling( this , _Scale.x , _Scale.y , _Scale.z );
	}


	/*====================================*/
	//! \brief 逆行列を返す
	/*====================================*/
	D3DXMATRIX GetInverse( void ){	
		D3DXMATRIX mOut;
		D3DXMatrixInverse( &mOut , NULL , this );
		return mOut;
	}	


	/*====================================*/
	//! \brief 座標の取得(X座標)
	/*====================================*/
	inline float GetX( void ){ return _41; }

	/*====================================*/
	//! \brief 座標の取得(Y座標)
	/*====================================*/
	inline float GetY( void ){ return _42; }

	/*====================================*/
	//! \brief 座標の取得(Z座標)
	/*====================================*/
	inline float GetZ( void ){ return _43; }

	/*====================================*/
	//! \brief 座標の取得(D3DXVECTOR3)
	/*====================================*/
	inline D3DXVECTOR3 GetPos( void ){ return D3DXVECTOR3( _41 , _42 , _43 ); }

};


