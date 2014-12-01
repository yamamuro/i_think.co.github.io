#include "stdafx.hpp"

#include "../Application/CApplication.hpp"


/*=========================================================*/
//!
//! \file
//! \brief <b>�v���O�����̃G���g���|�C���g</b>
//! \brief �E�B���h�E�̍쐬���s��
//!
//! \author	Y.Yamamuro \n
//! \date  2014/09/03
//! \since 2014/09/08
//!
/*=========================================================*/


/*===================================================================================*/
//! \brief �E�B���h�E���쐬����
//! \param[out]	*_FullScreenFlag	�t���X�N���[���ŋN������̂��̃t���O
//! \param[in]	_nWinMode			WinMain�̈������g�p	�E�B���h�E�̕\�����@
//! \return true  = ����
//! \return false = ���s
/*===================================================================================*/
bool MakeWindow( bool *_FullScreenFlag , const int _nWinMode );


/*=====================================================================*/
//! \brief �E�B���h�E�̃N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
//! \param[in]	_hWnd		�E�B���h�E�n���h��
//! \param[in]	_Width		�E�B���h�E�̕�
//! \param[in]	_Height		�E�B���h�E�̍���
/*=====================================================================*/
void SetClientSize( const HWND _hWnd , const int _Width , const int _Height );


