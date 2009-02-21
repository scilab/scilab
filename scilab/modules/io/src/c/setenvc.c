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
#include "stack-def.h"
#include "setenvc.h"
#include "../../tclsci/includes/setenvtcl.h"
#include "MALLOC.h" /* MALLOC */
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static int UpdateEnvVar = 0;
/*--------------------------------------------------------------------------*/
BOOL setenvc(char *stringIn,char *valueIn)
{
	char *string = NULL;
	char *value = NULL;
	BOOL ret = TRUE;

	char szTemp1[bsiz];
	char szTemp2[bsiz];

	string = UTFToLocale(stringIn, szTemp1);
	value = UTFToLocale(valueIn, szTemp2);

#ifdef _MSC_VER
	{
		int len_env = 0; 
		/*
		On Windows :
		each process has two copies of the environment variables,
		one managed by the OS and one managed by the C library. We set
		the value in both locations, so that other software that looks in
		one place or the other is guaranteed to see the value.
		*/
		#define ENV_FORMAT "%s=%s"
		if (SetEnvironmentVariableA(string,value) == 0) return FALSE;
		len_env = (int) (strlen(string) + strlen(value) + strlen(ENV_FORMAT)) + 1;
		if (len_env < _MAX_ENV)
		{
			char *env = (char*) MALLOC(len_env * sizeof(char));
			if (env)
			{
				sprintf(env,"%s=%s",string,value);
				if ( _putenv(env) ) ret = FALSE;
			}
		}
	}
#else
	/* linux and Mac OS X */
	/* setenv() function is strongly preferred to putenv() */
	/* http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/setenv.3.html */
	if ( setenv(string,value,1) ) ret = FALSE;
#endif

	if (ret)
	{
		UpdateEnvVar = 1;
		setenvtcl(stringIn,valueIn);
	}

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
	UpdateEnvVar = val;
}
/*--------------------------------------------------------------------------*/
