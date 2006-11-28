/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
#include "gw_core.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(intfort) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_fort) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfort)(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(sci_call) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfort)(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
