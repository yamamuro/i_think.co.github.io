

class CEventManager
{

	/*========================================================================*/
	//!
	//! \file
	//! \class CEventManager
	//!	\brief <b>イベントシーンの管理</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/17
	//! \since 2014/11/17
	//!
	/*========================================================================*/

public:

	/*==============================================*/
	//! \brief インスタンスを返す
	/*==============================================*/
	static CEventManager* GetInstance( void )
	{
		if( m_pInstance == nullptr )
		{
			m_pInstance = new CEventManager();
		}
		return m_pInstance;
	}

	/*=====================================*/
	//! \brief デストラクタ
	/*=====================================*/
	~CEventManager();


private:

	/*===============================================*/
	//! \brief デフォルトコンストラクタ
	/*===============================================*/
	CEventManager( void );

	/*===============================================*/
	//! \brief コピーコンストラクタ(使用禁止)
	/*===============================================*/
    CEventManager( const CEventManager& single );

	/*===============================================*/
    //! \brief 代入演算子(使用禁止)
	/*===============================================*/
    const CEventManager& operator = ( const CEventManager& single );


	//! \brief インスタンスのポインタ
	static CEventManager *m_pInstance;	//!< CEventManagerのポインタ

	int m_NowEvent;

};

/*=================================================*/
//! \def EVENT_MANAGER
//! \brief コール時に使用するマクロ
/*=================================================*/
#define EVENT_MANAGER CEventManager::GetInstance()
