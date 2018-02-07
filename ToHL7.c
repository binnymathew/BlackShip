#include "ToHL7.h"

/* Structure to hold triggers, Segments and Segments, Item Number.
 * It is represented by using a tree structure. The diagramatic representation
 * of the tree is:
 *
 * |-A01/SEG1----|-SEG1/DATA_TYPE
 * |             |     
 * |             |     
 * |             |     
 * |             |
 * |             |-SEG2/DATA_TYPE
 * |             |     
 * |             |     
 * |             | ... 
 * |
 * |-A02/SEG1----|-SEG1/DATA_TYPE
 * |             |     
 * |             |     
 * |             |     
 * |             | 
 * |             |-SEG2/DATA_TYPE
 * |             |     
 * |             |     
 * |             | ... 
 * ... 
 * This is implemented using link list
 * The structure contains 4 fields. 
 * iType      => Identifies whether it is a trigger, Segment or Item Number
 * szData  => The value of trigger or Segment or item Number
 * lpNext     => Pointer to the next node of the same item
 * lpBranch   => Pointer to the next node of different item
 */

typedef struct StructNode
{
	int iType;
	char szData[4];
	struct StructNode *lpNext;
	struct StructNode *lpBranch;
}Node;

//Buffer which contain HL7 message
char szHL7MessageBuffer[1000];
//Buffer which contain Data to be converted to HL7 message
char szDataBuffer[1000];
Node *lpTriggerHead;
Node *lpSegmentHead;


/*****************************************************************************
 * This function the linked list is filled. Both trigger and segment is filled.
 *****************************************************************************/

void FillTrigger_Segment(char *lpstrMainBuffer, DWORD dwFileSize)
{
	int i = 0, j = 0, k = 0;
	char szOneLine[300];

	lpTriggerHead = NULL;
	while((lpstrMainBuffer[i] != 0) && ((DWORD)i < dwFileSize))
	{
		j = 0;
		//Get one Line
		while(lpstrMainBuffer[i] != 10)
		{
			szOneLine[j] = lpstrMainBuffer[i];
			i++; j++;
		}
		i++;
		szOneLine[j] = 0;
		j = 0;
		while(szOneLine[j] != 0)
		{
			Node *Temp;
			Node *Temp1;
			char szOneWord[4];

			k = 0;
			Temp = (Node *)malloc(sizeof(Node));
			Temp->lpNext = NULL;
			Temp->lpBranch = NULL;
			while((szOneLine[j] != ' ') && (szOneLine[j] != 0x0D))
			{
				szOneWord[k] = szOneLine[j];
				k++;j++;
			}
			szOneWord[k] = 0;
			if( j <= 3 )
				Temp->iType = TRIGGER;
			else
				Temp->iType = SEGMENT;
			j++;
			lstrcpy(Temp->szData, szOneWord);
			if(lpTriggerHead == NULL)
			{
				lpTriggerHead = Temp;
				continue;
			}
			
			Temp1 = lpTriggerHead;

			if(Temp->iType == SEGMENT)
			{
				while(Temp1->lpNext != NULL)
					Temp1 = Temp1->lpNext;
				
				if(Temp1->lpBranch == NULL)
					Temp1->lpBranch = Temp;
				else
				{
					Temp1 = Temp1->lpBranch;
					while(Temp1->lpNext != NULL)
						Temp1 = Temp1->lpNext;
					Temp1->lpNext = Temp;
				}
			}
			if(Temp->iType == TRIGGER)
			{
				while(Temp1->lpNext != NULL)
					Temp1 = Temp1->lpNext;
				Temp1->lpNext = Temp;
			}
		}
	}
}


/*****************************************************************************
 * This function the linked list is filled. Only the item_number is filled.
 * The segment name is used for placing the item_number in the right place.
 *****************************************************************************/

