
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

/* res = strcspn(str1,str2) */
/* Scans str1 for the first occurrence of any of the characters */
/* that are part of str2, returning the number of characters */
/* of str1 read before this first occurrence. */
/*---------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*---------------------------------------------------------------------------*/
int C2F(sci_strcspn)(char *fname,unsigned long fname_len)
{
	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		int m1 = 0; int n1 = 0;
		char **InputString_Parameter1 = NULL;
		int m1n1 = 0; /* m1 * n1 */

		int m2 = 0; int n2 = 0;
		char **InputString_Parameter2 = NULL;
		int m2n2 = 0; /* m2 * n2 */

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&InputString_Parameter1);
		m1n1 = m1 * n1;

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&InputString_Parameter2);
		m2n2 = m2 * n2;

		if ( ((m1 == m2) && (n1 == n2)) || (m2n2 == 1) )
		{
			int outIndex = 0;
			int i = 0;
			int j = 0;

			/*Output*/
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&outIndex);   
			for ( i = 0 ; i < m1n1 ; i++ )
			{
				if (m2n2 == 1) j = 0;
				else j = i;

				stk(outIndex)[i] = (double) strcspn( InputString_Parameter1[i], InputString_Parameter2[j] );
			}
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
		}
		else
		{
			freeArrayOfString(InputString_Parameter1,m1n1);
			freeArrayOfString(InputString_Parameter2,m2n2);
			Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,2);
			return 0;
		}
		
		freeArrayOfString(InputString_Parameter1,m1n1);
		freeArrayOfString(InputString_Parameter2,m2n2);
	}
	else
	{
		if(GetType(1) != sci_strings)
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
