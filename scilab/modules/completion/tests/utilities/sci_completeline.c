/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "stack-c.h"
#include "completeLine.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int sci_completeline(char *fname,unsigned long fname_len)
{
	CheckRhs(5,5);
	CheckLhs(1,1);
	
	if ( (GetType(1) == sci_strings) &&	
			 (GetType(2) == sci_strings) &&	
			 (GetType(3) == sci_strings) &&	
			 (GetType(4) == sci_strings) &&	
			 (GetType(5) == sci_boolean) )
		{
			char *currentline = NULL;
			char *stringToAdd = NULL;
			char *filePattern = NULL;
			char *defaultPattern = NULL;
			BOOL stringToAddIsPath = FALSE;
			int l5 = 0;
			int m = 0, n = 0;
			
			char **InputString_Parameter1 = NULL;
			char **InputString_Parameter2 = NULL;
			char **InputString_Parameter3 = NULL;
			char **InputString_Parameter4 = NULL;
			
			char *result = NULL;
			
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m,&n,&InputString_Parameter1);
			if ( (m == 1) && (n == 1) )
			{
				currentline = InputString_Parameter1[0];
			}
			else
			{
				freeArrayOfString(InputString_Parameter1, m*n);
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
				return 0;
			}
			
			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m,&n,&InputString_Parameter2);
			if ( (m == 1) && (n == 1) )
			{
				stringToAdd = InputString_Parameter2[0];
			}
			else
			{
				freeArrayOfString(InputString_Parameter1, 1);
				freeArrayOfString(InputString_Parameter2, m*n);
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
				return 0;
			}
			
			GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m,&n,&InputString_Parameter3);
			if ( (m == 1) && (n == 1) )
			{
				filePattern = InputString_Parameter3[0];
			}
			else
			{
				freeArrayOfString(InputString_Parameter1, 1);
				freeArrayOfString(InputString_Parameter2, 1);
				freeArrayOfString(InputString_Parameter3, m*n);
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,3);
				return 0;
			}
			
			GetRhsVar(4,MATRIX_OF_STRING_DATATYPE,&m,&n,&InputString_Parameter4);
			if ( (m == 1) && (n == 1) )
			{
				defaultPattern = InputString_Parameter4[0];
			}
			else
			{
				freeArrayOfString(InputString_Parameter1, 1);
				freeArrayOfString(InputString_Parameter2, 1);
				freeArrayOfString(InputString_Parameter3, 1);
				freeArrayOfString(InputString_Parameter4, m*n);
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,4);
				return 0;
			}
			
			GetRhsVar(5,MATRIX_OF_BOOLEAN_DATATYPE,&m,&n,&l5);
			if ( (m == 1) && (n == 1) )
			{
				stringToAddIsPath = (BOOL) *istk(l5);
			}
			else
			{
				freeArrayOfString(InputString_Parameter1, 1);
				freeArrayOfString(InputString_Parameter2, 1);
				freeArrayOfString(InputString_Parameter3, 1);
				freeArrayOfString(InputString_Parameter4, 1);
				Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"),fname,4);
				return 0;
			}
			
			result = completeLine(currentline, stringToAdd, filePattern, defaultPattern, stringToAddIsPath);
			
			if (result == NULL)
			{
					int l = 0;
					m = 0, n = 0;
					CreateVar(Rhs+1,STRING_DATATYPE,  &m, &n, &l);
			}
			else
			{
				n = 1;
				CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m=(int)strlen(result), &m),&n, &result);
				if (result) {FREE(result);result = NULL;}
			}
			LhsVar(1) = Rhs+1;
			
			freeArrayOfString(InputString_Parameter1, 1);
			freeArrayOfString(InputString_Parameter2, 1);
			freeArrayOfString(InputString_Parameter3, 1);
			freeArrayOfString(InputString_Parameter4, 1);
			
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input arguments.\n"),fname);
		}
	return 0;
}
/*--------------------------------------------------------------------------*/
