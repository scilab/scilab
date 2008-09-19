/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - 2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "BOOL.h"
#include "stack-c.h"
#include "Funtab.h"
#include "IsAScalar.h"
#include "hashtable_core.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static BOOL Is_a_correct_function_name(char *functionname);
/*--------------------------------------------------------------------------*/
int C2F(sci_newfun) (char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;

	CheckRhs(2,2);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) &&  IsAScalar(2) )
	{
		char *VarName=NULL;
		int nameptr=0;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		nameptr=*istk(l1);

		if (nameptr > 0)
		{
			if (Is_a_correct_function_name(VarName))
			{
				BOOL IdAlreadyExist = FALSE;
				int id[nsiz];
				int i = 0;
				int zero = 0;
				int fptr = nameptr;
				int job = 0 ;

				/* initialize id */
				for(i = 0; i < nsiz; i++) id[i] = 0;

				/* checks in functions table that nameptr exists */
				job = SCI_HFUNCTIONS_BACKSEARCH;
				C2F(funtab)(id,&fptr,&job,VarName,(unsigned long)strlen(VarName));

				for(i = 0; i < nsiz; i++) 
				{
					if ( id[i] != 0 )
					{
						IdAlreadyExist = TRUE;
						break;
					}
				}

				if (IdAlreadyExist) /* nameptr exists */
				{
					C2F(cvname)(id,VarName,&zero,(unsigned long)strlen(VarName));
					job = SCI_HFUNCTIONS_ENTER;
					C2F(funtab)(id,&fptr,&job,VarName,(unsigned long)strlen(VarName));
				}
				else
				{
					Scierror(999,_("%s: Invalid '%s' value.\n"),fname,"nameptr");
					return 0;
				}

				LhsVar(1)=0;
				PutLhsVar();
			}
			else
			{
				Scierror(999,_("%s: function-name is incorrect.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: Must be %s.\n"),fname,2, "> 0");
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input arguments #%d or #%d.\n"), fname,1, 2);
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static BOOL Is_a_correct_function_name(char *functionname)
{
	int i=0;

	for(i=0;i<(int)strlen(functionname);i++)
	{
		if (functionname[i] == ' ')
		{
			return FALSE;
		}
		if (functionname[i] == '+')
		{
			return FALSE;
		}
		if (functionname[i] == '-')
		{
			return FALSE;
		}
		if (functionname[i] == '/')
		{
			return FALSE;
		}
		if (functionname[i] == '\\')
		{
			return FALSE;
		}
		if (functionname[i] == '.')
		{
			return FALSE;
		}
		if (functionname[i] == '*')
		{
			return FALSE;
		}
		if (functionname[i] == ',')
		{
			return FALSE;
		}
		if (functionname[i] == '!')
		{
			return FALSE;
		}
		if (functionname[i] == ';')
		{
			return FALSE;
		}
		if (functionname[i] == '&')
		{
			return FALSE;
		}
		/* To be continued ... :( */
		/* To be BANNED !!!! */
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/
