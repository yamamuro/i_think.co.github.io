#pragma once

class CLoadFile
{

	/*================================================================*/
	//!
	//! \file
	//! \class CLoadFile
	//! \brief <b>外部ファイルからの読み込みを管理</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/10/08
	//! \since 2014/10/11
	//!
	/*=================================================================*/

public:

	/*=======================================*/
	//! \enum ERROR_POINT
	//! \brief エラーが発生したポイント
	/*=======================================*/
	enum
	{
		OPEN_ERROR,		//!< ファイルオープンエラー
		READ_ERROR		//!< ファイルのデータに不具合があった場合
	};


	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CLoadFile( void );


	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CLoadFile( void );



	/*========================================================================================*/
	//! \brief 外部ファイルを読み込みデータを返す
	//! \tparam[out]	&_Data				読み込んだcsvデータを格納
	//! \param[in]		_ElementsNum		いくつの要素で1つのデータと識別するか
	//! \param[in]		_FilePass			読み込むファイルのパス
	//! \param[in]		_Separate			区切り文字
	//! \param[in]		_IgnoreString		切り落とす文字
	//! \return true  = 成功
	//! \return false = 失敗
	/*========================================================================================*/
	template <typename T>
	static bool LoadFile( std::vector< T > &_Data , const int _ElementsNum , const std::string _FilePass , const std::string _Separate , const std::string _IgnoreString )
	{


		// 外部ファイルの一行を格納
		std::string Line;

		// カンマ区切りになって帰ってきたデータを格納する
		T TmpData;

		// ファイルオープン
		std::ifstream ifs( _FilePass );

		// 最初の一行目には実データを入れていないのでエラーチェックに流用する
		getline( ifs , Line );


		// ファイルオープン時のエラーチェック
		if( ifs.fail() )
		{ 
			// ファイル読み込み時のエラーメッセージ
			FileErrorMessage( _FilePass , OPEN_ERROR );
			PostQuitMessage( 0 );
			return false;
		}

		while( 1 )
		{
		
			// 外部ファイルから一行分読み込む
			getline( ifs , Line );

			// エラー、終端まで読み取った場合処理を抜ける
			if( ifs.fail() )
			{

				// 問題が発生したが終端まで読み取っていないとき(データに不具合があった場合)
				if( ifs.eof() == false ) 
				{
					// ファイル読み込み時のエラーメッセージ
					FileErrorMessage( _FilePass , READ_ERROR );
					// 明示的なファイルクローズ処理
					ifs.close();
					PostQuitMessage( 0 );
					return false;
				}
				else
				{
					break;	// 終端まで読み取ったので処理を抜ける
				}

			}

			if( Line.length() != 0 )
			{
				if( Line.substr( 0 , 2 ) != "//" )		// コメントアウトを読み飛ばす
				{

					// データを区切る
					Split( TmpData , _ElementsNum , &Line , _Separate );

					// データの体裁を整える
					Arrange( TmpData , _ElementsNum , _IgnoreString );

					_Data.push_back( TmpData );

				}
			}

		}

		// 明示的なファイルクローズ処理
		ifs.close();

		return true;

	}


	/*============================================================*/
	//! \brief ファイル読み込み時のエラーメッセージ
	//! \param[in]	_FilePass	ファイルのパス
	//! \param[in]	_ErrorPoint	エラーが発生したポイント
	/*============================================================*/
	static void FileErrorMessage( const std::string _FilePass , const int _ErrorPoint );


	/*===============================================================================*/
	//! \brief 文字列を指定された区切り文字で区切る
	//! \param[out]		&_Data				区切ったデータの格納場所
	//! \param[in]		_ElementsNum		いくつの要素で1つのデータと識別するか
	//! \param[in]		*_Line				元データ
	//! \param[in]		_Separate			区切り文字
	/*===============================================================================*/
	template <typename T>
	static void Split( T &_Data , const int _ElementsNum , std::string *_Line , const std::string _Separate )
	{

		int SeekPos;

		for( int i=0 ; i<_ElementsNum ; i++ )
		{
			
			// 区切り文字が見つかった場合forループを抜ける
			if( ( SeekPos = _Line->find_first_of( _Separate ) ) == std::string::npos )
			{
				_Data.Str[i] = *_Line;
				break;
			}

			// 区切り文字までを保存する
			if( SeekPos ){ _Data.Str[i] = _Line->substr( 0 , SeekPos ); }

			// 区切り文字を読み飛ばす
			*_Line = _Line->substr( SeekPos + sizeof( char ) );

		}


	}


	/*===============================================================================*/
	//! \brief 不必要な文字を切り落とす
	//! \param[out]		&_Data				区切ったデータの格納場所
	//! \param[in]		_ElementsNum		いくつの要素で1つのデータと識別するか
	//! \param[in]		_IgnoreString		切り落とす文字
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

