/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "machine.h"

extern int C2F(intsetfield)();

int C2F(sci_setfield) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_setfield) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intsetfield)();
	return 0;
}
/*--------------------------------------------------------------------------*/
