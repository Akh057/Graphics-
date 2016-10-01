//Code listing for WMain.cpp
#include <windows.h>

char szWinName[] = "MyWin";
char szTitle[] = "Parabola Drawing";

/* This is where all the input to the window goes to */
extern LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	WNDCLASSEX wcl; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	wcl.cbClsExtra = 0;//extra bytes
    wcl.cbSize = sizeof(WNDCLASSEX); //size of the struct
  	wcl.cbWndExtra = 0;//extra bytes
  	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  	wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
  	wcl.hIcon = LoadIcon(NULL,IDI_WINLOGO);
  	wcl.hIconSm = NULL;
  	wcl.hInstance = hThisInst;
  	wcl.lpfnWndProc = WndProc;
  	wcl.lpszClassName = szWinName;
  	wcl.lpszMenuName = "MyMenu";
  	wcl.style = 0;




	if(!RegisterClassEx(&wcl)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

  hwnd = CreateWindow(
    szWinName,
    szTitle,
    WS_OVERLAPPEDWINDOW|WS_SYSMENU,// no scroll bar
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    640, // specific value instead of CW_USEDEFAULT 
    480, // specific value instead of CW_USEDEFAULT 
    HWND_DESKTOP,
    NULL,
    hThisInst,
    NULL
   );
   
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}


 	ShowWindow(hwnd, SW_RESTORE);
  	UpdateWindow(hwnd);

  	//Serve Messages to the callback function and blocks until it recieves smething 
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}














