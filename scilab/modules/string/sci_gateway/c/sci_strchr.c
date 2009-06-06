
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

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "stringsstrrchr.h"
/*----------------------------------------------------------------------------*/
int sci_strchr(char *fname,unsigned long fname_len)
{
	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		int m1 = 0, n1 = 0;
		char **Input_StringMatrix_1 = NULL;
		int m1n1 = 0;

		int m2 = 0, n2 = 0;
		char **Input_StringMatrix_2 = NULL;
		int m2n2 = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_StringMatrix_1);
		m1n1 = m1 * n1 ;

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Input_StringMatrix_2);
		m2n2 = m2 * n2 ;

		if ( ((m1==m2) &&(n1==n2)) || (m2n2 == 1) )
		{
			char **Output_Strings = NULL;
			int i = 0;

			for(i = 0;i < m2n2; i++)
			{
				if (strlen(Input_StringMatrix_2[i]) != 1)
				{
					freeArrayOfString(Input_StringMatrix_1,m1n1);
					freeArrayOfString(Input_StringMatrix_2,m2n2);
					Scierror(999,_("%s: Wrong size for input argument #%d: A character expected.\n"),fname,2);
					return 0;
				}
			}

			Output_Strings = strings_strrchr(Input_StringMatrix_1,m1n1,Input_StringMatrix_2,m2n2,FALSE);
			if (Output_Strings)
			{
				CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,Output_Strings);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();

				freeArrayOfString(Input_StringMatrix_1,m1n1);
				freeArrayOfString(Input_StringMatrix_2,m2n2);
				freeArrayOfString(Output_Strings,m1n1);
			}
			else
			{
				freeArrayOfString(Input_StringMatrix_1,m1n1);
				freeArrayOfString(Input_StringMatrix_2,m2n2);
				Scierror(999,_("%s: No more memory.\n"),fname);
			}
		}
		else
		{
			freeArrayOfString(Input_StringMatrix_1,m1n1);
			freeArrayOfString(Input_StringMatrix_2,m2n2);
			Scierror(999,_("%s : Wrong size for input argument #%d.\n"),fname,2);
			return 0;
		}
	}
	else
	{
		if(GetType(1) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A character expected.\n"),fname,2);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
