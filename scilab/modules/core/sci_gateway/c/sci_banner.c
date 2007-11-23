/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int C2F(intbanner) _PARAMS((char *fname));
extern void banner(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_banner) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intbanner)(fname);
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(intbanner) _PARAMS((char *fname))
{
	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	banner();

	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*--------------------------------------------------------------------------*/
