/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_dynamic_link.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfort) _PARAMS((char *fname,unsigned long fname_len)); /* fortran */
/*--------------------------------------------------------------------------*/
int C2F(sci_fort) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfort)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(sci_call) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfort)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
