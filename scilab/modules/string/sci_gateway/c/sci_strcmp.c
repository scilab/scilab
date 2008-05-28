
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

/*-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
#include "stringsCompare.h"
/*----------------------------------------------------------------------------*/
#define CHAR_I 'i'
#define CHAR_S 's'
/*----------------------------------------------------------------------------*/
int C2F(sci_strcmp)(char *fname,unsigned long fname_len)
{
	int Type_One = 0;
	int Type_Two = 0;
	BOOL do_stricmp = FALSE;

	CheckRhs(2,3);
	CheckLhs(1,1);

	Type_One = VarType(1);
	Type_Two = VarType(2);

	if (Rhs == 3)
	{
		int Type_Three = VarType(3);
		if (Type_Three == sci_strings)
		{
			int Row_Num_Two = 0,Col_Num_Two = 0,Stack_Pos=0;
			
			GetRhsVar(3,STRING_DATATYPE,&Row_Num_Two,&Col_Num_Two,&Stack_Pos);
			if ( (Row_Num_Two*Col_Num_Two) == 1 )
			{
				char typ = 's';
				/* To put "flag" into typ; whether "i"*/
				typ = cstk(Stack_Pos)[0];
				if ( (typ != CHAR_I) && (typ != CHAR_S))
				{
					Scierror(999,_("%s: Wrong value for input argument #%d: %s or %s expected.\n"),fname,3,"'i' (stricmp)","'s' (strcmp)");
					return 0;	
				}
				 if (typ == CHAR_I) do_stricmp = TRUE;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,3);
			return 0;
		}
	}
	
	if ( (Type_One == sci_strings) && (Type_Two == sci_strings) )
	{
		char **Input_Strings_One = NULL;
		int m1 = 0, n1 = 0;
		int m1n1 = 0;

		char **Input_Strings_Two = NULL;
		int m2 = 0, n2 = 0;
		int m2n2 = 0;

		/* To input the string matrix */
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_Strings_One); 
		m1n1 = m1 * n1;

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Input_Strings_Two); 
		m2n2 = m2 * n2;

		if ( ( (m2 == m1) && (n2 == n1) ) || (m2n2 == 1) )
		{
			int *values = stringsCompare(Input_Strings_One,m1n1,Input_Strings_Two,m2n2,do_stricmp);

			if (values == NULL)
			{
				freeArrayOfString(Input_Strings_One,m1n1);
				freeArrayOfString(Input_Strings_Two,m2n2);
				Scierror(999,_("%s : No more memory.\n"),fname);
			}
			else
			{
				int outIndex = 0 ;
				int x = 0;

				CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &m1,&n1, &outIndex );
				for  ( x = 0; x < m1n1; x++ )
				{
					stk(outIndex)[x] = values[x];
				}
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();

				freeArrayOfString(Input_Strings_One,m1n1);
				freeArrayOfString(Input_Strings_Two,m2n2);
				FREE(values);
				values = NULL;
			}
		}
		else
		{
			freeArrayOfString(Input_Strings_One,m1n1);
			freeArrayOfString(Input_Strings_Two,m2n2);
			Scierror(999,_("%s: Wrong size for input arguments: Same sizes expected.\n"),fname);
		}
	}
	else
	{
		if(Type_One != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,2);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
