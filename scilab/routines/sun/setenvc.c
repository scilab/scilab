/* Allan CORNET INRIA 2005 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char env[4096];
/*-----------------------------------------------------------------------------------*/
/* returns 0 if there is a problem else 1 */
int setenvc(char *string,char *value)
{
	#define TRUE  1
	#define FALSE 0

	int ret=0;

	#if linux
		if ( setenv(string,value,1) ) ret=FALSE;
		else ret=TRUE;
	#else /* others HP Solaris WIN32*/
		sprintf(env,"%s=%s",string,value);
		if ( putenv(env) ) ret=FALSE;
		else ret=TRUE;
	#endif

	return ret;
}
/*-----------------------------------------------------------------------------------*/
