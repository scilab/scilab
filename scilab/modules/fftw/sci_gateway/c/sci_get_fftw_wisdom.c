/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_get_fftw_wisdom.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/

/* Return text of fftw wisdom
 *
 * Scilab Calling sequence :
 *   -->tt=get_fftw_wisdom();
 *
 * Input : Nothing
 *
 * Output : a scilab string matrix
 *
 */
int sci_get_fftw_wisdom(char *fname,unsigned long fname_len)
{
	int n1 = 0,i = 0,j = 0;
	char *Str = NULL;
	char **Str1 = NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Str = call_fftw_export_wisdom_to_string();

	n1 = 0; j = 0;
	for(i = 0; i < (int)strlen(Str); i++) 
	{
		if (Str[i] == '\n') 
		{
			int len = 0;
			int k = 0;

			n1++;

			if (Str1) Str1 = (char **)REALLOC(Str1,sizeof(char *)*n1);
			else Str1 = (char **)MALLOC(sizeof(char *)*n1);

			if (Str1 == NULL) 
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return(0);
			}
			len = i-j;
			if ((Str1[n1-1] = (char *)MALLOC(sizeof(char)*(len+1))) == NULL) 
			{
				freeArrayOfString(Str1,n1-1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return(0);
			}

			for(k = 0; k < len;k++) 
			{
				Str1[n1-1][k] = Str[k+j];
			}
			Str1[n1-1][len] = '\0';
			j = i+1;
		}
	}

	n1++;

	if (Str1) Str1 = (char **)REALLOC(Str1,sizeof(char *)*n1);
	else Str1 = (char **)MALLOC(sizeof(char *)*n1);

	if (Str1 == NULL) 
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return(0);
	}
	if ((Str1[n1-1] = (char *)MALLOC(sizeof(char))) == NULL) 
	{
		freeArrayOfString(Str1,n1-1);
		Scierror(999,_("%s: No more memory.\n"),fname);
		return(0);
	}
	Str1[n1-1][0] = '\0';

	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &n1, (j=1,&j), Str1);
	LhsVar(1) = Rhs+1;
	PutLhsVar();

	freeArrayOfString(Str1,n1);
	return(0);
}
/*--------------------------------------------------------------------------*/
