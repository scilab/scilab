/* Allan CORNET INRIA 2005 */
#include <stdlib.h>
/*-----------------------------------------------------------------------------------*/
/* returns 0 if there is a problem else 1 */
int setenvc(char *envstring)
{
	#define TRUE  1
	#define FALSE 0

	int ret=0;

	if ( putenv (envstring) ) ret=FALSE;
	else ret=TRUE;

	return ret;
}
/*-----------------------------------------------------------------------------------*/
