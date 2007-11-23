/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
//extern int C2F(scimess) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len))
{
	//C2F(scimess)(fname,fname_len);
	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
