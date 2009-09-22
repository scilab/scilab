/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack-def.h"
#include "MALLOC.h"
#include "getDynamicDebugInfo_Windows.h"
#include "localization.h"
#include "getos.h"
#include "../../../../libs/GetWindowsVersion/GetWindowsVersion.h"
/*--------------------------------------------------------------------------*/
static char * GetRegKeyCPUIdentifier(void);
static char * GetRegKeyVideoCard(void);
static char * GetRegKeyVideoCardVersion(void);
static char * GetScreenResolution(void);
static char * GetNumberMonitors(void);
static char ** appendStringDebugInfo(char **listInfo,int *sizeListInfo,char *str);
/*--------------------------------------------------------------------------*/
char **getDynamicDebugInfo_Windows(int *sizeArray)
{
#define DIV 1024
#define WIDTH 7

#define BUFFER_LEN 255
	int nb_info = 0;
	char *str_info = NULL;
	char **outputDynamicList=NULL;
	char *fromGetenv = NULL;


	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	
	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Memory in use: %*ld %%"),
			WIDTH,
			statex.dwMemoryLoad);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Total Physical Memory (Kbytes): %*I64d"),
			WIDTH,
			statex.ullTotalPhys/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Free Physical Memory (Kbytes): %*I64d"),
			WIDTH,
			statex.ullAvailPhys/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Total Paging File (Kbytes): %*I64d"),
			WIDTH,
			statex.ullTotalPageFile/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Free Paging File (Kbytes): %*I64d"),
			WIDTH,
			statex.ullAvailPageFile/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Total Virtual Memory (Kbytes): %*I64d"),
			WIDTH,
			statex.ullTotalVirtual/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Free Virtual Memory (Kbytes): %*I64d"),
			WIDTH,
			statex.ullAvailVirtual/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,
			_("Free Extended Memory (Kbytes): %*I64d"),
			WIDTH,
			statex.ullAvailExtendedVirtual/DIV);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		char *OS = getOSFullName();
		char *OSRelease = getOSRelease();
		strcpy(str_info ,_("Operating System: "));

		if (OS && OSRelease)
		{
			strcat(str_info , OS);
			strcat(str_info , " ");
			strcat(str_info , OSRelease);
		}
		else
		{
			strcat(str_info, "ERROR");
		}

		if (OS) {FREE(OS); OS = NULL;}
		if (OSRelease) {FREE(OSRelease); OSRelease = NULL;}

		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,GetRegKeyCPUIdentifier());

	str_info = (char*)MALLOC( sizeof(char)*BUFFER_LEN );
	if (str_info)
	{
		SYSTEM_INFO siSysInfo;
		GetSystemInfo(&siSysInfo); 
		sprintf(str_info,_("Number of processors: %d"),  siSysInfo.dwNumberOfProcessors);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}


	str_info = (char*)MALLOC( sizeof(char)*BUFFER_LEN );
	if (str_info)
	{
		sprintf(str_info,_("Video card: %s"),  GetRegKeyVideoCard());
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC( sizeof(char)*BUFFER_LEN );
	if (str_info)
	{
		sprintf(str_info,_("Video card driver version: %s"),  GetRegKeyVideoCardVersion());
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,GetScreenResolution());
	outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,GetNumberMonitors());
	
	#define PATH_var "Path"
	fromGetenv = getenv(PATH_var);

	if (fromGetenv)
	{
		str_info = (char*)MALLOC( sizeof(char)*(strlen(fromGetenv) + strlen("%s : %s") + strlen(PATH_var) +1) );
		sprintf(str_info,"%s: %s", PATH_var, fromGetenv);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}
		
	#define COMSPEC_var "ComSpec"
	fromGetenv = getenv(COMSPEC_var);
	if (fromGetenv)
	{
		str_info = (char*)MALLOC( sizeof(char)*(strlen(fromGetenv) + strlen("%s : %s") + strlen(COMSPEC_var) + 1) );
		sprintf(str_info,"%s: %s", COMSPEC_var,fromGetenv);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}
	
	#define TMP_var "TMP"
	fromGetenv = getenv(TMP_var);
	if (fromGetenv)
	{
		str_info = (char*)MALLOC( sizeof(char)*(strlen(fromGetenv) + strlen("%s : %s") + strlen(TMP_var) + 1) );
		sprintf(str_info,"%s : %s", TMP_var,fromGetenv);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	#define TEMP_var "TEMP"
	fromGetenv = getenv(TEMP_var);
	if (fromGetenv)
	{
		str_info = (char*)MALLOC( sizeof(char)*(strlen(fromGetenv) + strlen("%s : %s") + strlen(TEMP_var) + 1) );
		sprintf(str_info,"%s: %s", TEMP_var,fromGetenv);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	#define SCIHOME_var "SCIHOME"
	fromGetenv = getenv(SCIHOME_var);
	if (fromGetenv)
	{
		str_info = (char*)MALLOC( sizeof(char)*(strlen(fromGetenv) + strlen("%s : %s") + strlen(SCIHOME_var) + 1) );
		sprintf(str_info,"%s: %s", SCIHOME_var,fromGetenv);
		outputDynamicList = appendStringDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	*sizeArray = nb_info;
	return outputDynamicList;
}
/*--------------------------------------------------------------------------*/
static char * GetRegKeyCPUIdentifier(void)
{
#define KeyCpuIdentifer "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define LenLine 255

	HKEY key;
	DWORD result;
	char *LineIdentifier;
	ULONG length = LenLine,Type;

	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyCpuIdentifer, 0, KEY_QUERY_VALUE , &key);

	LineIdentifier=(char*)MALLOC(sizeof(char)*length);

	if ( RegQueryValueEx(key, "ProcessorNameString", 0, &Type, (LPBYTE)LineIdentifier, &length) !=  ERROR_SUCCESS )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if( Type != REG_SZ )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	return (char *)LineIdentifier;
}
/*--------------------------------------------------------------------------*/
static char * GetScreenResolution(void)
{
	HDC hdc=GetDC(NULL);
	int BitsByPixel = GetDeviceCaps(hdc, BITSPIXEL);
	int ResX = GetSystemMetrics(SM_CXSCREEN);
	int ResY = GetSystemMetrics(SM_CYSCREEN) ;
	char *Resolution = NULL;
	
	ReleaseDC (NULL, hdc);

	Resolution = (char*)MALLOC( sizeof(char)*( strlen(_("Screen size: %d x %d %d bits")) + 32));
	if (Resolution)
	{
		sprintf(Resolution,_("Screen size: %d x %d %d bits"),ResX ,ResY,BitsByPixel);
	}
	
	return Resolution;
}
/*--------------------------------------------------------------------------*/
static char ** appendStringDebugInfo(char **listInfo,int *sizeListInfo,char *str)
{
	char ** returnListInfo = NULL;
	if (listInfo)
	{
		(*sizeListInfo)++;
		returnListInfo = (char**)REALLOC(listInfo,sizeof(char*)*(*sizeListInfo));
	}
	else
	{
		(*sizeListInfo) = 1;
		returnListInfo = (char**)MALLOC(sizeof(char*)*(*sizeListInfo));
	}
	returnListInfo[(*sizeListInfo) - 1] = str;

	return returnListInfo;
}
/*--------------------------------------------------------------------------*/
static char * GetNumberMonitors(void)
{
	#define NBMONITORS "Number of Monitors: %d"
	char *returnedStr = NULL;
	int nbMonitors = GetSystemMetrics(SM_CMONITORS) ;

	returnedStr = (char*)MALLOC( sizeof(char)*( strlen(_(NBMONITORS)) +  32));
	if (returnedStr)
	{
		sprintf(returnedStr,_(NBMONITORS),nbMonitors);
	}

	return returnedStr;
}
/*--------------------------------------------------------------------------*/
static char * GetRegKeyVideoCard(void)
{
	#define KeyDisplayIdentifer "SYSTEM\\ControlSet001\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000"
	#define LenLine 255

	HKEY key;
	DWORD result;
	char *LineIdentifier;
	ULONG length = LenLine,Type;

	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyDisplayIdentifer, 0, KEY_QUERY_VALUE , &key);

	LineIdentifier=(char*)MALLOC(sizeof(char)*length);

	if ( RegQueryValueEx(key, "DriverDesc", 0, &Type, (LPBYTE)LineIdentifier, &length) !=  ERROR_SUCCESS )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if( Type != REG_SZ )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	return (char *)LineIdentifier;
}
/*--------------------------------------------------------------------------*/
static char * GetRegKeyVideoCardVersion(void)
{
#define KeyDisplayIdentifer "SYSTEM\\ControlSet001\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000"
#define LenLine 255

	HKEY key;
	DWORD result;
	char *LineIdentifier;
	ULONG length = LenLine,Type;

	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyDisplayIdentifer, 0, KEY_QUERY_VALUE , &key);

	LineIdentifier=(char*)MALLOC(sizeof(char)*length);

	if ( RegQueryValueEx(key, "DriverVersion", 0, &Type, (LPBYTE)LineIdentifier, &length) !=  ERROR_SUCCESS )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if( Type != REG_SZ )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	return (char *)LineIdentifier;
}
/*--------------------------------------------------------------------------*/
