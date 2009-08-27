/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "SetScilabEnvironmentVariables.h"
#include "PATH_MAX.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "setgetSCIpath.h"
#include "getScilabDirectory.h"
#include "scilabDefaults.h"
#include "ConvertSlash.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static BOOL IsTheGoodShell(void);
static BOOL Set_Shell(void);
static BOOL Set_SCI_PATH(char *DefaultPath);
static BOOL Set_HOME_PATH(char *DefaultPath);
static BOOL AddScilabBinDirectoryToPATHEnvironnementVariable(char *DefaultPath);
static BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
/*--------------------------------------------------------------------------*/
/**
* Set some environment variablesSCI, and some others
*/
void SciEnvForWindows(void)
{
	char *SCIPathName=NULL;

	SCIPathName = getScilabDirectory(TRUE);

	/* Correction Bug 1579 */
	if (!IsTheGoodShell()) 
	{
		if ( (!Set_Shell()) || (!IsTheGoodShell()))
		{
			MessageBox(NULL,
				"Please modify ""ComSpec"" environment variable.\ncmd.exe on W2K and more.",
				"Warning",MB_ICONWARNING|MB_OK);
		}
	}

	SetScilabEnvironmentVariables(SCIPathName);
	if (SCIPathName) {FREE(SCIPathName);SCIPathName=NULL;}
}
/*--------------------------------------------------------------------------*/
/* set env variables (used when calling scilab from * other programs) */
void SetScilabEnvironmentVariables(char *DefaultSCIPATH)
{
	if (DefaultSCIPATH)
	{

		Set_SCI_PATH(DefaultSCIPATH);
		Set_HOME_PATH(DefaultSCIPATH);
		Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB();
		AddScilabBinDirectoryToPATHEnvironnementVariable(DefaultSCIPATH);
	}
	else
	{
		/* Error */
		exit(1);
	}

}
/*--------------------------------------------------------------------------*/
BOOL Set_SCI_PATH(char *DefaultPath)
{
	BOOL bOK = FALSE;
	char env[PATH_MAX + 1 + 10];
	char ShortPath[PATH_MAX+1];
	char *CopyOfDefaultPath = NULL;

	CopyOfDefaultPath = (char*) MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));
	if (CopyOfDefaultPath)
	{
		/* to be sure that it's unix 8.3 format */
		/* c:/progra~1/scilab-5.0 */
		GetShortPathName(DefaultPath,ShortPath,PATH_MAX);
		AntislashToSlash(ShortPath,CopyOfDefaultPath);

		sprintf (env, "SCI=%s",ShortPath);
		setSCIpath(ShortPath);

		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}

		if (_putenv (env))
		{
			bOK=FALSE;
		}
		else
		{
			bOK=TRUE;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL Set_HOME_PATH(char *DefaultPath)
{
	wchar_t *wHOME = _wgetenv(L"HOME");
	if (wHOME == NULL)
	{
		wchar_t *wUserProfile = _wgetenv(L"USERPROFILE");
		if (wUserProfile)
		{
			return SetEnvironmentVariableW(L"HOME", wUserProfile);
		}
		else
		{
			/* if USERPROFILE is not defined , we use default profile */
			wchar_t *wAllUsersProfile = _wgetenv(L"ALLUSERSPROFILE");
			if (wAllUsersProfile)
			{
				return SetEnvironmentVariableW(L"HOME", wUserProfile);
			}
			else
			{
				BOOL bRes = FALSE;
				wchar_t *wDefault = to_wide_string(DefaultPath);
				if (wDefault)
				{
					bRes = SetEnvironmentVariableW(L"HOME", wDefault);
					FREE(wDefault);
					wDefault = NULL;
				}
				return bRes;
			}
		}
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void)
{
	BOOL bOK=TRUE;

#ifdef _MSC_VER
	_putenv ("COMPILER=VC++");
#endif

	/* WIN32 variable Environment */
#ifdef _WIN32
	_putenv ("WIN32=OK");
#endif

	/* WIN64 variable Environment */
#ifdef _WIN64
	_putenv ("WIN64=OK");
#endif

	if ( GetSystemMetrics(SM_REMOTESESSION) ) 
	{
		_putenv ("SCILAB_MSTS_SESSION=OK");
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL IsTheGoodShell(void)
{
	BOOL bOK=FALSE;
	char shellCmd[PATH_MAX];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	strcpy(shellCmd,"");
	strcpy(fname,"");
	GetEnvironmentVariable("ComSpec", shellCmd, PATH_MAX);
	_splitpath(shellCmd,drive,dir,fname,ext);

	if (_stricmp(fname,"cmd")==0) bOK=TRUE;

	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL Set_Shell(void)
{
	BOOL bOK = FALSE;
	char env[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+_MAX_EXT+10];
	char *WINDIRPATH = NULL;

	WINDIRPATH = getenv ("SystemRoot");
	sprintf(env,"ComSpec=%s\\system32\\cmd.exe",WINDIRPATH);

	if (_putenv (env))
	{
		bOK=FALSE;		
	}
	else
	{
		bOK=TRUE;
	}

	if (WINDIRPATH){ FREE(WINDIRPATH); WINDIRPATH=NULL; }
	return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL AddScilabBinDirectoryToPATHEnvironnementVariable(char *DefaultPath)
{
	#define PATH_FORMAT "PATH=%s/bin;%s"

	BOOL bOK = FALSE;
	char *PATH = NULL;
	char *env = NULL;

	PATH = getenv("PATH");

	env = (char*) MALLOC(sizeof(char)* (strlen(PATH_FORMAT) + strlen(PATH) + strlen(DefaultPath) + 1));
	if (env)
	{
		sprintf(env, PATH_FORMAT, DefaultPath, PATH);
		if (_putenv (env))
		{
			bOK = FALSE;
		}
		else bOK = TRUE;
		FREE(env); env = NULL;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
