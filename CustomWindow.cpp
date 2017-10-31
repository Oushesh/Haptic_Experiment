//Custom Window
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool ReconnectIO(bool OpenNewConsole);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASS wc;

	if (!ReconnectIO(false))
		printf("Started from command prompt\n");

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = "Window";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClass(&wc);
	hwnd = CreateWindow(wc.lpszClassName, "Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 350, 250, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	printf("Entering GetMessage loop\n");
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/*******************************************************************************
* NAME:
*    ReconnectIO
*
* SYNOPSIS:
*    bool ReconnectIO(bool OpenNewConsole);
*
* PARAMETERS:
*    OpenNewConsole [I] -- This controls if we open a console window or not.
*                          True -- if the program was not started from an
*                                  existing console open a new console window.
*                          False -- Only connect stdio if the program was
*                                   started from an existing console.
*
* FUNCTION:
*    This function connects up the stardard IO (stdout, stdin, stderr) to
*    the windows console.  It will open a new console window if needed
*    (see 'OpenNewConsole').
*
* RETURNS:
*    true -- A new console window was opened
*    false -- Using an existing console window
*
* SEE ALSO:
*
******************************************************************************/
bool ReconnectIO(bool OpenNewConsole)
{
	int hConHandle;
	long lStdHandle;
	FILE *fp;
	bool MadeConsole;

	MadeConsole = false;
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		if (!OpenNewConsole)
			return false;

		MadeConsole = true;
		if (!AllocConsole())
			return false;   // Could throw here
	}

	// STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// C++ streams to console
	std::ios_base::sync_with_stdio();

	return MadeConsole;
}