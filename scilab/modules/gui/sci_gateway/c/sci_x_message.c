/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(scimess) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_x_message) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(scimess)(fname,fname_len);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
