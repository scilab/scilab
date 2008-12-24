
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : substitute a character string by another in a character string  */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "strsubst.h"
#include "localization.h"
#include "Scierror.h"
/*-------------------------------------------------------------------------------------*/
#define CHAR_R "r"
#define CHAR_S "s"
/*-------------------------------------------------------------------------------------*/
int C2F(sci_strsubst)(char *fname,unsigned long fname_len)
{
	BOOL bStrsubst_with_pattern = FALSE;
	CheckRhs(3,4);
	CheckLhs(1,1);

	switch (VarType(1))
	{
		case sci_matrix :
		{
			int m = 0, n = 0 , l = 0;
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
			if ( (m == 0) && (n == 0) ) /* strsubst([],...) */
			{
				l = 0;
				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();
				return 0;
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname);
				return 0;
			}
		}
		break;

		case sci_strings:
		{
			int m1 = 0 , n1 = 0;
			int m1n1 = 0; /* m1 * n1 */
			char **strings_input = NULL;
			
			int m2 = 0 , n2 = 0;
			int m2n2 = 0; /* m2 * n2 */
			char **string_to_search = NULL;
			
			int m3 = 0 , n3 = 0;
			int m3n3 = 0; /* m2 * n2 */
			char **replacement_string = NULL;
			
			char **Output_StringMatrix = NULL;
			
			if ( (VarType(2) != sci_strings) || (VarType(3) != sci_strings))
			{
				if(VarType(2) != sci_strings)
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
				}
				else
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,3);
				}
				
				return 0;
			}
			
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&strings_input);
			m1n1 = m1 * n1 ;
				
			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&string_to_search);
			m2n2 = m2 * n2 ;

			if (m2n2 != 1)
			{
				freeArrayOfString(strings_input,m1n1);
				freeArrayOfString(string_to_search,m2n2);
				Scierror(36,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname,2);
				return 0;
			}
			GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&replacement_string);
			m3n3 = m3 * n3 ;
			if (m3n3 != 1)
			{
				freeArrayOfString(strings_input,m1n1);
				freeArrayOfString(string_to_search,m2n2);
				freeArrayOfString(replacement_string,m3n3);
				Scierror(36,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,3);
				return 0;
			}

			if (Rhs == 4)
			{
				int m4 = 0;
				int n4 = 0;
				char **Strings_Input4 = NULL;
				int m4n4 = 0; /* m4 * n4 */

				if (VarType(4) != sci_strings)
				{
					Scierror(999,_("%s: Wrong type for input argument #%d: A character expected.\n"),fname,4);
					return 0;
				}
				GetRhsVar(4,MATRIX_OF_STRING_DATATYPE,&m4,&n4,&Strings_Input4);
				m4n4 = m4*n4;  
				
				if (m4n4 != 1)
				{
					freeArrayOfString(Strings_Input4,m4n4);
					Scierror(999,_("%s: Wrong size for input argument #%d: A character expected.\n"),fname,4);
					return 0;
				}
				
				if ( (strcmp(Strings_Input4[0],CHAR_R) == 0) || (strcmp(Strings_Input4[0],CHAR_S) == 0) )
				{
					if (strcmp(Strings_Input4[0],CHAR_R) == 0)
					{
						bStrsubst_with_pattern = TRUE;
					}
					else
					{
						bStrsubst_with_pattern = FALSE;
					}
					freeArrayOfString(Strings_Input4,m4n4);
				}
				else
				{
					freeArrayOfString(Strings_Input4,m4n4);
					Scierror(999,_("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),fname,4,"s","r");
					return 0;
				}
			}
			
			if (bStrsubst_with_pattern)
			{
				Output_StringMatrix = strsubst_reg(strings_input,m1n1,string_to_search[0],replacement_string[0]);
			}
			else
			{
				Output_StringMatrix = strsubst(strings_input,m1n1,string_to_search[0],replacement_string[0]);
			}
			freeArrayOfString(strings_input,m1n1);
			freeArrayOfString(string_to_search,m2n2);
			freeArrayOfString(replacement_string,m3n3);
			
			/* put on scilab stack */
			CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1,Output_StringMatrix );
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			freeArrayOfString(Output_StringMatrix,m1n1);
		}
		break;

		default:
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
			return 0;
		}
		break;
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
