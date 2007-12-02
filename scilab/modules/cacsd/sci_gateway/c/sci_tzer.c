/*--------------------------------------------------------------------------*/
/* INRIA 2006/2007 */
/* @author Allan CORNET */
/* @author Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_cacsd2.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scitzer) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_tzer) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(4,4);
	CheckLhs(2,3);
	C2F(scitzer)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
