/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_set_fftw_wisdom.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/

/* Set fftw wisdom
 *
 * Scilab Calling sequence :
 *   -->set_fftw_wisdom(txt);
 *
 * Input : a scilab string matrix
 *
 * Output : Nothing or an error messsage if
 *          wisdom can't be read by fftw
 *
 */
int sci_set_fftw_wisdom(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);

	if (VarType(1)==sci_strings) 
	{
		char **Str1 = NULL;
		char *Str = NULL;
		int m1 = 0, n1 = 0;
		int len = 0, k = 0;
		int j = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str1);
		
		for (j = 0; j < m1*n1; j++) 
		{
			int i = 0;

			len += (int)strlen(Str1[j])+1;

			if (Str) Str = (char *)REALLOC(Str,sizeof(char)*(len));
			else Str = (char *)MALLOC(sizeof(char)*(len));

			if (Str == NULL) 
			{
				freeArrayOfString(Str1,m1*n1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return(0);
			}

			for (i = 0; i < (int)strlen(Str1[j]); i++) 
			{
				Str[k+i] = Str1[j][i];
			}
			Str[k+strlen(Str1[j])] = '\n';
			k += (int)strlen(Str1[j])+1;
		}
		Str[k-1] = '\0';

		freeArrayOfString(Str1,m1*n1);
  
		if(!(call_fftw_import_wisdom_from_string(Str))) 
		{
			FREE(Str); Str = NULL;
			Scierror(999,_("%s: An error occurred: %s\n"),fname,_("FFTW can't read wisdom."));
			return(0);
		}
		FREE(Str); Str = NULL;
	}
	else 
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return(0);
	}

	LhsVar(1)= 0;
	PutLhsVar();
	return(0);
}
/*--------------------------------------------------------------------------*/
