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

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "PATH_MAX.h"
#include "gw_io.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "stack-c.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static int GetIndexLastModifiedFileInList(char **ListFilename,int numberelemnts);
/*--------------------------------------------------------------------------*/
int sci_newest(char *fname,unsigned long fname_len)
{
	CheckLhs(1,1);
	if (Rhs == 0)
	{
		/* newest() returns [] */
		int m1 = 0,n1 = 0,l1 = 0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		int m1 = 0,n1 = 0,l1 = 0;
		int RetIndex = 1;

		if (Rhs == 1)
		{
			if (GetType(1) == sci_matrix)
			{
				GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				if ( (m1 == 0) && (n1 == 0) ) /* newest([]) returns [] */
				{
					m1 = 0;n1 = 0;l1 = 0;
					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
					return 0;
				}
				else
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname);
					return 0;
				}
			}
			else
			{
				if (GetType(1) == sci_strings)
				{
					char **Str = NULL;

					GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
					RetIndex = GetIndexLastModifiedFileInList(Str,m1*n1);
					freeArrayOfString(Str,m1*n1);
				}
				else
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname);
					return 0;
				}
			}
		}
		else /* Rhs > 1 */
		{
			int i = 1;
			char **Str = NULL;
			int RhsBackup = Rhs;

			/* check that all input arguments are strings */
			for (i = 1; i <= Rhs ; i++)
			{
				if (GetType(i) != sci_strings)
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,i);
					return 0;
				}
			}

			Str = (char**)MALLOC(sizeof(char*) * RhsBackup);
			if (Str)
			{
				for (i = 1; i <= RhsBackup; i++)
				{
					GetRhsVar(i,STRING_DATATYPE,&m1,&n1,&l1);
					Str[i-1] = strdup(cstk(l1));
				}

				RetIndex = GetIndexLastModifiedFileInList(Str,RhsBackup);
				freeArrayOfString(Str,RhsBackup);
			}
		}

		/* Output on scilab's stack */
		if (RetIndex >= 1)
		{
			int *paramoutINT = (int*)MALLOC(sizeof(int));
			*paramoutINT = RetIndex;

			n1 = 1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();

			if (paramoutINT){FREE(paramoutINT); paramoutINT=NULL;}
		}
		else
		{
			m1=0;
			n1=0;
			l1=0;

			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int GetIndexLastModifiedFileInList(char **ListFilename,int numberelemnts)
{
#ifdef _MSC_VER
	struct _stat buf;
#else
	struct stat buf;
#endif

	int i=0;
	int RetVal=0;

	int RetIndex=1;
	long int MaxTime=0;

	for (i = 0; i<numberelemnts ;i++)
	{

		int resultstat = 0;
		char *FileName = NULL;

		FileName = expandPathVariable(ListFilename[i]);

#ifdef _MSC_VER
		if (FileName)
		{
			if ( (FileName[strlen(FileName)-1]=='/') || (FileName[strlen(FileName)-1]=='\\') )
			{
				FileName[strlen(FileName)-1]='\0';
			}

		}

		{
		wchar_t *pszFileName = to_wide_string(FileName);
		resultstat = _wstat(pszFileName, &buf );
		FREE(pszFileName);
		}
#else
		resultstat = stat(FileName, &buf );
#endif
		if (resultstat == 0)
		{
			if ((long int)buf.st_mtime>MaxTime)
			{
				MaxTime=(long int)buf.st_mtime;
				RetIndex=i+1;
			}
		}

		FREE(FileName);
		FileName = NULL;
	}

	RetVal=RetIndex;
	return RetVal;
}
/*--------------------------------------------------------------------------*/
