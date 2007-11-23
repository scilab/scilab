/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "machine.h"

extern int C2F(intgetfield)();

int C2F(sci_getfield) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_getfield) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intgetfield)();
	return 0;
}
/*--------------------------------------------------------------------------*/
