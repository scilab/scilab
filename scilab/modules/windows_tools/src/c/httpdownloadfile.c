/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#pragma comment(lib,"Wininet.lib")
/*--------------------------------------------------------------------------*/
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501
#include <windows.h>
#include <wininet.h>
#include "MALLOC.h"
#include "httpdownloadfile.h"
/* http://msdn2.microsoft.com/en-us/library/aa385098.aspx */
/*--------------------------------------------------------------------------*/
#define MO 0x100000 /* Read 1 Mo by 1Mo. */
/*--------------------------------------------------------------------------*/
BOOL httpDownloadFile(char * szURL,char * szSaveFilePath)
{
	HINTERNET hiConnex;
	/* * / * : /*rfc 2616 protocole http.  all files type accepted*/
	char szHeader[]="Accept: */*\r\n\r\n"; 
	HINTERNET hiDownload;
	HANDLE haFile;
	char * szBuff;
	DWORD dwBytesRequired;
	DWORD dwSizeOfByReq = 4;
	DWORD dwBytesRead;
	DWORD dwBytesWritten;

	hiConnex = InternetOpen("Scilab_Downnload",INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	if(hiConnex == NULL)return FALSE;


	if(!(hiDownload = InternetOpenUrl(hiConnex,szURL,szHeader,lstrlen(szHeader),INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE,0))){
		InternetCloseHandle(hiConnex);
		return FALSE;
	}

	haFile = CreateFile(szSaveFilePath,GENERIC_WRITE,FILE_SHARE_WRITE,0,CREATE_ALWAYS,0,0);
	if(haFile == INVALID_HANDLE_VALUE)
	{
		InternetCloseHandle(hiConnex);
		return FALSE;
	}


	/* Get file size */
	HttpQueryInfo(hiDownload,HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,(LPVOID)&dwBytesRequired,&dwSizeOfByReq,0);

	if(dwBytesRequired > MO)
	{
		szBuff = (char*)MALLOC(MO);
		if(szBuff == NULL)
		{
			CloseHandle(haFile);
			InternetCloseHandle(hiConnex);
			return FALSE;
		}
	}
	else
	{
		szBuff = (char*)MALLOC(dwBytesRequired);
		if(szBuff == NULL)
		{
			CloseHandle(haFile);
			InternetCloseHandle(hiConnex);
			return FALSE;
		}
	}

	while(dwBytesRequired > 0)
	{
		/* we read 1Mo from file. */
		if(dwBytesRequired >= MO)
		{
			if(!InternetReadFile(hiDownload,szBuff,MO,&dwBytesRead) || dwBytesRead != MO)
			{
				CloseHandle(haFile);
				InternetCloseHandle(hiConnex);
				FREE(szBuff);
				return FALSE;
			}
			dwBytesRequired -= MO;


			/* we write buffer */
			if(!WriteFile(haFile,szBuff,MO,&dwBytesWritten,NULL) || dwBytesWritten != MO)
			{
				CloseHandle(haFile);
				InternetCloseHandle(hiConnex);
				FREE(szBuff);
				return FALSE;
			}
		}
		else
		{
			if(!InternetReadFile(hiDownload,szBuff,dwBytesRequired,&dwBytesRead) || dwBytesRead != dwBytesRequired)
			{
				CloseHandle(haFile);
				InternetCloseHandle(hiConnex);
				FREE(szBuff);
				return FALSE;
			}

			/* we write buffer in a backup file*/
			if(!WriteFile(haFile,szBuff,dwBytesRequired,&dwBytesWritten,NULL) || dwBytesWritten != dwBytesRequired)
			{
				CloseHandle(haFile);
				InternetCloseHandle(hiConnex);
				FREE(szBuff);
				return FALSE;
			}

			dwBytesRequired = 0;
		}
	}

	InternetCloseHandle(hiConnex);
	CloseHandle(haFile);
	FREE(szBuff);
	return TRUE;

}
/*--------------------------------------------------------------------------*/
