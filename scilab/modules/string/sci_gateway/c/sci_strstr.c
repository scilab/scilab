
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
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_strstr)(char *fname,unsigned long fname_len)
{
	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		int m1 = 0; int n1 = 0;
		char **InputString_Parameter1 = NULL;
		int m1n1 = 0; /* m1 * n1 */

		int m2 = 0; int n2 = 0 ;
		char **InputString_Parameter2 = NULL;
		int m2n2 = 0; /* m2 * n2 */

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&InputString_Parameter1);
		m1n1 = m1 * n1;

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&InputString_Parameter2);
		m2n2 = m2 * n2;

		if (m2n2 == 0)
		{
			freeArrayOfString(InputString_Parameter1,m1n1);
			freeArrayOfString(InputString_Parameter2,m2n2);
			Scierror(999,_("%s: Wrong size for second argument: Non-empty matrix of strings expected.\n"),fname);
			return 0;
		}
		else
		{
			int j = 0;
			if ( ((m1 == m2) && (n1 == n2)) || (m2n2 == 1) )
			{
				char **OutputStrings = NULL;

				OutputStrings = (char**)MALLOC(sizeof(char*)*m1n1);
				if (OutputStrings == NULL)
				{
					freeArrayOfString(InputString_Parameter1,m1n1);
					freeArrayOfString(InputString_Parameter2,m2n2);
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				}

				for (j = 0;j < m1n1; j++)
				{
					int i = 0;

					if (m2n2 == 1) 
					{
						i = 0; /* Second input argument is one dimensional */
					}
					else 
					{
						i = j; /* First and second input arguments have same size */
					}

					if (strlen(InputString_Parameter1[j])< strlen(InputString_Parameter2[i]))
					{
						freeArrayOfString(OutputStrings,m1n1);
						freeArrayOfString(InputString_Parameter1,m1n1);
						freeArrayOfString(InputString_Parameter2,m2n2);
						Scierror(999,_("%s: Wrong size for second input argument.\n"),fname);
						return 0;
					}
					else
					{
						char *ptrstrstr = strstr(InputString_Parameter1[j],InputString_Parameter2[i]);
						if (ptrstrstr)
						{
							OutputStrings[j] = (char *)MALLOC(sizeof(char)*(strlen(ptrstrstr)+1));
							if (OutputStrings[j])
							{
								strcpy(OutputStrings[j],ptrstrstr);
							}
							else
							{
								freeArrayOfString(OutputStrings,m1n1);
								freeArrayOfString(InputString_Parameter1,m1n1);
								freeArrayOfString(InputString_Parameter2,m2n2);
								Scierror(999,_("%s: No more memory.\n"),fname);
								return 0;
							}
						}
						else
						{
							OutputStrings[j] = (char *)MALLOC(sizeof(char)*(strlen("")+1));
							if (OutputStrings[j])
							{
								strcpy(OutputStrings[j],"");
							}
							else
							{
								freeArrayOfString(OutputStrings,m1n1);
								freeArrayOfString(InputString_Parameter1,m1n1);
								freeArrayOfString(InputString_Parameter2,m2n2);
								Scierror(999,_("%s: No more memory.\n"),fname);
								return 0;
							}
						}
					}
				}

				CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,OutputStrings);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();

				freeArrayOfString(OutputStrings,m1n1);
				freeArrayOfString(InputString_Parameter1,m1n1);
				freeArrayOfString(InputString_Parameter2,m2n2);
			}
			else
			{
				freeArrayOfString(InputString_Parameter1,m1n1);
				freeArrayOfString(InputString_Parameter2,m2n2);
				Scierror(999,_("%s: Wrong size for second parameter.\n"),fname);
				return 0;
			}
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument(s): Matrix of strings expected.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
