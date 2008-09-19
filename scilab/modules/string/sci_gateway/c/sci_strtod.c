
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
#include "stringsstrrchr.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_strtod)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		int m1 = 0, n1 = 0;
		char **Input_StringMatrix_1 = NULL;
		int m1n1 = 0;

		int x = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_StringMatrix_1);
		m1n1 = m1 * n1 ;

		if (Lhs == 2)
		{
			double *OutputDoubles = NULL;
			char **OutputStrings = NULL;

			OutputStrings = (char **)MALLOC(sizeof(char*)*m1n1);
			if (OutputStrings == NULL)
			{
				freeArrayOfString(Input_StringMatrix_1,m1n1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}

			OutputDoubles = (double*)MALLOC(sizeof(double)*m1n1);
			if (OutputDoubles == NULL)
			{
				FREE(OutputStrings); OutputStrings = NULL;
				freeArrayOfString(Input_StringMatrix_1,m1n1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}

			for ( x = 0 ; x < m1n1 ; x++ )
			{
				char *stopstring = NULL;
				OutputDoubles[x] = (double)strtod( Input_StringMatrix_1[x], &stopstring);

				if (stopstring) OutputStrings[x] = (char*)MALLOC(sizeof(char)*(strlen(stopstring)+1));
				else OutputStrings[x] = (char*)MALLOC(sizeof(char)*(strlen("")+1));

				if (OutputStrings[x] == NULL)
				{
					freeArrayOfString(Input_StringMatrix_1,m1n1);
					freeArrayOfString(OutputStrings,m1n1);
					if (OutputDoubles) {FREE(OutputDoubles); OutputDoubles = NULL;}
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				}

				if (stopstring) strcpy(OutputStrings[x],stopstring);
				else strcpy(OutputStrings[x],"");
			}
			CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&OutputDoubles);
			CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,OutputStrings);

			LhsVar(1) = Rhs+1 ;
			LhsVar(2) = Rhs+2 ;

			freeArrayOfString(OutputStrings,m1n1);
			if (OutputDoubles) {FREE(OutputDoubles); OutputDoubles = NULL;}
		}
		else /* Lhs == 1 */
		{
			int outIndex = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&outIndex);
			for ( x = 0 ; x < m1n1 ; x++ )
			{
				char  *stopstring = NULL;
				stk(outIndex)[x] = (double)strtod( Input_StringMatrix_1[x], &stopstring);
			
			}
			LhsVar(1) = Rhs+1 ;
		}

		freeArrayOfString(Input_StringMatrix_1,m1n1);
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
