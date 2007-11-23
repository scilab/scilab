/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
//extern int C2F(scidellm) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_delmenu_old) _PARAMS((char *fname,unsigned long fname_len))
{
	//C2F(scidellm)(fname,fname_len);
	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
