/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_incremental_link.h"
#include "machine.h"
#include "stack-c.h"
#include "link.h"
/*--------------------------------------------------------------------------*/
extern int C2F(sciislink) _PARAMS((char *fname,unsigned long fname_len)); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int C2F(sci_c_link) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(sciislink)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
