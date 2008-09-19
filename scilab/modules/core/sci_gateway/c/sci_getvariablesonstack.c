/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "stack-c.h"
#include "gw_core.h"
#include "getvariablesname.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getvariablesonstack)(char *fname,unsigned long fname_len)
{
	int l1,n1,m1;
	char **Variables = NULL;
	int sizeVariables = 0;

	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		Variables = getVariablesName(&sizeVariables,TRUE);
	}
	else
	{
		if (GetType(1) == sci_strings)
		{
			char *parameter = NULL;
			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			parameter = cstk(l1);

			if ( ( strcmp(parameter,"local") == 0 ) || ( strcmp(parameter,"global") == 0) )
			{
				if (strcmp(parameter,"local") == 0)
				{
					Variables = getLocalVariablesName(&sizeVariables,TRUE);
				}
				else /* global */
				{
					Variables = getGlobalVariablesName(&sizeVariables,TRUE);
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,1, "global","locale");
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
			return 0;
		}
	}

	m1 = sizeVariables;
	n1 = 1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, Variables);

	LhsVar(1) = Rhs + 1;
	C2F(putlhsvar)();

	if (Variables)
	{
		int i = 0;
		for( i = 0; i < sizeVariables; i++)
		{
			if (Variables[i])
			{
				FREE(Variables[i]);
				Variables[i] = NULL;
			}
		}
		FREE(Variables);
		Variables = NULL;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
