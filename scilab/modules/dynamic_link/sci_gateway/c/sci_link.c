
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

/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_dynamic_link.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "dl_genErrorMessage.h"
#include "freeArrayOfString.h"
/*-----------------------------------------------------------------------------------*/
static int linkNoRhs(void);
static int linkOneRhsShow(void);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_link)(char *fname,unsigned long fname_len)
{
	BOOL fflag = FALSE;
	int idsharedlibrary = -1;

	char *SharedLibraryName = NULL;

	char **subname = NULL;
	int sizesubname = 0;

	char *param3flag = NULL;

	int returnedID = -1;
	int ierr = 0;

	CheckRhs(0,3);
	CheckLhs(1,1);

	if (Rhs == 0)
	{
		return linkNoRhs();
	}
	else
	{
		if (Rhs >= 1)
		{
			if (VarType(1)== sci_matrix)
			{
				int m1 = 0, n1 = 0, l1 = 0;
				GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
				if ( (m1 == n1) && (n1 == 1) )
				{
					idsharedlibrary= (int)*stk(l1);
				}
				else
				{
					Scierror(999,_("%s : Wrong value for argument #%d: %s\n"),fname,1,_("Unique id of a shared library expected."));
					return 0;
				}
			}
			else if (VarType(1) == sci_strings)
			{
				char **strings = NULL;
				int m1 = 0, n1 = 0;
				GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&strings);

				if ( (m1 == 1) && (n1 == 1) )
				{
					SharedLibraryName =(char*)MALLOC(sizeof(char)*(strlen(strings[0])+1));
					strcpy(SharedLibraryName,strings[0]);
				}
				else
				{
					Scierror(999,_("%s : Wrong type for input argument #%d: %s\n"),fname,1,_("Unique dynamic library name expected."));
					return 0;
				}

				if ( (Rhs == 1) && (strcmp(SharedLibraryName,"show")==0) )
				{
					return linkOneRhsShow();
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input arguments: Strings expected.\n"),fname);
				return 0;
			}
		}

		if (Rhs >= 2)
		{
			if (VarType(2) == sci_strings)
			{
				int m2 = 0, n2 = 0;
				GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&subname);
				if (m2 == 1)
				{
					sizesubname = n2;
				}
				else
				{
					freeArrayOfString(subname,m2*n2);
					Scierror(999,_("%s: Wrong type for input argument. Strings vector expected.\n"),fname);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument. Strings expected.\n"),fname);
				return 0;
			}
		}

		if (Rhs == 3)
		{
			int m3 = 0,n3 = 0,l3 = 0;
			GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
			if ( ( strcmp(cstk(l3),"f") == 0 ) || ( strcmp(cstk(l3),"c") == 0 ) )
			{
				param3flag = (char*)MALLOC(sizeof(char)*( strlen( cstk(l3) )+1 ) );
				strcpy(param3flag,cstk(l3));
			}
			else
			{
				Scierror(999,_("%s Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,3,"f","c");
				return 0;
			}
		}
		else
		{
			param3flag = (char*)MALLOC(sizeof(char)*( strlen( "f" )+1 ) );
			strcpy(param3flag,"f");
		}

		if (strcmp("f",param3flag)==0) fflag = TRUE;
		else fflag = FALSE;

		returnedID = scilabLink(idsharedlibrary,SharedLibraryName,subname,sizesubname,fflag,&ierr);
		if (ierr == 0)
		{
			int n = 1 ,l = 0;
			CreateVar(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &n, &n,&l);
			*istk(l) = (int)returnedID;
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
		}
		else
		{
			dl_genErrorMessage(fname, ierr, SharedLibraryName);
		}

		if (SharedLibraryName) { FREE(SharedLibraryName); SharedLibraryName=NULL;}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int linkNoRhs(void)
{
	int retval = 0;
	static int l1 = 0,n1 = 0,m1 = 0;
	int sizeFunctionsList = 0;
	char ** FunctionsList = NULL;

	FunctionsList = getNamesOfFunctionsInSharedLibraries(&sizeFunctionsList);

	if ( (FunctionsList) && (sizeFunctionsList > 0) )
	{
		int i = 0;
		m1 = sizeFunctionsList;
		n1 = 1;
		CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &n1, &m1, FunctionsList);

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

		for (i=0;i < sizeFunctionsList;i++)
		{
			if (FunctionsList[i])
			{
				FREE(FunctionsList[i]);
				FunctionsList[i]=NULL;
			}
		}
		FREE(FunctionsList);
		FunctionsList = NULL;
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
	return retval;
}
/*-----------------------------------------------------------------------------------*/
static int linkOneRhsShow(void)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int *IdsList = NULL;
	int sizeIds = 0;

	ShowDynLinks();
	IdsList = getAllIdSharedLib(&sizeIds);

	if ( (sizeIds>0) && (IdsList) )
	{
		m1=1;
		n1=sizeIds;
		CreateVarFromPtr(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &IdsList);
		if (IdsList) {FREE(IdsList); IdsList=NULL;}
	}
	else
	{
		m1=0;
		n1=0;
		l1=0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
	}
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
