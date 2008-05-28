
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - 2008 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "tokens.h"
#include "localization.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define one 1
/*--------------------------------------------------------------------------*/
int C2F(sci_tokens)(char *fname,unsigned long fname_len)
{

	int Row_One = 0,Col_One = 0; 
	int mn_One = 0; /* Row_One * Col_One */
	char **Input_Strings = NULL;
	char *StringWhereWeSearch = NULL;

	int Row_Two = 0,Col_Two = 0;
	int mn_Two = 0; /* Row_Two * Col_Two */
	char **Delimiters = NULL;
	int sizeDelimiters = 0;

	char **Output_Strings = NULL;
	int sizeOutput_Strings = 0;
	int Row_Output = 0,Col_Output = 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if ( VarType(1) == sci_strings )
	{
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_Strings);
		mn_One = Row_One * Col_One;

		if (mn_One == 1)
		{
			if (Rhs == 2)
			{
				if ( VarType(2) == sci_strings )
				{
					int i = 0;
					GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Delimiters); 
					mn_Two = Row_Two * Col_Two;

					for (i = 0; i < mn_Two ; i++)
					{
						if (Delimiters[i])
						{
							if ( (int)strlen(Delimiters[i]) > 1)
							{
								freeArrayOfString(Input_Strings,mn_One);
								Scierror(999,_("%s: Wrong type for input argument #%d: A character or a vector of single characters expected.\n"),fname,2);
								return 0;
							}
						}
					}
					sizeDelimiters = mn_Two;
				}
				else
				{
					freeArrayOfString(Input_Strings,mn_One);
					Scierror(999,_("%s: Wrong type for input argument #%d: A character or a vector of single characters expected.\n"),fname);
					return 0;
				}
			}
			else
			{
				/* default delimiters are ' ' and Tabulation */
				sizeDelimiters = 2;
				Delimiters = (char**)MALLOC(sizeof(char*)*(sizeDelimiters+1));
				if (Delimiters == NULL)
				{
					freeArrayOfString(Input_Strings,mn_One);
					Scierror(999,_("%s : Memory allocation error.\n"),fname);
					return 0;
				}
				Delimiters[0] = strdup(" ");
				Delimiters[1] = strdup("\t");
				Delimiters[2] = 0;
			}

			StringWhereWeSearch = Input_Strings[0];

			/* tokens('') */
			if ( (int)strlen(StringWhereWeSearch) == 0 )
			{
				int l = 0;
				Row_Output = 0;
				Col_Output = 0;

				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_Output,&Col_Output,&l);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();
			}
			else
			{
				Output_Strings = stringTokens(StringWhereWeSearch,Delimiters,sizeDelimiters,&sizeOutput_Strings);
				if ( (Output_Strings == NULL) && (sizeOutput_Strings == 0) ) /* returns [] */
				{
					int l = 0;
					Row_Output = 0;
					Col_Output = 0;

					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_Output,&Col_Output,&l);
					LhsVar(1) = Rhs+1 ;
					C2F(putlhsvar)();
				}
				else
				{
					Row_Output = sizeOutput_Strings;
					Col_Output = one;
					CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &Row_Output, &Col_Output, Output_Strings );
					LhsVar(1) = Rhs+1 ;
					C2F(putlhsvar)();

					freeArrayOfString(Output_Strings,sizeOutput_Strings);
				}
			}

			freeArrayOfString(Input_Strings,mn_One);
			freeArrayOfString(Delimiters,sizeDelimiters);
		}
		else
		{
			freeArrayOfString(Input_Strings,mn_One);
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
