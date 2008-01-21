/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_incremental_link.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scilink) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_link) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(scilink)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
