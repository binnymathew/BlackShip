#include <windows.h>

#define WM_DHRUVA 10000

void test(HWND hWnd)
{
	SendMessage(hWnd,WM_DHRUVA, 0,0);
}



long FAR PASCAL WndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
	
	switch(message)
	{
	case WM_KEYDOWN:
		test(hWnd);
		break;
		
	case WM_DHRUVA:
		MessageBox(hWnd,"Hello Dhruva", "Info",MB_OK);
		break;
		
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{

	MSG msg;
	WNDCLASS wndclass;
	HWND hWnd;

	if(!hPrevInstance)
	{
		wndclass.style = (UINT) NULL;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = (LPSTR) "cl1menu";
		wndclass.lpszClassName = (LPSTR)"Cl1";

		RegisterClass(&wndclass);
	}
	hWnd = CreateWindow("Cl1", "Client 1 - uses WSAAsyncSelect", 
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
			 CW_USEDEFAULT	, CW_USEDEFAULT,
			 CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&msg,NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (msg.wParam);
}

