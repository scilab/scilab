/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "gw_gui.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
//extern int C2F(scimess1) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_message_modeless) _PARAMS((char *fname,unsigned long fname_len))
{
	//C2F(scimess1)(fname,fname_len);
	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
