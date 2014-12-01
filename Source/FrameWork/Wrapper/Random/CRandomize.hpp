#pragma once

#include <random>
#include <cstdint>

class CRandomize
{

	/*==========================================================*/
	//!
	//! \file
	//! \class CRandomize
	//! \brief <b>std::random_device�̃��b�p�[�N���X</b>
	//!
	//! \author	Y.Yamamuro \n
	//! \date  2014/09/04
	//! \since 2014/09/04
	//!
	/*==========================================================*/
 
public:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CRandomize( void )
	{
		Initialize();
	}


	/*====================================================*/
	//! \brief �w�肵���͈͂̐��l��Ԃ�(����)
	//! \param[in]	_iMin	�����̍ŏ��l
	//! \param[in]	_iMax	�����̍ő�l
	//! \return ������������
	/*====================================================*/
	inline int GetRandomize( const int _iMin , const int _iMax )
	{
		std::uniform_int_distribution<int> Dist( _iMin , _iMax );
		return Dist( m_mt );
	}
 

	/*====================================================*/
	//! \brief �w�肵���͈͂̐��l��Ԃ�(�����_�t��)
	//! \param[in]	_dMin	�����̍ŏ��l
	//! \param[in]	_dMax	�����̍ő�l
	//! \return ������������
	/*====================================================*/
	inline double GetRandomize( const double _dMin , const double _dMax )
	{
		std::uniform_real_distribution<double> Dist( _dMin , _dMax );
		return Dist( m_mt );
	}


private:


	/*===================================*/
	//! \brief �����̏�����
	/*===================================*/
	inline void Initialize( void )
	{
		std::random_device RndDev;
		v.push_back( 10 );
		std::generate( v.begin(), v.end(), std::ref( RndDev ) ) ;
		m_mt = std::mt19937( std::seed_seq( v.begin(), v.end() ) ) ;
	}

	std::vector< std::uint_least32_t > v;		//!< �����������p�̃x�N�^
	std::mt19937 m_mt;							//!< ���������G���W��

};