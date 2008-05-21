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

/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "MALLOC.h" 
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
#pragma comment(lib, "winmm.lib")
/*--------------------------------------------------------------------------*/
int sci_mcisendstring(char *fname,unsigned long l)
{
	int m1,n1,l1;

	char *Output=NULL;
	int *BoolOutput=NULL;
	int *CodeOutput=NULL;

	CheckRhs(1,1);
    CheckLhs(1,3);

	if (GetType(1) == sci_strings)	
	{
		MCIERROR Error;
		char *MCICommand=NULL;
		char ReturnString[2048];

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		MCICommand=cstk(l1);

		Error=mciSendString(MCICommand,(LPSTR)ReturnString,sizeof(ReturnString),NULL);

		BoolOutput=(int*)MALLOC(sizeof(int));
		CodeOutput=(int*)MALLOC(sizeof(int));

		*BoolOutput=(int)0;
		*CodeOutput=(int)Error;
		if (Error)
		{
			char ErrorText[128];
			*BoolOutput=(int)FALSE;
			
			if ( mciGetErrorString(Error,ErrorText,sizeof(ErrorText)) == FALSE )
			{
				wsprintf(ErrorText,"%s","Unknown MCI error");
			}

			Output = strdup(ErrorText);
		}
		else
		{
			*BoolOutput=(int)TRUE;
			Output = strdup("OK");
		}

		n1=1;

		if ( Lhs == 1 )
		{
			CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &BoolOutput);
			LhsVar(1)=Rhs+1;
		}
		else
		if ( Lhs == 2 )
		{
			CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &BoolOutput);
			LhsVar(1)=Rhs+1;

			CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &CodeOutput);
			LhsVar(2)=Rhs+2;
		}
		else /* Lhs == 3 */
		{
			CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &BoolOutput);
			LhsVar(1)=Rhs+1;

			CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &CodeOutput);
			LhsVar(2)=Rhs+2;

			CreateVarFromPtr(Rhs+ 3,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
			LhsVar(3)=Rhs+3;
		}
		
		C2F(putlhsvar)();

		if (Output) {FREE(Output);Output=NULL;}
		if (BoolOutput) {FREE(BoolOutput);BoolOutput=NULL;}
		if (CodeOutput) {FREE(CodeOutput);Output=NULL;}

		return 0;
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
		return 0;
	}
}
/*--------------------------------------------------------------------------*/
