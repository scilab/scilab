/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
#include <string.h>
/*--------------------------------------------------------------------------*/
extern int C2F(intree4) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_sci_tree4) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckLhs(2,2);
	CheckRhs(5,5);
	C2F(intree4)(fname, fname_len);
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
