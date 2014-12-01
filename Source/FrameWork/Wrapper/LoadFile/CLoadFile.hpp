#pragma once

class CLoadFile
{

	/*================================================================*/
	//!
	//! \file
	//! \class CLoadFile
	//! \brief <b>�O���t�@�C������̓ǂݍ��݂��Ǘ�</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/08
	//! \since 2014/10/11
	//!
	/*=================================================================*/

public:

	/*=======================================*/
	//! \enum ERROR_POINT
	//! \brief �G���[�����������|�C���g
	/*=======================================*/
	enum
	{
		OPEN_ERROR,		//!< �t�@�C���I�[�v���G���[
		READ_ERROR		//!< �t�@�C���̃f�[�^�ɕs����������ꍇ
	};


	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CLoadFile( void );


	/*=====================================*/
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CLoadFile( void );



	/*========================================================================================*/
	//! \brief �O���t�@�C����ǂݍ��݃f�[�^��Ԃ�
	//! \tparam[out]	&_Data				�ǂݍ���csv�f�[�^���i�[
	//! \param[in]		_ElementsNum		�����̗v�f��1�̃f�[�^�Ǝ��ʂ��邩
	//! \param[in]		_FilePass			�ǂݍ��ރt�@�C���̃p�X
	//! \param[in]		_Separate			��؂蕶��
	//! \param[in]		_IgnoreString		�؂藎�Ƃ�����
	//! \return true  = ����
	//! \return false = ���s
	/*========================================================================================*/
	template <typename T>
	static bool LoadFile( std::vector< T > &_Data , const int _ElementsNum , const std::string _FilePass , const std::string _Separate , const std::string _IgnoreString )
	{


		// �O���t�@�C���̈�s���i�[
		std::string Line;

		// �J���}��؂�ɂȂ��ċA���Ă����f�[�^���i�[����
		T TmpData;

		// �t�@�C���I�[�v��
		std::ifstream ifs( _FilePass );

		// �ŏ��̈�s�ڂɂ͎��f�[�^�����Ă��Ȃ��̂ŃG���[�`�F�b�N�ɗ��p����
		getline( ifs , Line );


		// �t�@�C���I�[�v�����̃G���[�`�F�b�N
		if( ifs.fail() )
		{ 
			// �t�@�C���ǂݍ��ݎ��̃G���[���b�Z�[�W
			FileErrorMessage( _FilePass , OPEN_ERROR );
			PostQuitMessage( 0 );
			return false;
		}

		while( 1 )
		{
		
			// �O���t�@�C�������s���ǂݍ���
			getline( ifs , Line );

			// �G���[�A�I�[�܂œǂݎ�����ꍇ�����𔲂���
			if( ifs.fail() )
			{

				// ��肪�����������I�[�܂œǂݎ���Ă��Ȃ��Ƃ�(�f�[�^�ɕs����������ꍇ)
				if( ifs.eof() == false ) 
				{
					// �t�@�C���ǂݍ��ݎ��̃G���[���b�Z�[�W
					FileErrorMessage( _FilePass , READ_ERROR );
					// �����I�ȃt�@�C���N���[�Y����
					ifs.close();
					PostQuitMessage( 0 );
					return false;
				}
				else
				{
					break;	// �I�[�܂œǂݎ�����̂ŏ����𔲂���
				}

			}

			if( Line.length() != 0 )
			{
				if( Line.substr( 0 , 2 ) != "//" )		// �R�����g�A�E�g��ǂݔ�΂�
				{

					// �f�[�^����؂�
					Split( TmpData , _ElementsNum , &Line , _Separate );

					// �f�[�^�̑̍ق𐮂���
					Arrange( TmpData , _ElementsNum , _IgnoreString );

					_Data.push_back( TmpData );

				}
			}

		}

		// �����I�ȃt�@�C���N���[�Y����
		ifs.close();

		return true;

	}


	/*============================================================*/
	//! \brief �t�@�C���ǂݍ��ݎ��̃G���[���b�Z�[�W
	//! \param[in]	_FilePass	�t�@�C���̃p�X
	//! \param[in]	_ErrorPoint	�G���[�����������|�C���g
	/*============================================================*/
	static void FileErrorMessage( const std::string _FilePass , const int _ErrorPoint );


	/*===============================================================================*/
	//! \brief ��������w�肳�ꂽ��؂蕶���ŋ�؂�
	//! \param[out]		&_Data				��؂����f�[�^�̊i�[�ꏊ
	//! \param[in]		_ElementsNum		�����̗v�f��1�̃f�[�^�Ǝ��ʂ��邩
	//! \param[in]		*_Line				���f�[�^
	//! \param[in]		_Separate			��؂蕶��
	/*===============================================================================*/
	template <typename T>
	static void Split( T &_Data , const int _ElementsNum , std::string *_Line , const std::string _Separate )
	{

		int SeekPos;

		for( int i=0 ; i<_ElementsNum ; i++ )
		{
			
			// ��؂蕶�������������ꍇfor���[�v�𔲂���
			if( ( SeekPos = _Line->find_first_of( _Separate ) ) == std::string::npos )
			{
				_Data.Str[i] = *_Line;
				break;
			}

			// ��؂蕶���܂ł�ۑ�����
			if( SeekPos ){ _Data.Str[i] = _Line->substr( 0 , SeekPos ); }

			// ��؂蕶����ǂݔ�΂�
			*_Line = _Line->substr( SeekPos + sizeof( char ) );

		}


	}


	/*===============================================================================*/
	//! \brief �s�K�v�ȕ�����؂藎�Ƃ�
	//! \param[out]		&_Data				��؂����f�[�^�̊i�[�ꏊ
	//! \param[in]		_ElementsNum		�����̗v�f��1�̃f�[�^�Ǝ��ʂ��邩
	//! \param[in]		_IgnoreString		�؂藎�Ƃ�����
	/*===============================================================================*/
	template <typename T>
	static void Arrange( T &_Data , const int _ElementsNum , const std::string _IgnoreString )
	{

		int SeekPos;

		for( int i=0 ; i<_ElementsNum ; i++ )
		{
			while( ( SeekPos = _Data.Str[i].find_first_of( _IgnoreString ) ) != std::string::npos )
			{
				_Data.Str[i].erase( SeekPos , sizeof( char ) );
			}
		}

	}


};

