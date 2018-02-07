#include <windows.h>

New Line

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
/*	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;

	hInst = hInstance;

	if(!hPrevInstance)
	{
		wndclass.style = (UINT) NULL;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
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

	
	return (msg.wParam);*/

	MessageBox(NULL,"Test Message Box fhgkjfdhgfd kdsfkjhs jkghskjgh hkjdfh sdkjhf","Test Box", MB_OK);

}