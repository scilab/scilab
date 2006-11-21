/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(scichoice) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_xchoicesi) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_xchoicesi) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(scichoice)(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
