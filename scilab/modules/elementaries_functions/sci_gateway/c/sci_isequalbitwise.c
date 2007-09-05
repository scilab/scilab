/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_elementaries_functions.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intisequalvar)(char * fname, int *job, long int fl); /* the gateway */
/*-----------------------------------------------------------------------------------*/
int C2F(sci_isequalbitwise) _PARAMS((char *fname,unsigned long fname_len))
{
	int job= 0;
	C2F(intisequalvar)(fname, (job=0,&job),(unsigned long)strlen(fname) );
	return 0;
}
/*-----------------------------------------------------------------------------------*/
