/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "setenvc.h"
#ifdef WITH_TK
#include "../../tclsci/includes/setenvtcl.h"
#endif
#include "MALLOC.h" /* MALLOC */

#ifdef _MSC_VER
#define putenv _putenv
static char *env=NULL;
#endif

static int UpdateEnvVar=0;
/*-----------------------------------------------------------------------------------*/
int setenvc(char *string,char *value)
{
  int ret=0;
  char* env;

#ifdef LINUX
  if ( setenv(string,value,1) ) ret=FALSE;
  else 
    {
      ret=TRUE;
      UpdateEnvVar=1;
    }
#else /* others HP Solaris WIN32*/
  env=(char*)MALLOC((strlen(string)+strlen(value)+2)*sizeof(char));
  sprintf(env,"%s=%s",string,value);
  if ( putenv(env) )
	  { 
		  ret=FALSE;

	  }
  else 
	  {
#ifdef WITH_TK
      setenvtcl(string,value);
      ret=TRUE;
      UpdateEnvVar=1;
#else
      ret=FALSE;
      UpdateEnvVar=0;
#endif
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
int getUpdateEnvVar(void)
{
	return UpdateEnvVar;
}
/*-----------------------------------------------------------------------------------*/
void setUpdateEnvVar(int val)
{
	UpdateEnvVar=val;
}
/*-----------------------------------------------------------------------------------*/
