/* Allan CORNET INRIA 2005 */
#include "../machine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef WITH_TK
  #include "../tclsci/TCL_Global.h"
#endif
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#ifdef WIN32
static char *env=NULL;
#endif

int UpdateEnvVar=0;
/*-----------------------------------------------------------------------------------*/
#ifdef WITH_TK
int setenvtcl(char *string,char *value)
{
	int bOK=FALSE;
	char MyTclCommand[2048];

	sprintf(MyTclCommand,"env(%s)",string);

	if (TCLinterp==NULL) return((int)FALSE);
	
	if ( !Tcl_SetVar(TCLinterp,MyTclCommand, value, TCL_GLOBAL_ONLY) )
	{
		bOK=(int)(FALSE);
	}
	else
	{
		bOK=(int)(TRUE);
	}

	return bOK;
}
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
#ifdef WIN32
  if (env)
    {
      FREE(env);
      env=NULL;
    }
#endif
#endif

  return ret;
}
