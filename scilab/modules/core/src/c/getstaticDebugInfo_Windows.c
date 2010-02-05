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

/* !!! PLEASE DO NOT TRANSLATE STRINGS IN THIS FILE SEE BUG 5505 !!! */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <xmlwin32version.h>
#include <tcl.h>
#include <tk.h>
#include "MALLOC.h"
#include "getstaticDebugInfo_Windows.h"
#include "localization.h"
#include "version.h"
#include "withtk.h"
#include "..\..\..\..\libs\pcre\pcre.h"
/*--------------------------------------------------------------------------*/
extern BOOL BuildWithVS8ExpressF2C(void);
static char ** appendStringStaticDebugInfo(char **listInfo,int *sizeListInfo,char *str);
/*--------------------------------------------------------------------------*/
char **getStaticDebugInfo_Windows(int *sizeArray)
{
	#define BUFFER_LEN 255
	int nb_info = 0;
	char **outputDynamicList = NULL;
	char *str_info = NULL;

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"Version: %s",SCI_VERSION_STRING);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"Compilation date: %s",__DATE__);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"Compilation time: %s",__TIME__);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		#ifdef _WIN64
		sprintf(str_info,"Compiler Architecture: %s","X64");
		#else
		sprintf(str_info,"Compiler Architecture: %s","x86");
		#endif
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{

	#ifdef __INTEL__
		sprintf(str_info,"Compiled with %s","Intel compiler");
	#elif defined _MSC_VER
		sprintf(str_info,"Compiled with %s","Microsoft compiler");
	#else	
		sprintf(str_info,"Compiled with %s","unknown compiler");
	#endif
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	if (BuildWithVS8ExpressF2C())
	{
		str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
		if (str_info)
		{
			sprintf(str_info,"Compiled with %s","F2C");
			outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
		}
	}
	
	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"XML version: %s",LIBXML_DOTTED_VERSION);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	if ( withtk() )
	{
		str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
		if (str_info)
		{
			sprintf(str_info,"Tcl/Tk: %s","Enable");
			outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
		}

		str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
		if (str_info)
		{
			sprintf(str_info,"TCL version: %s",TCL_PATCH_LEVEL);
			outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
		}

		str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
		if (str_info)
		{
			sprintf(str_info,"TK version: %s",TK_PATCH_LEVEL);
			outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
		}
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"Path separator: %s",PATH_SEPARATOR);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"Directory separator: %s",DIR_SEPARATOR);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	str_info = (char*)MALLOC(sizeof(char)*BUFFER_LEN);
	if (str_info)
	{
		sprintf(str_info,"PCRE Version: %d.%d", PCRE_MAJOR,PCRE_MINOR);
		outputDynamicList = appendStringStaticDebugInfo(outputDynamicList,&nb_info,str_info);
	}

	*sizeArray = nb_info;
	return outputDynamicList;
}
/*--------------------------------------------------------------------------*/
static char ** appendStringStaticDebugInfo(char **listInfo,int *sizeListInfo,char *str)
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

	if (returnListInfo)
	{
		returnListInfo[(*sizeListInfo) - 1] = str;
	}
	return returnListInfo;
}
/*--------------------------------------------------------------------------*/
