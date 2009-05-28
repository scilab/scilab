
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
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*----------------------------------------------------------------------------*/
static BOOL previousCallToStrtok = FALSE;
static char *previousStrToken = NULL;
/*----------------------------------------------------------------------------*/
int sci_strtok(char *fname,unsigned long fname_len)
{
	char **Input_strings1 = NULL;
	char **Input_strings2 = NULL;
	char *token = NULL;

	int m = 0, n = 0 ;
	int outIndex = 0 ;

	int m2n2 = 0;
	int m1n1 = 0;

	CheckRhs(1,2);
	CheckLhs(0,1);

	if (Rhs == 2)
	{
		if (GetType(2) == sci_strings)
		{
			int m2 = 0, n2 = 0;
			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Input_strings2);
			m2n2 = m2*n2;
			if (m2n2 != 1)
			{
				freeArrayOfString(Input_strings2,m2n2);
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}
	}
	
	if (GetType(1) == sci_strings)
	{
		int m1 = 0, n1 = 0;
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_strings1);
		m1n1 = m1*n1;
		if (m1n1 != 1)
		{
			freeArrayOfString(Input_strings1,m1n1);
			freeArrayOfString(Input_strings2,m2n2);
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}
	}
	else
	{
		freeArrayOfString(Input_strings2,m2n2);
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	if (Rhs == 2)
	{
		/* strtok need to backup pointer on string */
		if (previousStrToken) previousStrToken = (char*)REALLOC(previousStrToken,strlen(Input_strings1[0])+1);
		else previousStrToken = (char*)MALLOC(sizeof(char*)*(strlen(Input_strings1[0])+1));

		if (previousStrToken)
		{
			strcpy(previousStrToken,Input_strings1[0]);
			token = strtok(previousStrToken, Input_strings2[0]);
			previousCallToStrtok = TRUE;
		}
		else token = NULL;
	}
	else 
	{
		if (previousCallToStrtok) token = strtok(NULL,Input_strings1[0]);
		else token = NULL;
	}
	
	freeArrayOfString(Input_strings1,m1n1);
	freeArrayOfString(Input_strings2,m2n2);

	if (token)
	{
		m = (int)strlen(token);
		n = 1;
		CreateVar(Rhs+1,STRING_DATATYPE,&m,&n,&outIndex);
		strcpy(cstk(outIndex),token);
	}
	else
	{
		if (previousStrToken) {FREE(previousStrToken);previousStrToken = NULL;}
		previousCallToStrtok = FALSE;
		outIndex = 0;
		m = 0;
		n = 0;
		CreateVar(Rhs+1,STRING_DATATYPE,  &m, &n, &outIndex);
	}

	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
