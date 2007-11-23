/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_special_functions1.h"
#include "machine.h"
#include "stack-c.h"
#include <string.h>
/*--------------------------------------------------------------------------*/
extern int C2F(intsdlgamma) _PARAMS((char *id,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_dlgamma) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intsdlgamma)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
