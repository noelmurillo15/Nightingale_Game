#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>		//	Win32 Application
#include <vld.h>			//	Visual Leak Detector!!!
#include "Game.h"			//	Our Game class
#include "PauseGameMsg.h"	//	Pause game during alt + tab
#include "CharacterSelect.h"//	Get instantiation of gameplaystate
#include "../resource.h"

#define WINDOW_CLASS_NAME	((const wchar_t*)L"SGP Game Project")		// window class name
#define WINDOW_TITLE		((const wchar_t*)L"Nightingale")			// window title

int WINAPI			WinMain				( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
bool				IsAlreadyRunning	( void );
HWND				MakeWindow			(HINSTANCE hInstance, SGD::Size screensize);
LRESULT CALLBACK	WindowProc			( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

		// Don't let more than one instance of the application exist
	if( IsAlreadyRunning() == true )
		return -1;		

		//	Determine screen size
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

    
		// Create the window
	HWND hWnd = MakeWindow(hInstance, { (float)desktop.right - 256.0f, (float)desktop.bottom - 256.0f });
	if( hWnd == 0 )
	{
		MessageBoxW( HWND_DESKTOP, L"ERROR: Failed to create the Main Window.", WINDOW_TITLE, MB_OK | MB_ICONERROR );
		return -2;
	}

		// Display the window
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );	

		// Access the singleton
	Game* pGame = Game::GetInstance();

	if ((int)desktop.right == 800 && (int)desktop.bottom == 600){
		pGame->Initialize((float)desktop.right, (float)desktop.bottom);
		pGame->SetCurrentRes(Game::RES_ARCADE);
	}
	else
		pGame->Initialize((float)desktop.right - 256.0f, (float)desktop.bottom - 256.0f);


		// Run the message loop
	MSG msg = { };
	while( true )
	{
		if( PeekMessageW( &msg, NULL, 0, 0, PM_REMOVE ) == TRUE )
		{ 
				// Quit the application?
			if( msg.message == WM_QUIT )
				break;
		
				// Send the message to the window proc
			DispatchMessageW( &msg );
		}
		else
		{
				// Run game
			int result = pGame->Update();
			if( result != 0 )
				PostQuitMessage( result );
		}
	}
	
		// Terminate game
	pGame->Terminate();
	pGame = nullptr;
	Game::DeleteInstance();

		// Unregister the window class
	UnregisterClassW( WINDOW_CLASS_NAME, hInstance );
	
		// Return message's Quit code to the OS
	return (int)(msg.wParam);
}

bool IsAlreadyRunning( void )
{
		// Is there another window with the same class name & title?
	HWND hOtherWnd = FindWindowW( WINDOW_CLASS_NAME, WINDOW_TITLE );
	if( hOtherWnd != 0 )
	{
			// Is it minimized?
		if( IsIconic( hOtherWnd ) == TRUE )
			ShowWindow( hOtherWnd, SW_RESTORE );

			// Bring it to the foreground
		SetForegroundWindow( hOtherWnd );
		return true;
	}

		// No other instances are running
	return false;
}

HWND MakeWindow(HINSTANCE hInstance, SGD::Size screensize)
{
		// Window class description
	WNDCLASSEXW	wc;
	HICON hIcon; 
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
 	

		// Set window class attributes
	wc.cbSize			= sizeof(wc);
	wc.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= hIcon;
	wc.hIconSm			= hIcon;
	wc.hCursor			= LoadCursorW( NULL, (LPWSTR)IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)(1+COLOR_WINDOW);
	wc.lpszMenuName		= NULL; 
	wc.lpszClassName	= WINDOW_CLASS_NAME;

		// Register the window class
	ATOM atomClass = RegisterClassExW( &wc );
	if( atomClass == 0 )
		return 0;

	DWORD dwWindowStyleFlags;
		// Setup window style flags
	if (screensize == SGD::Size(800, 600))
		dwWindowStyleFlags = WS_POPUP;
	else
		dwWindowStyleFlags = WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		// Setup the desired client area size
	RECT rWindow;
	rWindow.left	= 0;
	rWindow.top		= 0;
	rWindow.right	= (LONG)screensize.width;
	rWindow.bottom = (LONG)screensize.height;

		// Get the dimensions of a window that will have a client rect that
		// will really be the resolution we're looking for.
	AdjustWindowRectEx( &rWindow, dwWindowStyleFlags, FALSE, WS_EX_APPWINDOW );
	
		// Calculate the the window's dimensions
	int nWindowWidth	= rWindow.right  - rWindow.left;
	int nWindowHeight	= rWindow.bottom - rWindow.top;

		// Create the window
	HWND hWnd = CreateWindowExW(
					WS_EX_APPWINDOW,										// extended style flags
					WINDOW_CLASS_NAME,										// window class Name
					WINDOW_TITLE,											// title
					dwWindowStyleFlags,										// window style flags
					(GetSystemMetrics(SM_CXSCREEN) - nWindowWidth)/2,		// position (x, y)
					(GetSystemMetrics(SM_CYSCREEN) - nWindowHeight)/2,		//	- centered on the desktop
					nWindowWidth,											// width
					nWindowHeight,											// height
					HWND_DESKTOP,											// parent window handle
					NULL,													// menu handle
					hInstance,												// application instance
					NULL													// creation parameters
				);

	if (hIcon)
	{
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		DestroyIcon(hIcon);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		DestroyIcon(hIcon);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
		DestroyIcon(hIcon);
	}
	
		// Did that fail?
	if( hWnd == 0 )
		UnregisterClassW( WINDOW_CLASS_NAME, hInstance );

		// Return the handle to the new window
	return hWnd;
}

LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

		// What is the message?
	switch( msg ) {

	case WM_CREATE:			// Window constructed
		break;
	
	case WM_CLOSE:			// Window closed
		DestroyWindow( hWnd );	// completely destroy the window
		break;

	case WM_DESTROY:		// Window destroyed
		PostQuitMessage( 0 );	// completely quit the application
		break;

	case WM_SYSKEYUP:		// System menu (ALT pop-up)
		break;

	case WM_SYSCHAR:
		break;

	case WM_ACTIVATE:		// Window activated / deactivated
		if( LOWORD( wParam ) != WA_INACTIVE )	//	gaining focus (unpause)
		{

		}
		else {								//	losing focus (pause) 
			if (CharacterSelect::GetInstance()->GetIsCharacterInstantiated()){
				PauseGameMsg* msg = new PauseGameMsg();
				msg->SendMessageNow();
				delete msg;
			}
		}
		break;

	case WM_PAINT:			// Window needs repainting
		ValidateRect( hWnd, nullptr );	// ignore painting
		break;
	
	default:			
		return DefWindowProcW( hWnd, msg, wParam, lParam );	// handled by default proc
	}
	return 0;
}