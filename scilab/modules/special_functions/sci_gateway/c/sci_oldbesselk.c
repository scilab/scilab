/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_special_functions1.h"
#include "machine.h"
#include "stack-c.h"
#include <string.h>
/*--------------------------------------------------------------------------*/
extern int C2F(intsbesselk) _PARAMS((char *id,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_oldbesselk) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intsbesselk)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
