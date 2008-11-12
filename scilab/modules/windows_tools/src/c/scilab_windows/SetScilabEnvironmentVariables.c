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
/*--------------------------------------------------------------------------*/
#define putenv _putenv
/*--------------------------------------------------------------------------*/
static BOOL IsTheGoodShell(void);
static BOOL Set_Shell(void);
static BOOL Set_SCI_PATH(char *DefaultPath);
static BOOL Set_HOME_PATH(char *DefaultPath);
static BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
/*--------------------------------------------------------------------------*/
/**
* Les variables d'environnements SCI, and some others
* sont définies directement dans scilab
* scilex peut donc etre executé seul 
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
/*----------------------------------------------------
* set env variables (used when calling scilab from
* other programs)
*----------------------------------------------------*/
void SetScilabEnvironmentVariables(char *DefaultSCIPATH)
{
	if (DefaultSCIPATH)
	{
		Set_SCI_PATH(DefaultSCIPATH);
		Set_HOME_PATH(DefaultSCIPATH);
		Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB();
	}
	else
	{
		/* Error */
		exit(1);
	}

}
/*--------------------------------------------------------------------------*/
char *GetScilabDirectory(BOOL UnixStyle)
{
	LPSTR ScilabModuleName=NULL;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	char *SciPathName=NULL;
	char *DirTmp=NULL;

	ScilabModuleName = (LPSTR) MALLOC (MAX_PATH+ 1);

	if (!GetModuleFileName ((HINSTANCE)GetModuleHandle("LibScilab"), (LPSTR) ScilabModuleName, MAX_PATH))
	{
		if (ScilabModuleName) {FREE(ScilabModuleName);ScilabModuleName=NULL;}
		return NULL;
	}

	_splitpath(ScilabModuleName,drive,dir,fname,ext);
	if (ScilabModuleName) {FREE(ScilabModuleName);ScilabModuleName=NULL;}
	if (dir[strlen(dir)-1] == '\\') dir[strlen(dir)-1] = '\0';

	DirTmp=strrchr (dir, '\\');
	if (strlen(dir)-strlen(DirTmp)>0)
	{
		dir[strlen(dir)-strlen(DirTmp)] = '\0';
	}
	else return NULL;

	SciPathName=(char*)MALLOC((int)(strlen(drive)+strlen(dir)+5)*sizeof(char));
	
	_makepath(SciPathName,drive,dir,NULL,NULL);

	if ( UnixStyle )
	{	
		int i=0;
		for (i=0;i<(int)strlen(SciPathName);i++)
		{
			if (SciPathName[i]=='\\') SciPathName[i]='/';
		}
	}
	SciPathName[strlen(SciPathName)-1]='\0';
	setSCIpath(SciPathName);
	return SciPathName;
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
	BOOL bOK=FALSE;
	char env[PATH_MAX + 1 + 10];
	char *GetHOMEpath=NULL;

	GetHOMEpath=getenv ("HOME");

	if (GetHOMEpath) 
	{
		char ShortPath[PATH_MAX+1];
		char *CopyOfDefaultPath=NULL;
		CopyOfDefaultPath=MALLOC(((int)strlen(GetHOMEpath)+1)*sizeof(char));
		if (GetShortPathName(GetHOMEpath,ShortPath,PATH_MAX)==0)
		{
			fprintf(stderr,"\n%s%s\n","Incorrect HOME path. Please verify your HOME environment variable.\n",
				"HOME has been redefined.");
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
			CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

			/* to be sure that it's unix format */
			/* c:/progra~1/scilab-3.1 */
			GetShortPathName(DefaultPath,ShortPath,PATH_MAX);
			slashToAntislash(ShortPath,CopyOfDefaultPath);
			sprintf (env, "HOME=%s",ShortPath);
			SetEnvironmentVariableA("HOME",ShortPath);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
			
		}
		else
		{
			/* to be sure that it's unix format */
			/* c:/progra~1/scilab-3.1 */
			slashToAntislash(ShortPath,CopyOfDefaultPath);
			sprintf (env, "HOME=%s",CopyOfDefaultPath);
			SetEnvironmentVariableA("HOME",CopyOfDefaultPath);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
		}
	}
	else
	{
		char ShortPath[PATH_MAX+1];
		char *CopyOfDefaultPath=NULL;
		CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

		/* to be sure that it's unix format */
		/* c:/progra~1/scilab-3.1 */
		AntislashToSlash(DefaultPath,CopyOfDefaultPath);
		GetShortPathName(CopyOfDefaultPath,ShortPath,PATH_MAX);
		sprintf (env, "HOME=%s",ShortPath);
		SetEnvironmentVariableA("HOME",ShortPath);
		
		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
	}

	if (_putenv (env))
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void)
{
	BOOL bOK=TRUE;

	#ifdef _MSC_VER
		putenv ("COMPILER=VC++");
	#endif

	/* WIN32 variable Environment */
    #ifdef _WIN32
		putenv ("WIN32=OK");
	#endif

	/* WIN64 variable Environment */
    #ifdef _WIN64
		putenv ("WIN64=OK");
	#endif

	if ( GetSystemMetrics(SM_REMOTESESSION) ) 
	{
		putenv ("SCILAB_MSTS_SESSION=OK");
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
	BOOL bOK=FALSE;
	char env[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+_MAX_EXT+10];
	char *WINDIRPATH=NULL;

	WINDIRPATH=getenv ("SystemRoot");
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
