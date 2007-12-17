/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intstring) _PARAMS((int *id)); /* fortran routine */
/*--------------------------------------------------------------------------*/
int C2F(sci_string) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(1,1);
	CheckLhs(1,3);

	switch(VarType(1))
	{
		case sci_matrix:
		{
			if (Lhs == 1)
			{
				int id[6];
				C2F(intstring)(id);
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
					int id[6];
					C2F(intstring)(id);
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
				int id[6];
				C2F(intstring)(id);
			}
			else Error(41);
		}
		break;
		case sci_lib:
		{
			if (Lhs == 1)
			{
				int id[6];
				C2F(intstring)(id);
			}
			else Error(39);
		}
		break;

		default:
		{
			Scierror(999,_("%s : Input type unknown.\n"),fname);
		}
		break;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