/*================================================================*/
//! \brief �v���O�����̃G���g���|�C���g
//! \param[in]	_hInstance	�C���X�^���X�n���h��
//! \param[in]	_hPrevInst	�g�p���Ȃ��@�l��NULL
//! \param[in]	_lpszArgs	�g�p���Ȃ��@�R�}���h���C������
//! \param[in]	_nWinMode	�E�B���h�E�̕\�����[�h
/*================================================================*/
int WINAPI WinMain( HINSTANCE _hInstance , HINSTANCE _hPrevInst , LPSTR _lpszArgs , int _nWinMode )
{

	bool FullScreenFlag = false;
	CDebug	*m_pDebug = nullptr;

#ifdef _DEBUG
	// ���������[�N�̌��o
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// �f�o�b�O�Ǘ��p�̃N���X
	m_pDebug = DEBUG;	
#endif


	// �C���X�^���X���R�s�[���Ă���
	CAPP->hInst = _hInstance;
	CAPP->SetCDebug( m_pDebug );

	DEBUG->PrintConsole( "===" + GAME_TITLE + "�̏��������n�߂܂�===\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( "\n\n" );

	// �E�B���h�E���쐬����
	if( MakeWindow( &FullScreenFlag , _nWinMode ) == false )
	{
		std::string sErrorMessage = "�E�B���h�E�̍쐬�Ɏ��s���܂���\n�{��i��XP�ȍ~��WindowsOS�ł̓����O��Ƃ��Ă��܂��B\n������L�𖞂����Ă���A�G���[�����������ꍇ�͂��A�����肢���܂�";
		ErrorMessage( sErrorMessage );

		SafeExit();
		SafeDelete( VARIABLE_DATA( m_pDebug ) );
		return 1;
	}
	DEBUG->PrintConsole( "�E�B���h�E�̍쐬�ɐ������܂���...\n" , CDebug::ConPreside );
	
	// �A�v���P�[�V�����̏�����
	if( CAPP->InitializeApplication( FullScreenFlag ) == false )
	{
		std::string sErrorMessage = "�Q�[���̏��������ɃG���[���������܂���\n���S�ɃQ�[�����I�����܂�";
		ErrorMessage( sErrorMessage );

		SafeExit();
		SafeDelete( VARIABLE_DATA( m_pDebug ) );
		return 1;
	}

	DEBUG->PrintConsole( "================================================================\n" , CDebug::ConFence );
	DEBUG->PrintConsole( GAME_TITLE + "�̏������ɐ������܂���...\n" , CDebug::ConPreside );
	DEBUG->PrintConsole( "\n\n" );


	// �Q�[�����[�v�̊J�n
	CAPP->GameLoop();


	// ���
	CAPP->Delete();


#ifdef _DEBUG
	// CDebug�̉���͈�ԍŌ�ɍs��
	SafeDelete( VARIABLE_DATA( m_pDebug ) );
#endif


	return 0; 

}


/*=========================================================================*/
//! \brief �E�B���h�E�v���V�[�W��
//! \param[in]	_hWnd		�E�B���h�E�n���h��
//! \param[in]	_Message	���b�Z�[�W�̎��ʎq
//! \param[in]	_wParam		���b�Z�[�W�̍ŏ��̃p�����[�^
//! \param[in]	_lParam		���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
//! \return		���b�Z�[�W�����̌���
/*=========================================================================*/
LRESULT CALLBACK WindowProc( HWND _hWnd , UINT _Message , WPARAM _wParam , LPARAM _lParam )
{

	// ���b�Z�[�W�ɂ���ď�����I��
	switch( _Message )
	{

	case WM_CLOSE:
		MessageBox( CAPP->GetHWND() , "�G�X�P�[�v�L�[�������ďI�����Ă�������" , NULL , MB_OK );
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	case WM_SIZE:
		CAPP->SetWindowSize();
		break;

	case WM_MOUSEWHEEL:
		MOUSE->SetWheelMovement( (SHORT)HIWORD(_wParam)/WHEEL_DELTA );
		break;

	default:
		return DefWindowProc( _hWnd , _Message , _wParam , _lParam );
	}

	return 0;
}


/*=======================================================================================*/
//! \brief �E�B���h�E���쐬����
//! \param[out]	*_FullScreenFlag	�t���X�N���[���ŋN������̂��̃t���O
//! \param[in]	_nWinMode			WinMain�̈������g�p	�E�B���h�E�̕\�����@
//! \return true  = ����
//! \return false = ���s
/*=======================================================================================*/
bool MakeWindow( bool *_FullScreenFlag , const int _nWinMode )
{

	WNDCLASSEX wc;											// �E�B���h�E�N���X�̒�`
	
	// �E�B���h�E�N���X�̒�`
	wc.cbSize = sizeof(WNDCLASSEX);							// �\���̂̃T�C�Y
	wc.style = CS_DBLCLKS;									// �X�^�C��
	wc.lpfnWndProc = WindowProc;							// �E�C���h�E�֐�
	wc.cbClsExtra = 0;										// �G�L�X�g���N���X��� 
	wc.cbWndExtra = 0;										// �G�L�X�g���E�B���h�E���
	wc.hInstance = CAPP->hInst;								// �C���X�^���X�n���h��
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// ���[�W�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);				// �X���[���A�C�R�� 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// �}�E�X�J�[�\��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// �w�i�F 
	wc.lpszMenuName = NULL;									// ���C�����j���[��
	wc.lpszClassName = WINDOW_NAME;							// �E�B���h�E�N���X��

	// �E�B���h�E�N���X�̓o�^
	if ( !RegisterClassEx( &wc ) )
	{
		ErrorMessage( "�E�B���h�E�̓o�^�Ɏ��s���܂����B\n" );
		return false;
	}

	// �X�N���[���̉𑜓x���擾
	int ScreenSizeWidth  = GetSystemMetrics( SM_CXSCREEN );
	int ScreenSizeHeight = GetSystemMetrics( SM_CYSCREEN );
	POINT WindowPos;
	WindowPos.x = ( ScreenSizeWidth  - WINDOW_WIDTH  ) / 2;
	WindowPos.y = ( ScreenSizeHeight - WINDOW_HEIGHT ) / 2;

	// �E�B���h�E�̍쐬
	CAPP->hWnd = CreateWindow(
		WINDOW_NAME,						// �쐬����E�B���h�E��
		GAME_TITLE.c_str(),					// �E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW,				// �E�B���h�E�^�C�v��W���^�C�v��	
		WindowPos.x + WINDOW_POSITION_X ,	// �E�B���h�E�̈ʒu�i�w���W�j
		WindowPos.y + WINDOW_POSITION_Y ,	// �E�B���h�E�̈ʒu�i�x���W�j						
		WINDOW_WIDTH,						// �E�B���h�E�̕�
		WINDOW_HEIGHT,						// �E�B���h�E�̍���			
		NULL,								// �e�E�B���h�E�̃n���h��
		NULL,								// ���j���[�̃n���h��
		CAPP->hInst,						// �C���X�^���X�n���h�� 
		NULL);								// �ǉ���� 


	// �E�B���h�E�̕\��
#ifdef _DEBUG
	ShowWindow( CAPP->hWnd , bFullScreen ? ( SW_SHOW ) : ( _nWinMode ) );
#else
	int Select;
	Select = MESSAGE( "�t���X�N���[���ŋN�����܂����H" );
	*_FullScreenFlag = Select == IDYES ? ( true ) : ( false );
	ShowWindow( CAPP->hWnd , Select == IDYES ? ( SW_SHOW ) : ( _nWinMode ) );
#endif

	// �E�B���h�E�̍X�V
	UpdateWindow( CAPP->hWnd );

	return true;

}


/*=========================================================================*/
//! \brief �E�B���h�E�̃N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
//! \param[in]	_hWnd		�E�B���h�E�n���h��
//! \param[in]	_Width		�E�B���h�E�̕�
//! \param[in]	_Height		�E�B���h�E�̍���
/*=========================================================================*/
void SetClientSize( const HWND _hWnd , const int _Width , const int _Height )
{

	RECT rcWnd, rcCli;

	GetWindowRect( _hWnd , &rcWnd );	// �E�B���h�E��RECT�擾
	GetClientRect( _hWnd , &rcCli );	// �N���C�A���g�̈��RECT�擾

	// �E�B���h�E�̗]�����l���āA�N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
	MoveWindow( _hWnd ,
		rcWnd.left ,	// X���W
		rcWnd.top ,		// Y���W
		_Width  + ( rcWnd.right  - rcWnd.left ) - ( rcCli.right  - rcCli.left ),
		_Height + ( rcWnd.bottom - rcWnd.top  ) - ( rcCli.bottom - rcCli.top  ),
		TRUE );

}

