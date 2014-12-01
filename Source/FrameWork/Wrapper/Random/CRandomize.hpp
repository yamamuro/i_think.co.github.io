#pragma once

#include <random>
#include <cstdint>

class CRandomize
{

	/*==========================================================*/
	//!
	//! \file
	//! \class CRandomize
	//! \brief <b>std::random_deviceのラッパークラス</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/04
	//! \since 2014/09/04
	//!
	/*==========================================================*/
 
public:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CRandomize( void )
	{
		Initialize();
	}


	/*====================================================*/
	//! \brief 指定した範囲の数値を返す(整数)
	//! \param[in]	_iMin	乱数の最小値
	//! \param[in]	_iMax	乱数の最大値
	//! \return 生成した乱数
	/*====================================================*/
	inline int GetRandomize( const int _iMin , const int _iMax )
	{
		std::uniform_int_distribution<int> Dist( _iMin , _iMax );
		return Dist( m_mt );
	}
 

	/*====================================================*/
	//! \brief 指定した範囲の数値を返す(小数点付き)
	//! \param[in]	_dMin	乱数の最小値
	//! \param[in]	_dMax	乱数の最大値
	//! \return 生成した乱数
	/*====================================================*/
	inline double GetRandomize( const double _dMin , const double _dMax )
	{
		std::uniform_real_distribution<double> Dist( _dMin , _dMax );
		return Dist( m_mt );
	}


private:


	/*===================================*/
	//! \brief 乱数の初期化
	/*===================================*/
	inline void Initialize( void )
	{
		std::random_device RndDev;
		v.push_back( 10 );
		std::generate( v.begin(), v.end(), std::ref( RndDev ) ) ;
		m_mt = std::mt19937( std::seed_seq( v.begin(), v.end() ) ) ;
	}

	std::vector< std::uint_least32_t > v;		//!< 乱数初期化用のベクタ
	std::mt19937 m_mt;							//!< 乱数生成エンジン

};