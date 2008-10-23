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

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "setenvc.h"
#include "../../tclsci/includes/setenvtcl.h"
#include "MALLOC.h" /* MALLOC */

#ifdef _MSC_VER
#define putenv _putenv
static char *env = NULL;
#endif

static int UpdateEnvVar=0;
/*--------------------------------------------------------------------------*/
int setenvc(char *string,char *value)
{
	int ret = 0;
	/* 2 is = and \0 */

	char *env = (char*)MALLOC((strlen(string)+strlen(value)+2)*sizeof(char));

#ifdef _MSC_VER

	/* 
	 On Windows :
     each process has two copies of the environment variables,
     one managed by the OS and one managed by the C library. We set
     the value in both locations, so that other software that looks in
     one place or the other is guaranteed to see the value.
	*/
	SetEnvironmentVariableA(string,value);
#endif

#ifdef linux	/* @TODO Check where stands Mac OS X */
	if ( setenv(string,value,1) ) {
#else /* others HP Solaris WIN32*/
	sprintf(env,"%s=%s",string,value);
	if ( putenv(env) ) {
#endif

		ret = FALSE;
	}
	else 
	{
		UpdateEnvVar = 1;
		ret = TRUE;
    }

	if (ret) {
		setenvtcl(string,value);
	}


	FREE(env);

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
