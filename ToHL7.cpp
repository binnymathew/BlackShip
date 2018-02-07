
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
	if(!lstrcmp(szTemp, "A01"))
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

			
			if(lstrcmp(lpTempSegment->szData, CODED_VALUES) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData, TIME_STAMP) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData, STRING) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData,NUMERIC) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData, DATE) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData, ADDRESS) == 0)
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

			else if(lstrcmp(lpTempSegment->szData, TEXT_DATA) == 0)
				lstrcat(szHL7,szOneWord);

			else if(lstrcmp(lpTempSegment->szData, COMPOSITE_ID) == 0)
				lstrcat(szHL7,szOneWord);
			
			else if(lstrcmp(lpTempSegment->szData, TELEPHONE_NUMBER) == 0)
				lstrcat(szHL7,szOneWord);
			
			szHL7[lstrlen(szHL7)] = FIELD_SEPARATOR;
			lpTempSegment = lpTempSegment->lpNext;
		}
		lstrcat(szHL7MessageBuffer,szHL7);
		lpTempTrigger = lpTempTrigger->lpNext;
	}
}



