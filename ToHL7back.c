#include <windows.h>
#include <malloc.h>
#include <stdio.h>


#define FIELD_SEPARATOR				'|'
#define COMPONENT_SEPARATOR			'^'
#define SUB_COMPONENT_SEPARATOR		'&'
#define	REPETITION_SEPARATOR		'&'
#define ESCAPE_CHARACTER			'\\'


char cHL7MessageBuffer[1000];
char cDataBuffer[1000];

void CreateMessageHeader(char cMsgHeader[100],char cTime[15])
{
	char cMessageType[10];
	char cTemp[10];
	int i = 0, j = 0;

	memset(cMessageType, 0, 10);
	memset(cMsgHeader, 0, 10);
	while(lpszInputBuffer[i] != 13)
	{
		cTemp[j] = lpszInputBuffer[i];
		i++; j++;
	}
	cTemp[j] = 0;
	lstrcpy(cMessageType,cTemp);
	
	j = 0; i+=2;
	while(lpszInputBuffer[i] != 13)
	{
		cTemp[j] = lpszInputBuffer[i];
		i++;j++;
	}
	cTemp[j] = 0;
	cMessageType[lstrlen(cMessageType)] = COMPONENT_SEPARATOR;
	//lstrcat(cMessageType,COMPONENT_SEPARATOR);
	lstrcat(cMessageType,cTemp);
	lstrcpy(cTemp,"^~\\&");
	sprintf(cMsgHeader,"MSH%c%s%cHMS%cHL7%cCITATION%cHL7%c%s%c%c%s%c%cP%c2.2%c%c",
		FIELD_SEPARATOR,cTemp,FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR,
		FIELD_SEPARATOR,cTime,FIELD_SEPARATOR,FIELD_SEPARATOR,cMessageType,FIELD_SEPARATOR,
		FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR);
}



int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE hFileRead;
	HANDLE hFileWrite;
	char cTime[15];
	char cMsgHeader[100];

	DWORD dwFileSize, dwReadFlag;
	SYSTEMTIME ts;

	GetLocalTime(&ts);
	sprintf(cTime,"%i%i%i%i%i%i",ts.wYear,ts.wMonth,ts.wDay,ts.wHour,ts.wMinute,ts.wSecond);
	
	
	hFileRead = CreateFile("C:\\binny\\outfile",GENERIC_READ|GENERIC_WRITE,0, 
				(SECURITY_ATTRIBUTES *)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hFileRead == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"FILE OPENING ERROR","Error Client",MB_OK);
		return;
	}
	dwFileSize = GetFileSize(hFileRead,NULL);
	lpszInputBuffer = (char *)malloc(sizeof(char)*dwFileSize );
			
	ReadFile(hFileRead,lpszInputBuffer,dwFileSize ,&dwReadFlag,NULL);
	lpszInputBuffer[dwReadFlag+1] = '\0';

	if(CloseHandle(hFileRead) == 0)
	{
		MessageBox(NULL,"Error in closing the file","client:Error",MB_OK);
		return;
	}
	CreateMessageHeader(cMsgHeader,cTime);
	MessageBox(NULL,cMsgHeader,"Info",MB_OK);

}