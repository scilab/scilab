/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <string.h>
#include <stdio.h>
#include <stdio.h> /* sprintf */
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "Scierror.h"
#include "commandwords.h"
#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static char **LocalFunctionsTab=NULL;
static int SizeLocalFunctionsTab=0;
/*--------------------------------------------------------------------------*/
static int CreateLocalFunctionsTab(void);
static int IsACommand(char *primitive);
static void DispInternalFunctions(void);
static void DispCommands(void);
static int cmpfunctionnames( const void *a ,const void *b);
/*--------------------------------------------------------------------------*/
extern char **GetFunctionsList(int *sizeList);
/*--------------------------------------------------------------------------*/
int C2F(sci_what)(char *fname,unsigned long fname_len)
{
	int sizecommandwords = 0;
	char **commandwords = getcommandkeywords(&sizecommandwords);

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

	CreateLocalFunctionsTab();
	qsort( LocalFunctionsTab , SizeLocalFunctionsTab ,  sizeof (char *)  , cmpfunctionnames);

	if (Lhs == 1)
	{
		DispInternalFunctions();
		DispCommands();
		LhsVar(1)=0;
	}
	else /* Lhs == 2 */
	{
		int ncol=1;	
		int nrowFunctions=SizeLocalFunctionsTab;
		int nrowCommands=sizecommandwords;

		CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &nrowFunctions, &ncol, LocalFunctionsTab);
		LhsVar(1)=Rhs+1;

		CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE, &nrowCommands, &ncol, commandwords);
		LhsVar(2)=Rhs+2;

		freeArrayOfString(LocalFunctionsTab, nrowFunctions);
		freeArrayOfString(commandwords, nrowCommands);

	}

	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static void DispInternalFunctions(void)
{
	int i=0;

	sciprint("\n");
	sciprint(_("Internal Functions:\n"));
	sciprint("\n");
	for (i=0;i<SizeLocalFunctionsTab;i++)
	{
		sciprint("%+24s ",LocalFunctionsTab[i]);
		if ((i+1)%4==0) sciprint("\n");
	}
	sciprint("\n");
}
/*--------------------------------------------------------------------------*/
static void DispCommands(void)
{
	int i=0;
	int sizecommandwords = 0;
	char **commandwords = getcommandkeywords(&sizecommandwords);

	sciprint("\n");
	sciprint(_("Commands:\n"));
	sciprint("\n");
	for (i=0;i <sizecommandwords;i++)
	{
		sciprint("%+24s ",commandwords[i]);
		if ((i+1)%4==0) sciprint("\n");
	}
	sciprint("\n");

	freeArrayOfString(commandwords, sizecommandwords);
}
/*--------------------------------------------------------------------------*/
static int IsACommand(char *primitive)
{
	int sizecommandwords = 0;
	char **commandwords = getcommandkeywords(&sizecommandwords);
	int bOK=FALSE;
	int i=0;

	for (i=0;i<sizecommandwords ;i++)
	{
		if (strcmp(commandwords[i],primitive)==0)
		{
			freeArrayOfString(commandwords, sizecommandwords);
			return TRUE;
		}
	}

	freeArrayOfString(commandwords, sizecommandwords);

	return bOK;
}
/*--------------------------------------------------------------------------*/
static int CreateLocalFunctionsTab(void)
{
	char **LocalFunctionsTabTmp=NULL;
	int i=0;
	int j=0;
	int SizeTab=0;
	int MaxSizeWithoutCommands=0;

	LocalFunctionsTabTmp=GetFunctionsList(&SizeTab);

	if (LocalFunctionsTabTmp)
	{
		for (i=0;i<SizeTab;i++)
		{
			if ( !IsACommand(LocalFunctionsTabTmp[i]) ) MaxSizeWithoutCommands++;
		}

		LocalFunctionsTab=(char **)MALLOC(sizeof(char**)*MaxSizeWithoutCommands);

		if (LocalFunctionsTab == NULL) 
		{
			SizeLocalFunctionsTab = 0;
			return FALSE;
		}

		j=0;
		for (i=0;i<SizeTab;i++)
		{
			if ( !IsACommand(LocalFunctionsTabTmp[i]) ) 
			{
				LocalFunctionsTab[j] = strdup(LocalFunctionsTabTmp[i]);
				j++;
			}
		}

		freeArrayOfString(LocalFunctionsTabTmp, SizeTab);
	}
	else
	{
		SizeLocalFunctionsTab = 0;
		return FALSE;
	}

	SizeLocalFunctionsTab=MaxSizeWithoutCommands;

	return TRUE;
}
/*--------------------------------------------------------------------------*/
static int cmpfunctionnames( const void *a ,const void *b)
{
	return strcmp(*(const char **)a, *(const char **)b );
}
/*--------------------------------------------------------------------------*/
