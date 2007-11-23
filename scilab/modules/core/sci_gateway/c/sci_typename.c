/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(inttypnam) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_typename) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(inttypnam)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
