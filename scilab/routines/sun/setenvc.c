/* Allan CORNET INRIA 2005 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *env=NULL;
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
		env=(char*)malloc((strlen(string)+strlen(value)+2)*sizeof(char));
		sprintf(env,"%s=%s",string,value);
		if ( putenv(env) ) ret=FALSE;
		else ret=TRUE;
		#ifdef WIN32
		if (env)
		{
			free(env);
			env=NULL;
		}
        #endif
	#endif

	return ret;
}
/*-----------------------------------------------------------------------------------*/
