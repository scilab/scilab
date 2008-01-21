/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_incremental_link.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scidint) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_addinter) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(scidint)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
