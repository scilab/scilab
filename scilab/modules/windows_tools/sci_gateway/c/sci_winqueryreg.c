/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "registry.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_winqueryreg(char *fname,unsigned long l)
{
	static int l1,n1,m1;

	char *param1 = NULL,*param2 = NULL,*param3 = NULL;
	char *output = NULL ;
	int *paramoutINT = NULL;
	BOOL OuputIsREG_SZ = FALSE;
	BOOL TestWinQuery = FALSE;

	Rhs=Max(0,Rhs);
	CheckRhs(2,3);
	CheckLhs(0,1);

	if (Rhs == 3)
	{
		if ( (GetType(1) != sci_strings) || (GetType(2) != sci_strings) || (GetType(3) != sci_strings))
		{
			Scierror(999,_("%s: Wrong type for input arguments: String expected.\n"),fname);
			return 0;
		}
	}
	else /* Rhs == 2 */
	{
		if ( (GetType(1) != sci_strings) || (GetType(2) != sci_strings) )
		{
			Scierror(999,_("%s: Wrong type for input arguments: String expected.\n"),fname);
			return 0;
		}
	}

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	param1=cstk(l1);

	GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
	param2=cstk(l1);

	if ( Rhs == 3 )
	{
		GetRhsVar(3,STRING_DATATYPE,&m1,&n1,&l1);
		param3=cstk(l1);

		if (strcmp(param1,"name") == 0)
		{
			int NumbersElm=0;

			WindowsQueryRegistryNumberOfElementsInList(param2,param3,&NumbersElm);
			if (NumbersElm)
			{
				static char *ListKeysName[255];

				if (NumbersElm > 255) NumbersElm=255;
				if ( WindowsQueryRegistryList(param2,param3,NumbersElm,ListKeysName) )
				{
					int i=0;

					CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &NumbersElm, &n1, &ListKeysName);

					freeArrayOfString(ListKeysName, NumbersElm);

					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
					return 0;
				}
				else
				{
					Scierror(999,_("%s: Cannot open Windows registry.\n"),fname);
					return 0;
				}
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
			return 0;
		}
	}

	output=(char*)MALLOC(PATH_MAX*sizeof(char));
	paramoutINT=(int*)MALLOC(sizeof(int));

	if ( Rhs == 3 )
	{
		TestWinQuery=WindowsQueryRegistry(param1,param2,param3,output,paramoutINT,&OuputIsREG_SZ);
	}
	else
	{
		TestWinQuery=WindowsQueryRegistry(param1,param2,NULL,output,paramoutINT,&OuputIsREG_SZ);
	}

	if ( TestWinQuery )
	{
		n1=1;
		if ( OuputIsREG_SZ )
		{
			CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
		}
		else
		{
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
		}

		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		if (output) {FREE(output);output=NULL;}
		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
		Scierror(999,_("%s: Cannot query value of this type.\n"),fname);
		return 0;
	}

	if (output) {FREE(output);output=NULL;}
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
