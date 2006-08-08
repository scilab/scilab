/* Allan CORNET INRIA 2005 */
#include "machine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MALLOC.h" /* MALLOC */

#ifdef _MSC_VER
#define putenv _putenv
static char *env=NULL;
#endif

int UpdateEnvVar=0;
/*-----------------------------------------------------------------------------------*/
#ifdef WITH_TK
	extern int setenvtcl(char *string,char *value);
#endif
/*-----------------------------------------------------------------------------------*/
/* returns 0 if there is a problem else 1 */
int setenvc(char *string,char *value)
{
#define TRUE  1
#define FALSE 0

  int ret=0;
  char* env;

#if linux
  if ( setenv(string,value,1) ) ret=FALSE;
  else 
    {
      ret=TRUE;
      UpdateEnvVar=1;
    }
#else /* others HP Solaris WIN32*/
  env=(char*)MALLOC((strlen(string)+strlen(value)+2)*sizeof(char));
  sprintf(env,"%s=%s",string,value);
  if ( putenv(env) ) ret=FALSE;
  else 
    {
#ifdef WITH_TK
      setenvtcl(string,value);
#endif

      ret=TRUE;
      UpdateEnvVar=1;
    }
#ifdef _MSC_VER
  if (env)
    {
      FREE(env);
      env=NULL;
    }
#endif
#endif

  return ret;
}
/*-----------------------------------------------------------------------------------*/
