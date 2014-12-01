

class CEventManager
{

	/*========================================================================*/
	//!
	//! \file
	//! \class CEventManager
	//!	\brief <b>�C�x���g�V�[���̊Ǘ�</b>
	//!
	//! \author Y.Yamamuro \n
	//! \date  2014/11/17
	//! \since 2014/11/17
	//!
	/*========================================================================*/

public:

	/*==============================================*/
	//! \brief �C���X�^���X��Ԃ�
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
	//! \brief �f�X�g���N�^
	/*=====================================*/
	~CEventManager();


private:

	/*===============================================*/
	//! \brief �f�t�H���g�R���X�g���N�^
	/*===============================================*/
	CEventManager( void );

	/*===============================================*/
	//! \brief �R�s�[�R���X�g���N�^(�g�p�֎~)
	/*===============================================*/
    CEventManager( const CEventManager& single );

	/*===============================================*/
    //! \brief ������Z�q(�g�p�֎~)
	/*===============================================*/
    const CEventManager& operator = ( const CEventManager& single );


	//! \brief �C���X�^���X�̃|�C���^
	static CEventManager *m_pInstance;	//!< CEventManager�̃|�C���^

	int m_NowEvent;

};

/*=================================================*/
//! \def EVENT_MANAGER
//! \brief �R�[�����Ɏg�p����}�N��
/*=================================================*/
#define EVENT_MANAGER CEventManager::GetInstance()
