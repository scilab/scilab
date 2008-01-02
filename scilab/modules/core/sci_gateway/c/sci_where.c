/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(where)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_where) _PARAMS((char *fname,unsigned long fname_len))
{
	if (Rhs > 0)
	{
		Scierror(39,_("%s: Wrong number of input arguments.\n"),"where");
		return 0;
	}

	if (Lhs != 2)
	{
		Scierror(41,_("%s: Wrong number of output arguments: %d expected.\n"),"where",2);
		return 0;
	}

 	C2F(where)();
	return 0;
}
/*--------------------------------------------------------------------------*/
