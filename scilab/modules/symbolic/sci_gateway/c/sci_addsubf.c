/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_symbolic.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(sciaddfsubf) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_addf) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(sciaddfsubf)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(sci_subf) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(sciaddfsubf)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
