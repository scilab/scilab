/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_console.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "completion.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_completion) _PARAMS((char *fname,unsigned long fname_len))
{

	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
