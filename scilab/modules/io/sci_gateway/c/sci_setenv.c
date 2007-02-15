/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
#include "gw_io.h"
#include "setenvc.h"
#include "intsetenv.h"
#include "sci_setenv.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_setenv) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intsetenv)(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
