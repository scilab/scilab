/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
//extern int C2F(scidialog) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
int C2F(sci_x_dialog) _PARAMS((char *fname,unsigned long fname_len))
{
	//C2F(scidialog)(fname,fname_len);
	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