void FillSegment_DataType(char *lpstrMainBuffer, DWORD dwFileSize)
{
	int i = 0, j = 0, k = 0;
	char szOneLine[300];
	
	lpSegmentHead = NULL;
	while((lpstrMainBuffer[i] != 0) && ((DWORD)i < dwFileSize))
	{
		j = 0;
		//Get one Line
		while(lpstrMainBuffer[i] != 10)
		{
			szOneLine[j] = lpstrMainBuffer[i];
			i++; j++;
		}
		i++;
		szOneLine[j] = 0;
		j = 0;
		while(szOneLine[j] != 0)
		{
			Node *Temp;
			Node *Temp1;
			char szOneWord[6];

			k = 0;
			Temp = (Node *)malloc(sizeof(Node));
			Temp->lpNext = NULL;
			Temp->lpBranch = NULL;
			while((szOneLine[j] != ' ') && (szOneLine[j] != 0x0D))
			{
				szOneWord[k] = szOneLine[j];
				k++;j++;
			}
			szOneWord[k] = 0;

			if( j <= 3 )
				Temp->iType = SEGMENT;
			else
				Temp->iType = ITEM_NUMBER;
			
			lstrcpy(Temp->szData, szOneWord);
			j++;
			if(lpSegmentHead == NULL)
			{
				lpSegmentHead = Temp;
				continue;
			}
			Temp1 = lpSegmentHead;


			if(Temp->iType == ITEM_NUMBER)
			{
				while(Temp1->lpNext != NULL)
					Temp1 = Temp1->lpNext;
				
				if(Temp1->lpBranch == NULL)
					Temp1->lpBranch = Temp;
				else
				{
					Temp1 = Temp1->lpBranch;
					while(Temp1->lpNext != NULL)
						Temp1 = Temp1->lpNext;
					Temp1->lpNext = Temp;
				}
			}
			if(Temp->iType == SEGMENT)
			{
				while(Temp1->lpNext != NULL)
					Temp1 = Temp1->lpNext;
				Temp1->lpNext = Temp;
			}

			/*
*			while(Temp1 != NULL)
*			{
*				int iFlag = 0;
*				int iFlag = 0;
*
*				Temp2 = Temp1->lpBranch;
*
*				while(lstrcmp(Temp2->szData,szSegment) != 0)
*				{
*					Temp2 = Temp2->lpNext;
*					if(Temp2 == NULL)
*					{
*						iFlag = 1;
*						break;
*					}
*				}
*				if(iFlag == 0)
*				{
*					if(Temp2->lpBranch == NULL)
*						Temp2->lpBranch = Temp;
*					else
*					{
*						Temp2 = Temp2->lpBranch;
*						while(Temp2->lpNext != NULL)
*							Temp2 = Temp2->lpNext;
*						Temp2->lpNext = Temp;
*					}
*					break;
*				}
*				Temp1 = Temp1->lpNext;
*			}*/

		}
	}
}

/*****************************************************************************
 * This function the linked list is filled. This calls two functions to do the job
 *****************************************************************************/


