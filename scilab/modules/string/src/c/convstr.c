
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
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
#include <string.h>
#include <stdio.h>
#include "convstr.h"
#include "MALLOC.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
char **convstr(char **Input_Matrix, char typ, int mn)
{
	char **Output_Matrix = (char**)MALLOC(sizeof(char*)*mn);
	if (Output_Matrix)
	{
		int x = 0;
		for (x =0 ; x < mn; x++)
		{
			wchar_t *wcInput = to_wide_string(Input_Matrix[x]);
			if (wcInput)
			{
				wchar_t *wcOutput = to_wide_string(Input_Matrix[x]);
				int y = 0;
				for (y = 0; y < (int)wcslen(wcInput); y++)
				{
					/*To traverse every string in the string matrix */
					if ( (typ == UPPER) || (typ == UPPER_B) )
					{
						/*converts the matrix of strings  str-matrix into upper case */
						wcOutput[y] = (wchar_t)towupper(wcInput[y]);
					}
					else if ( (typ==LOW) || (typ==LOW_B) )
					{
						/*converts the matrix of strings  str-matrix  into lower case */
						wcOutput[y] = (wchar_t)towlower(wcInput[y]);
					}
				}
				FREE(wcInput); wcInput = NULL;
				wcOutput[y] = L'\0'; 
				Output_Matrix[x] = wide_string_to_UTF8(wcOutput);
				FREE(wcOutput); wcOutput = NULL;
			}
			else
			{
				Output_Matrix[x] = NULL;
			}
		}
	}
	return Output_Matrix;
}
/*--------------------------------------------------------------------------*/
