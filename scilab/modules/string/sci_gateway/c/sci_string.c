
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
#include <string.h>
#include "gw_string.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intstring) (void); /* fortran routine */
/*--------------------------------------------------------------------------*/
int C2F(sci_string)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,3);

	switch(VarType(1))
	{
		case sci_matrix:
		{
			if (Lhs == 1)
			{
				int relativePosition = Top - Rhs + 1;
				int address = *Lstk(relativePosition);
				int intAddress = iadr(address);
				int m = getNumberOfLines(intAddress);
				int n = getNumberOfColumns(intAddress);

				/* case string(A) returns '' */
				/* with A has special dimensions (-1,-1) */
				/* bug 3747 */
				if ( (m == -1) && (n == -1) ) 
				{
					int m1 = 0, n1 = 0, l1 = 0;
					CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
				}
				else C2F(intstring)();
			}
			else Error(39);
		}
		break;

		case sci_poly: 
		case sci_boolean:
		case sci_sparse: 
		case sci_boolean_sparse:
		case sci_matlab_sparse: 
		case sci_ints:
		case sci_handles: 
		case sci_strings:
		case sci_list: 
		case sci_tlist:
		case sci_mlist: 
		case sci_lufact_pointer:
		{
			if (Lhs == 1)
			{
				C2F(intstring)();
			}
			else
			{
				Error(39);
			}
		}
		break;

		case sci_u_function: 
		case sci_c_function:
		{
			/* macros */
			if (Lhs == 3)
			{
				C2F(intstring)();
			}
			else Error(41);
		}
		break;
		case sci_lib:
		{
			if (Lhs == 1)
			{
				C2F(intstring)();
			}
			else Error(39);
		}
		break;

		default:
		{
			Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname,1);
		}
		break;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
