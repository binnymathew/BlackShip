#include "Main.h"

void FindFileNames()
{
   	HANDLE hFileFirstChange;
	WIN32_FIND_DATA  aData;
	//DWORD dAttrib;

	if((hFileFirstChange = FindFirstFile("c:\\Binny\\Temp\\*.*",&aData)) 
				== INVALID_HANDLE_VALUE )
	{
		MessageBox(NULL,"Error in FindFirstFile","Error",MB_OK);
		return ;
	}
	while(FindNextFile(hFileFirstChange ,&aData) != FALSE)
	{
		if((!lstrcmp(aData.cFileName,".")) || (!lstrcmp(aData.cFileName,"..")))
			continue;
				
		/*dAttrib = GetFileAttributes(cFileName);
		if(dAttrib == 0xFFFFFFFF)
		{
			MessageBox(NULL,"Error in GetFileAttributes","Error",MB_OK);
			return ;
		}
		if(dAttrib == FILE_ATTRIBUTE_ARCHIVE)*/

		if(aData.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
		{
			lstrcpy(cFileName,"c:\\Binny\\Temp\\");
			lstrcat(cFileName,aData.cFileName);
			if(SetFileAttributes(cFileName,FILE_ATTRIBUTE_NORMAL) == FALSE)
			{
				MessageBox(NULL,"Error in SetFileAttributes","Error",MB_OK);
				return ;
			}
			char szName[15];
			sprintf(szName,"c:\\Binny\\t\\%s",aData.cFileName);
			CopyFile(cFileName,szName,FALSE);
			//memset(cFileName,0,100);
			lstrcpy(cFileName,"c:\\Binny\\t\\");
			lstrcat(cFileName,aData.cFileName);
			SendMessage(hWnd,WM_WSAASYNC, 0, FD_WRITE);
		}
				
		if(aData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL)
			continue ;
	}
	FindClose(hFileFirstChange);
}

void Function()
{
	hFileChange = FindFirstChangeNotification("c:\\Binny\\Temp",
					FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
	if(hFileChange  == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"Error in FindFirstChangeNotification",
			"Error",MB_OK);
	}
	WaitForSingleObject(hFileChange,INFINITE);
	FindFileNames();

	//MessageBox(NULL,"After WaitForSingleObject()","Notification",MB_OK);
	
	while(FindNextChangeNotification(hFileChange) != FALSE)
	{
		if(hFileChange  == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL,"Error in FindFirstChangeNotification",
				"Error",MB_OK);
		}
		WaitForSingleObject(hFileChange,INFINITE);
		FindFileNames();
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MakeConnection(HWND hWnd)
{

	HCURSOR hCursor;

	// Variables used in Winsock calls
	WSADATA 	wsadata;
	WORD		wVer;
	struct sockaddr_in	addr_server;

	int  iRc;
	DWORD	dwIP;
	LPHOSTENT	lphp;

	// Initiate use of WINSOCK.DLL
	wVer = MAKEWORDD(1, 1);
	iRc = WSAStartup(wVer, &wsadata);
	if(iRc != 0)
	{
		MessageBox(NULL, "WSAStartup Error ", "Client Error", MB_OK);
		return;
	}

	// ensure the WINSOCK>DLL version is right
	if(LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		MessageBox(NULL, "Version  Error ", "Client Error", MB_OK);
		WSACleanup();
		return;
	}

	// Create client socket
	sd_cl = socket(PF_INET, SOCK_STREAM, 0);

	if(sd_cl == INVALID_SOCKET)
	{
		MessageBox(NULL, "Socket creation  Error ", "Client Error", MB_OK);
		return;
	}

	// set server address fields  Set adress to connect to
	addr_server.sin_family = PF_INET;
	addr_server.sin_port = htons(iDefPort);

	// See if user entered an IP address value
	dwIP = inet_addr(szServerName);
	if(dwIP != INADDR_NONE)
	// Have IP address
	addr_server.sin_addr.s_addr = dwIP;
	else
	{
		SetCapture(hWnd);
		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

		lphp = gethostbyname(szServerName);
		if (lphp == NULL)
		{
			// error in gethostbyname()
			SetCursor(hCursor);
			ReleaseCapture();
			MessageBox(NULL, "Get Host  Error ", "Client Error", MB_OK);
			return;
		}
		SetCursor(hCursor);
		ReleaseCapture();
		addr_server.sin_addr.s_addr = ((struct in_addr far *) (lphp->h_addr))->s_addr;
	}
			
	// request notification of incoming connections
	iRc = WSAAsyncSelect(sd_cl, hWnd, WM_WSAASYNC, FD_CONNECT | FD_WRITE | FD_CLOSE);
	//char szTTT[50];
	//sprintf(szTTT,"sd_cl = %d,WSAGetLastError = %d",sd_cl,WSAGetLastError());
	//MessageBox(NULL, szTTT, "WSAGetLastError()", MB_OK);
		
	if(iRc == SOCKET_ERROR)
	{
		MessageBox(NULL, "WSAAsyncSelect Error ", "Client Error", MB_OK);
		return;
	}
				
	// request connection
	iRc = connect(sd_cl, (struct sockaddr far *) &addr_server,sizeof(addr_server));
	if(iRc == SOCKET_ERROR)
	{
		if(iRc = WSAGetLastError() != WSAEWOULDBLOCK)
		{
			MessageBox(NULL, "Connect Error ", "Client Error", MB_OK);
			return;
		}
	}
	// noew wait for the FD_CONNECT message to complete the connection
	return;
}

void WriteTheData()
{
	DWORD dwFileSize;
	DWORD dwRead;

	int i, iRc;

	hFile = CreateFile(cFileName,GENERIC_READ|GENERIC_WRITE,0, 
		(SECURITY_ATTRIBUTES *)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	//char szT[200];
	//sprintf(szT,"GetLast error in CreateFile = %d",GetLastError());
	//MessageBox(NULL,szT,"CreateFile",MB_OK);
	DWORD dd = GetLastError();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		if(lstrlen(cFileName) != 0)
		{
			char szTemp[25];
			sprintf(szTemp,"Error in opening the file %s",cFileName);
			MessageBox(NULL,szTemp,"File Error",MB_OK);
		}
		return;
	}
			
	dwFileSize = GetFileSize(hFile,NULL);
			
	ReadFile(hFile,sData,dwFileSize ,&dwRead,NULL);
	sData[dwRead+1] = '\0';
	MessageBox(NULL,sData,"Client:File Data",MB_OK);
	if(CloseHandle(hFile) == 0)
	{
		MessageBox(NULL,"Error in closing the file","client:Error",MB_OK);
		return;
	}
	/*if(DeleteFile("c:\\binny\\test.txt") == 0)
	{
		MessageBox(NULL,"Error in deleting the file","client:Error",MB_OK);
		return(0);
	}*/
	iFlag = TRUE;
	for(i = 0; i < 5; i++)
	{
		char szTTT[25];

		LPVOID lpMsgBuf;

		iRc = send(sd_cl, (char FAR *)sData, sizeof(sData), 0);
		if(WSANOTINITIALISED == WSAGetLastError())
			MessageBox(NULL,"WSANOTINITIALISED ","Error",MB_OK);
		
		//sprintf(szTTT,"iRc  = %d, WsaGetLastError = %d",iRc,WSAGetLastError());
		//MessageBox(NULL,szTTT,"Error",MB_OK);

		if(iRc == MAXOUTMSGLEN)
		{
			//MessageBox(NULL,"Breaking the send loop","Error",MB_OK);
			break;
		}
		Sleep((DWORD)1000);
	}
			
	if(iRc == SOCKET_ERROR)
	{
		MessageBox(NULL, "Send  Error ", "Client Error", MB_OK);
		return;
	}
		
	if(iRc != sizeof(sData))
	{
		MessageBox(NULL, "Sent size error Error ", "Client Error", MB_OK);
		return;
	}
						
	return;
}

