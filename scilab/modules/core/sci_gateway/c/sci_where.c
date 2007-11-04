/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(where)();
/*-----------------------------------------------------------------------------------*/
int C2F(sci_where) _PARAMS((char *fname,unsigned long fname_len))
{
	if (Rhs > 0)
	{
		Scierror(39,_("Incorrect number of input arguments.\n"));
		return 0;
	}

	if (Lhs != 2)
	{
		Scierror(41,_("Incompatible number of output parameter (LHS).\n"));
		return 0;
	}

 	C2F(where)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
