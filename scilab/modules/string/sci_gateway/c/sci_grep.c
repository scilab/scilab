
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : search position of a character string in an other string
          using regular express .                                         */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "gw_string.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "MALLOC.h" /* MALLOC */
#include "pcre_private.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre_error.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*------------------------------------------------------------------------*/
#define GREP_OK             0
#define MEMORY_ALLOC_ERROR -1
/*------------------------------------------------------------------------*/
typedef struct grep_results
{
	int sizeArraysMax;
	int currentLength;
	int *values;
	int *positions;
} GREPRESULTS;
/*------------------------------------------------------------------------*/
static int sci_grep_common(char *fname,BOOL new_grep);
static int GREP_NEW(GREPRESULTS *results,char **Inputs_param_one,int mn_one,char **Inputs_param_two,int mn_two);
static int GREP_OLD(GREPRESULTS *results,char **Inputs_param_one,int mn_one,char **Inputs_param_two,int mn_two);
/*------------------------------------------------------------------------*/
int C2F(sci_grep)(char *fname,unsigned long fname_len)
{
    CheckRhs(2,3);
    CheckLhs(1,2);

	if (VarType(1) == sci_matrix)
	{
		int m1 = 0, n1 = 0;
		char **Str=NULL;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Str);

		if ((m1 == 0) && (n1 == 0))
		{
			int l = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();

			return 0;
		}
	}

	if (Rhs == 3)
	{
		if (VarType(3) == sci_strings)
		{
			char typ = 'd'; /*default */
			int m3 = 0,n3 = 0,l3 = 0;
			
			GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
			if ( m3*n3 != 0) typ = cstk(l3)[0];
			
			if (typ == 'r' )
			{
				sci_grep_common(fname,TRUE);
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),fname,3,"s");
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,3);
			return 0;
		}
	}
	else /* Rhs == 2 */
	{
		sci_grep_common(fname,FALSE);
	}
    return 0;
}
/*-----------------------------------------------------------------------------------*/
static int GREP_NEW(GREPRESULTS *results,char **Inputs_param_one,int mn_one,char **Inputs_param_two,int mn_two)
{
	int x = 0,y = 0;
    char *save=NULL;
	pcre_error_code answer = PCRE_FINISHED_OK;
	for (x = 0; x <  mn_one ;x++) 
	{
		results->sizeArraysMax = results->sizeArraysMax + (int)strlen(Inputs_param_one[x]);
	}

	results->values = (int *)MALLOC(sizeof(int)*(3*results->sizeArraysMax+1));
	results->positions = (int *)MALLOC(sizeof(int)*(3*results->sizeArraysMax+1));

	if ( (results->values == NULL) || (results->positions == NULL) )
	{
		if (results->values) {FREE(results->values);results->values = NULL;}
		if (results->positions) {FREE(results->positions);results->positions = NULL;}
		return MEMORY_ALLOC_ERROR;
	}

	results->currentLength = 0;
	for ( y = 0; y < mn_one; ++y)
	{
		for ( x = 0; x < mn_two; ++x)
		{
			int Output_Start = 0;
			int Output_End = 0;
			save = (char *)MALLOC( sizeof(char) * ( strlen(Inputs_param_two[x]) +1) );
			save = strdup(Inputs_param_two[x]);
			answer = pcre_private(Inputs_param_one[y],save,&Output_Start,&Output_End);

			if ( answer == PCRE_FINISHED_OK )
			{
				if (results->currentLength < results->sizeArraysMax) 
				{
					results->values[results->currentLength] = y+1;
					results->positions[results->currentLength] = x+1;
					results->currentLength++;
				}
			}
			else
			{
				pcre_error("grep",answer);
			}
			if (save) {FREE(save);save=NULL;}
		}
	}

	if (results->currentLength > results->sizeArraysMax) results->currentLength = results->sizeArraysMax;

	return GREP_OK;
}
/*-----------------------------------------------------------------------------------*/
static int GREP_OLD(GREPRESULTS *results,char **Inputs_param_one,int mn_one,char **Inputs_param_two,int mn_two)
{
	int x = 0,y = 0;

    results->values = (int *)MALLOC(sizeof(int)*(mn_one*mn_two+1));
	results->positions = (int *)MALLOC(sizeof(int)*(mn_one*mn_two+1));

	for (y = 0; y < mn_one; ++y)
	{
		for (x = 0; x < mn_two; ++x)
		{
			if (strstr(Inputs_param_one[y],Inputs_param_two[x])!=NULL)
			{
				results->values[results->currentLength] = y+1;
				results->positions[results->currentLength] = x+1;
				results->currentLength++;
			}
		}
	}
	return GREP_OK;
}
/*-----------------------------------------------------------------------------------*/
static int sci_grep_common(char *fname,BOOL new_grep)
{
	int i = 0;

	int m1 = 0, n1 = 0;
	char **Strings_Input_One = NULL;
	int m1n1 = 0; /* m1 * n1 */

	int m2 = 0, n2 = 0;
	char **Strings_Input_Two = NULL;
	int m2n2 = 0; /* m2 * n2 */

	GREPRESULTS grepresults;
	int code_error_grep = GREP_OK;

	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input_One);
	m1n1 = m1*n1;  
	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input_Two);
	m2n2 = m2*n2; 

	for (i = 0;i < m2n2;i++)
	{
		if ( strlen(Strings_Input_Two[i]) == 0)
		{
			Scierror(249,_("%s: Wrong values for input argument #%d: Non-empty strings expected.\n"),fname,2);
			return 0;
		}
	}

	grepresults.currentLength = 0;
	grepresults.sizeArraysMax = 0;
	grepresults.positions = NULL;
	grepresults.values = NULL;

	if (new_grep)
	{
		code_error_grep = GREP_NEW(&grepresults,Strings_Input_One,m1n1,Strings_Input_Two,m2n2);
	}
	else
	{
		code_error_grep = GREP_OLD(&grepresults,Strings_Input_One,m1n1,Strings_Input_Two,m2n2);
	}

	freeArrayOfString(Strings_Input_One,m1n1);
	freeArrayOfString(Strings_Input_Two,m2n2);

	switch (code_error_grep)
	{
		case GREP_OK :
		{
			int x = 0;
			int numRow   = 0;
			int outIndex = 0;

			numRow   = 1;  /* Output values[]*/
			outIndex = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&grepresults.currentLength,&outIndex);
			for ( x = 0 ; x < grepresults.currentLength ; x++ )
			{
				stk(outIndex)[x] = (double)grepresults.values[x] ;
			}
			LhsVar(1) = Rhs+1 ;
			if (Lhs == 2)
			{
				/* Output positions[]*/
				numRow   = 1;
				outIndex = 0;
				CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&grepresults.currentLength,&outIndex);
				for ( x = 0 ; x < grepresults.currentLength ; x++ )
				{
					stk(outIndex)[x] = (double)grepresults.positions[x] ;
				}
				LhsVar(2) = Rhs+2;
			}
			C2F(putlhsvar)();
			if (grepresults.values) {FREE(grepresults.values); grepresults.values = NULL;}
			if (grepresults.positions) {FREE(grepresults.positions); grepresults.positions = NULL;}
		}
		break;

		case MEMORY_ALLOC_ERROR :
		{
			if (grepresults.values) {FREE(grepresults.values); grepresults.values = NULL;}
			if (grepresults.positions) {FREE(grepresults.positions); grepresults.positions = NULL;}
			Scierror(999,_("%s: No more memory.\n"),fname);
		}
		break;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
