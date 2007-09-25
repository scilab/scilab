/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(where)();
/*-----------------------------------------------------------------------------------*/
int C2F(sci_where) _PARAMS((char *fname,unsigned long fname_len))
{
	if (Rhs > 0)
	{
		error_scilab(39,"incorrect number of arguments.");
		return 0;
	}

	if (Lhs != 2)
	{
		error_scilab(41,"incompatible LHS.");
		return 0;
	}

 	C2F(where)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
