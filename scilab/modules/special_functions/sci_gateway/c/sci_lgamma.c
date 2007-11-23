/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_special_functions1.h"
#include "machine.h"
#include "stack-c.h"
#include <string.h>
/*--------------------------------------------------------------------------*/
extern int C2F(intslgamma) _PARAMS((char *id,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_lgamma) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intslgamma)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
