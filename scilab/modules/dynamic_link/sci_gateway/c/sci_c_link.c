
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

/*--------------------------------------------------------------------------*/
#include "gw_dynamic_link.h"
#include "machine.h"
#include "stack-c.h"
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_c_link)(char *fname,unsigned long fname_len)
{
	int ilib = 0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	/* [test,ilib]=c_link(routine-name)  */
	/* test = c_link(routine-name)  */
	/* test = c_link(routine-name,num)  */

	if (Rhs == 2)
	{
		if (VarType(2) == sci_matrix)
		{
			int m1 = 0, n1 = 0, l1 = 0;
			GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			if ( (m1 == n1) && (n1 == 1) )
			{
				ilib =  (int)*stk(l1);
			}
			else
			{
				Scierror(999,_("%s : second argument must be a unique id of a shared library.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s : second argument must be a unique id of a shared library.\n"),fname);
			return 0;
		}
	}
	else ilib = -1;


	if (VarType(1) == sci_strings)
	{
		char **routinename = NULL;
		int m1 = 0, n1 = 0;
		GetRhsVar(1,"S",&m1,&n1,&routinename);

		if ( (m1 == 1) && (n1 == 1) )
		{
			int *paramoutINT=(int*)MALLOC(sizeof(int));
			BOOL FindFunction = FALSE;

			FindFunction = c_link(routinename[0],&ilib);
			if (routinename) {FREE(routinename);routinename = NULL;}

			

			if ( FindFunction )
			{
				*paramoutINT=(int)(TRUE);
			}
			else
			{
				*paramoutINT=(int)(FALSE);
			}

			CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;

			if (Lhs == 2)
			{
				int one = 1;
				int l = 0;

				CreateVar(Rhs+2, MATRIX_OF_INTEGER_DATATYPE, &one, &one,&l);
				*istk(l) = (int)ilib;
				LhsVar(2)=Rhs+2;
			}

			C2F(putlhsvar)();
			if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
			
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname,1);
		}
	}
	else
	{
		Error(201);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
