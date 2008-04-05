/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : search position of a character string in an other string        */
/*        using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "machine.h"
#include "MALLOC.h"
#include "kmp.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre_private.h"
#include "BOOL.h"
#include "pcre_error.h"
#include "Scierror.h"
/*------------------------------------------------------------------------*/
#define CHAR_S "s"
#define CHAR_R "r"
struct In
{
		int data;
		int position;
} ;
int cmp( const void *a ,const void *b)
{
return (*(struct In *)a).data > (*(struct In *)b).data ? 1 : -1;
}

/*------------------------------------------------------------------------*/
int C2F(sci_strindex)(char *fname,unsigned long fname_len)
{
	BOOL bStrindex_with_pattern = FALSE;
	int outIndex = 0;
	int numRow = 1;
    int *next= NULL;
	int i = 0;

	CheckRhs(2,3);
	CheckLhs(1,2);

	if (Rhs == 3)
	{
		int m3 = 0;
		int n3 = 0;
		char **Strings_Input3 = NULL;
		int m3n3 = 0; /* m3 * n3 */

		if (VarType(3) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for third input argument: String expected.\n"),fname);
			return 0;
		}
		GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Strings_Input3);
		m3n3 = m3*n3;

		if (m3n3 != 1)
		{
			freeArrayOfString(Strings_Input3,m3n3);
			Scierror(999,_("%s: Wrong type for third input argument: String expected.\n"),fname);
			return 0;
		}

		if ( (strcmp(Strings_Input3[0],CHAR_R) == 0) || (strcmp(Strings_Input3[0],CHAR_S) == 0) )
		{
			if (strcmp(Strings_Input3[0],CHAR_R) == 0)
			{
				bStrindex_with_pattern = TRUE;
			}
			else
			{
				bStrindex_with_pattern = FALSE;
			}
			freeArrayOfString(Strings_Input3,m3n3);
		}
		else
		{
			freeArrayOfString(Strings_Input3,m3n3);
			Scierror(999,_("%s: Wrong type for third input argument: ''%s'' or ''%s'' expected.\n"),fname,"s","r");
			return 0;
		}
	}

	if (VarType(1) == sci_matrix)
	{
		int m1 = 0;
		int n1 = 0;
		int l1 = 0;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		if ((m1 == 0) && (n1 == 0))
		{
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;
		}
		else
		{
			Scierror(999,_("%s: Wrong type for third input argument: String or [] expected.\n"),fname);
			return 0;
		}
	}

	if ( (VarType(1) == sci_strings) && (VarType(2) == sci_strings) )
	{
		int m1 = 0, n1 = 0;
		char **Strings_Input1 = NULL;
		int m1n1 = 0; /* m1 * n1 */

		int m2 = 0, n2 = 0;
		char **Strings_Input2 = NULL;
		int m2n2 = 0; /* m2 * n2 */


        struct In *values=NULL;

	    int nbValues = 0;
		int nbposition = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input1);
		m1n1 = m1*n1;

		if (m1n1 != 1)
		{
			freeArrayOfString(Strings_Input1,m1n1);
			Scierror(999,_("%s: Wrong type for first input argument: String expected.\n"),fname);
			return 0;
		}

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input2);
		m2n2 = m2*n2;

		if ( (m2 != 1) && (n2 != 1) )
		{
			freeArrayOfString(Strings_Input1,m1n1);
			freeArrayOfString(Strings_Input2,m2n2);
			Scierror(999,_("%s: Wrong type for second input argument: String or vector of strings expected.\n"),fname);
			return 0;
		}

		if ( (int)strlen(Strings_Input1[0]) == 0 )
		{
			values= (struct In*)MALLOC(sizeof(struct In));
		}
		else
		{
			values = (struct In *)MALLOC( sizeof(struct In) * ( strlen(Strings_Input1[0]) ) );

		}

		if (bStrindex_with_pattern)
		{
			int x = 0;
			int w = 0;

			int Output_Start = 0;
			int Output_End = 0;

			/* We use pcre library */
			for (x = 0; x < m2n2; ++x)
			{
				w = pcre_private(Strings_Input1[0],Strings_Input2[x],&Output_Start,&Output_End);
				if ( w == 0)
				{
					values[nbValues++].data = Output_Start+1; /* adding the answer into the outputmatrix */
					values[nbposition++].position = x+1;        /* The number according to the str2 matrix */
				}
				else
				{
					pcre_error("GREP_NEW",w);
					break;
				}
			}



           qsort(values,nbValues,sizeof(values[0]),cmp);


		}
		else
		{
			int x = 0;
			int pos = 0;
				/* We don't use pcre library */
			for (x=0; x < m2n2 ;++x)
			{
				int w = 0;
				if ( strlen(Strings_Input2[x]) == 0 )
				{
						freeArrayOfString(Strings_Input2,m2n2);
						freeArrayOfString(Strings_Input1,m1n1);
						if (next) {FREE(next); next = NULL;}
						if (values) {FREE(values); values = NULL;}
						Scierror(999, _("%s: Wrong size for second input argument: Non-empty string expected.\n"), fname);
						return 0;
				}
				if (Strings_Input2)
				{
					do
					{
						next=getnext(Strings_Input2[x]);
						/*Str is the input string matrix, Str2[x] is the substring to match; pos is the start point*/
						w = kmp(*Strings_Input1,Strings_Input2[x],pos,next);
						if (w !=0)
						{
							values[nbValues++].data = w;
							values[nbposition++].position = x+1;
						}
						pos = w;
					}
					while(w != 0);/* w is the answer of the kmp algorithem*/

					/* values are sorted */
					qsort(values,nbValues,sizeof(values[0]),cmp);
				}
			}
		}

		freeArrayOfString(Strings_Input1,m1n1);
		freeArrayOfString(Strings_Input2,m2n2);

		numRow   = 1;
		outIndex = 0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
		for ( i = 0 ; i < nbValues ; i++ )
		{
			stk(outIndex)[i] = (double)values[i].data ;
		}
		LhsVar(1) = Rhs+1 ;

		if (Lhs == 2)
		{
			numRow   = 1;
			outIndex = 0;
			CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbposition,&outIndex);
			for ( i = 0 ; i < nbposition ; i++ )
			{
				stk(outIndex)[i] = (double)values[i].position ;
			}
			LhsVar(2) = Rhs+2;
		}

		C2F(putlhsvar)();

		if (values) {FREE(values); values = NULL;}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input arguments: Strings expected.\n"),fname);
		return 0;
	}
	return 0;
}
/*------------------------------------------------------------------------*/
