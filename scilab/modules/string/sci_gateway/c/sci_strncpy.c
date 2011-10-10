/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*----------------------------------------------------------------------------*/
int sci_strncpy(char *fname,unsigned long fname_len)
{
	CheckRhs(2,2);
	CheckLhs(0,1);
	
	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_matrix) )
	{
		int m1 = 0; int n1 = 0;
		char **InputString_Parameter1 = NULL;
		int m1n1 = 0; /* m1 * n1 */
		
		int m2 = 0; int n2 = 0 ; int l2 = 0;
		double *InputLength_Parameter2 = NULL;
		int *InputLength_Parameter2_checked = NULL;
		int m2n2 = 0; /* m2 * n2 */
		
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&InputString_Parameter1);
		m1n1 = m1 * n1;
		
		GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
		InputLength_Parameter2 = stk(l2);
		m2n2 = m2 * n2;
		
		if (m2n2 == 0)
		{
			freeArrayOfString(InputString_Parameter1,m1n1);
			Scierror(999,_("%s: Wrong size for input argument #%d: Non-empty matrix expected.\n"),fname,2);
			return 0;
		}
		else
		{
			int j = 0;
			InputLength_Parameter2_checked = (int*)MALLOC(sizeof(int)*m2n2);
			if (InputLength_Parameter2_checked == NULL)
			{
				freeArrayOfString(InputString_Parameter1,m1n1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
			
			if ( ((m1 == m2) && (n1 == n2)) || (m2n2 == 1) )
			{
				for(j = 0; j < m2n2;j++)
				{
					int len = 0;
					wchar_t *wcstring = to_wide_string(InputString_Parameter1[j]);
					if (wcstring)
					{
						len = (int)wcslen(wcstring);
						FREE(wcstring);
						wcstring = NULL;
					}
					if ( len < InputLength_Parameter2[j] ) InputLength_Parameter2_checked[j] = len;
					else
					{
						if ( InputLength_Parameter2[j] < 0 ) InputLength_Parameter2_checked[j] = 0;
						else InputLength_Parameter2_checked[j] = (int)InputLength_Parameter2[j];
					}
				}
			}
		}
		
		if ( ((m1 == m2) && (n1 == n2)) || (m2n2 == 1) )
		{
			char **OutputStrings = NULL;
			int i = 0;
			
			OutputStrings = (char**)MALLOC(sizeof(char*)*m1n1);
			if (OutputStrings == NULL)
			{
				if (InputLength_Parameter2_checked) 
				{
					FREE(InputLength_Parameter2_checked);
					InputLength_Parameter2_checked = NULL;
				}
				freeArrayOfString(InputString_Parameter1,m1n1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
			
			for (i=0; i < m1n1 ; i++)
			{
				wchar_t *wcInput = NULL;
				wchar_t *wcOutput = NULL;

				int j = 0;
				
				if (m2n2 == 1) 
				{
					j = 0; /* Input parameter two is dimension one */
				}
				else 
				{
					j = i; /* Input parameter One & two have same dimension */
				}

				wcInput = to_wide_string(InputString_Parameter1[i]);
				wcOutput = (wchar_t*)MALLOC(sizeof(wchar_t)*(InputLength_Parameter2_checked[j]+1));
				if (wcInput && wcOutput)
				{
					wcsncpy(wcOutput,wcInput,InputLength_Parameter2_checked[j]);
					wcOutput[InputLength_Parameter2_checked[j]] = L'\0';

					OutputStrings[i] = wide_string_to_UTF8(wcOutput);
					FREE(wcOutput); wcOutput = NULL;
					FREE(wcInput); wcInput = NULL;
				}
				else
				{
					if (InputLength_Parameter2_checked) 
					{
						FREE(InputLength_Parameter2_checked);
						InputLength_Parameter2_checked = NULL;
					}
					freeArrayOfString(InputString_Parameter1,m1n1);
					freeArrayOfString(OutputStrings,i);
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				}
			}
			
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,OutputStrings);
			freeArrayOfString(OutputStrings,m1n1);
			if (InputLength_Parameter2_checked) 
			{
				FREE(InputLength_Parameter2_checked);
				InputLength_Parameter2_checked = NULL;
			}
			
			LhsVar(1) = Rhs+1 ;
			PutLhsVar();
		}
		else
		{
			freeArrayOfString(InputString_Parameter1,m1n1);
			Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,2);
			return 0;
		}
		
		freeArrayOfString(InputString_Parameter1,m1n1);
	}
	else
	{
		if(GetType(1) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"),fname,2);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
