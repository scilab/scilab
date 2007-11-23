/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
#include <string.h>
/*--------------------------------------------------------------------------*/
extern int C2F(intree3) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_sci_tree3) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckLhs(2,2);
	CheckRhs(7,7);
	C2F(intree3)(fname,fname_len);
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