void FillLinkList()
{
	HANDLE hFile;
	int iFlag;
	DWORD dwFileSize, dwRead;
	char *lpstrMainBuffer;
	
	// Allocate memory for the Head node


	//lpHead = (Node *)malloc(sizeof(Node));

	hFile = CreateFile("c:\\Binny\\MessageChecklist.txt",GENERIC_READ|GENERIC_WRITE,0, 
				(SECURITY_ATTRIBUTES *)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"File opening the file c:\\Binny\\MessageChecklist.txt","ERROR",MB_OK);
		return;
	}
	
	dwFileSize = GetFileSize(hFile,NULL);
	lpstrMainBuffer = (char *)malloc(dwFileSize + 1);
	if(lpstrMainBuffer == NULL)
	{
		MessageBox(NULL,"Malloc failed","ERROR",MB_OK);
		return;
	}
			
	iFlag = ReadFile(hFile,lpstrMainBuffer, dwFileSize ,&dwRead,NULL);
	lpstrMainBuffer[dwRead+1] = 0;
	if ((iFlag == 0) || (dwFileSize != dwRead))
	{
		MessageBox(NULL,"Error in reading the file c:\\Binny\\MessageChecklist.txt","Error",MB_OK);
		return;
	}
	if(CloseHandle(hFile) == 0)
	{
		MessageBox(NULL,"Error in closing the file c:\\Binny\\MessageChecklist.txt","Error",MB_OK);
		return;
	}
	FillTrigger_Segment(lpstrMainBuffer, dwFileSize );

      /////////////////////////////////////////////////////////////////////////////////////////

	hFile = CreateFile("c:\\Binny\\SegmentChecklist.txt",GENERIC_READ|GENERIC_WRITE,0, 
				(SECURITY_ATTRIBUTES *)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"File opening the file c:\\Binny\\SegmentChecklist.txt","ERROR",MB_OK);
		return;
	}
	//memset(lpstrMainBuffer,0,dwFileSize);

	dwFileSize = GetFileSize(hFile, NULL);
	lpstrMainBuffer = (char *)realloc(lpstrMainBuffer, dwFileSize + 1);
					
	iFlag = ReadFile(hFile,lpstrMainBuffer, dwFileSize ,&dwRead,NULL);
	lpstrMainBuffer[dwRead+1] = 0;
	if ((iFlag == 0) || (dwFileSize != dwRead))
	{
		MessageBox(NULL,"Error in reading the file c:\\Binny\\SegmentChecklist.txt","Error",MB_OK);
		return;
	}
	if(CloseHandle(hFile) == 0)
	{
		MessageBox(NULL,"Error in closing the file c:\\Binny\\SegmentChecklist.txt","Error",MB_OK);
		return;
	}

	FillSegment_DataType(lpstrMainBuffer, dwFileSize );
}
				
/*****************************************************************************
 * This function created the Message Header.
 *****************************************************************************/


void CreateMessageHeader(char szMsgHeader[100])
{
	char szTrigger[8];
	char szTemp[10];
	char szTime[15];
	SYSTEMTIME ts;

	int i = 0, j = 0;
	
	memset(szTrigger, 0, 8);
	memset(szMsgHeader, 0, 100);

	GetLocalTime(&ts);
	sprintf(szTime,"%i%i%i%i%i%i",ts.wYear,ts.wMonth,ts.wDay,ts.wHour,ts.wMinute,ts.wSecond);

	//Get the trigger
	while(szDataBuffer[i] != 10)
	{
		szTemp[j] = szDataBuffer[i];
		i++; j++;
	}
	szTemp[j] = 0;

	//Compute the Message type from the trigger.

	lstrcpy(szTrigger,"ADT");

	szTrigger[lstrlen(szTrigger)] = COMPONENT_SEPARATOR;
	szTemp[j+1] = 0;
	lstrcat(szTrigger,szTemp);
	
	lstrcpy(szTemp,"^~\\&");
	sprintf(szMsgHeader,"MSH%c%s%cHMS%cHL7%cCITATION%cHL7%c%s%c%c%s%c%cP%c2.2%c%c",
		FIELD_SEPARATOR,szTemp,FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR,
		FIELD_SEPARATOR,szTime,FIELD_SEPARATOR,FIELD_SEPARATOR,szTrigger,FIELD_SEPARATOR,
		FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR,FIELD_SEPARATOR);
}


