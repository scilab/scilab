/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scicosdebug) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_scicos_debug) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(scicosdebug)(fname,fname_len);
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
