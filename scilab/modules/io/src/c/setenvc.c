/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "machine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "setenvc.h"
#include "../../tclsci/includes/setenvtcl.h"
#include "MALLOC.h" /* MALLOC */

#ifdef _MSC_VER
#define putenv _putenv
static char *env=NULL;
#endif

static int UpdateEnvVar=0;
/*--------------------------------------------------------------------------*/
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
      setenvtcl(string,value);
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
/*--------------------------------------------------------------------------*/
int getUpdateEnvVar(void)
{
	return UpdateEnvVar;
}
/*--------------------------------------------------------------------------*/
void setUpdateEnvVar(int val)
{
	UpdateEnvVar=val;
}
/*--------------------------------------------------------------------------*/