void ConvertTheRest()
{
	Node *lpTempTrigger;
	Node *lpTempSegment;

	int i = 0, j = 0;
	char szOneLine[100];
	char szHL7[100];
	
	// Temporary pointers pointing to the head of trigger tree
	lpTempTrigger = lpTriggerHead;
	
	
	// Scan the whole buffer and make the HL7 message
	while(szDataBuffer[i] != 0)
	{
		j = 0;
		memset(szHL7, 0, 100);
		// Temporary pointers pointing to the head of segment tree
		lpTempSegment = lpSegmentHead;

		//If there is no data in the line, then that segment is not present.
		if(szDataBuffer[i] == 10)
		{
			i++;
			lpTempTrigger = lpTempTrigger->lpNext;
			continue;
		}


		// Get one line. This will be one segment.
		while(szDataBuffer[i] != 10)
		{
			szOneLine[j] = szDataBuffer[i];
			i++; j++;
			if(szDataBuffer[i] == 0)
				break;
		}
		szOneLine[j] = 0;
		j = 0;
		i++;

		if(i <= 4)
		{
			// Find the trigger value, and make the pointer lpTempTrigger points to that 
			while(lstrcmp(lpTempTrigger->szData,szOneLine) != 0)
				lpTempTrigger = lpTempTrigger->lpNext;

			//Make the pointer lpTempTrigger  points to the branch of the trigger
			lpTempTrigger = lpTempTrigger->lpBranch;
			continue;
		}
		
		
		//From the segment name from the lpTempTrigger, get the segment from the 
		//lpTempSegment.
		while(lstrcmp(lpTempTrigger->szData, lpTempSegment->szData) != 0)
			lpTempSegment = lpTempSegment->lpNext;

		//Copy the Segment name.
		lstrcpy(szHL7,lpTempTrigger->szData);
		szHL7[lstrlen(szHL7)] = FIELD_SEPARATOR;

		lpTempSegment  = lpTempSegment->lpBranch;

		//Scan the line and generate the HL7 Message
		while(szOneLine[j] != 0)
		{
			char szOneWord[100];
			int iIndex1 = 0;

			
			//Get one word, ie get data util it encounters a '|'
			while(szOneLine[j] != FIELD_SEPARATOR)
			{
				szOneWord[iIndex1] = szOneLine[j];
				iIndex1++; j++;
			}
			szOneWord[iIndex1] = 0;
			j++;

			if(lstrlen(szOneWord) == 0)
			{
				szHL7[lstrlen(szHL7)] = FIELD_SEPARATOR;
				lpTempSegment = lpTempSegment->lpNext;
				continue;
			}

			if(lstrcmp(lpTempSegment->szData, STRING) == 0)
				lstrcat(szHL7,szOneWord);
			else if(lstrcmp(lpTempSegment->szData, TEXT_DATA) == 0)
				lstrcat(szHL7,szOneWord);
			else if(lstrcmp(lpTempSegment->szData,NUMERIC) == 0)
				lstrcat(szHL7,szOneWord);
			else if(lstrcmp(lpTempSegment->szData, DATE) == 0)
				lstrcat(szHL7,szOneWord);
			else if(lstrcmp(lpTempSegment->szData, TIME) == 0)
				lstrcat(szHL7,szOneWord);
			else if(lstrcmp(lpTempSegment->szData, TELEPHONE_NUMBER) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData, PERSON_NAME) == 0)
			{
				int iLen;
				iIndex1 = 0;
				iLen = lstrlen(szHL7);
				while(szOneWord[iIndex1] != 0)
				{
					if(szOneWord[iIndex1] == ' ')
						szHL7[iLen] = COMPONENT_SEPARATOR;
					else
						szHL7[iLen] = szOneWord[iIndex1];
					iLen++; iIndex1++;
				}
				szHL7[iLen] = 0;
			}
			szHL7[lstrlen(szHL7)] = FIELD_SEPARATOR;
			lpTempSegment = lpTempSegment->lpNext;
		}
		lstrcat(szHL7MessageBuffer,szHL7);
		lpTempTrigger = lpTempTrigger->lpNext;
	}
}

		

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	char szMsgHeader[100];
		
	lpTriggerHead = NULL;
	lpSegmentHead = NULL;
	//Read file and fill the struct
	
	FillLinkList();
	
	//Fill the buffer cHL7MessgeBuffer with NULL characters.
	memset(szHL7MessageBuffer, 0, 1000);

	//This is just to put values into the variable cDataBuffer. This will be replaced 
	//with a fucntion which will get the value from the database.
	lstrcpy(szDataBuffer,"A02\n12345|19980514||100||\n12345678||Binny Mathew|\n\n\n|||bin|ny|mat|he|w|||||");

		//Create MSH Segment
	CreateMessageHeader(szMsgHeader);

	//Copy the MSH Segment to the cHL7MessageBuffer
	lstrcpy(szHL7MessageBuffer,szMsgHeader);
	MessageBox(NULL,szHL7MessageBuffer,"Info",MB_OK);

	ConvertTheRest();
	MessageBox(NULL,szHL7MessageBuffer,"Info",MB_OK);
	return 0;
}