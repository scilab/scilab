/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scitzer) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_tzer) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_tzer) _PARAMS((char *fname,unsigned long fname_len))
{
	if (Rhs != 4)
	{
		Scierror(42,_("Incompatible input parameter (RHS).\n"));
		return 0;
	}
	if ( (Lhs != 2) && (Lhs !=3) )
	{
		Scierror(41,_("Incompatible output parameter (LHS).\n"));
		return 0;
	}

	C2F(scitzer)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
