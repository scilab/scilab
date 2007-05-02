/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "SciEnv.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "win_mem_alloc.h" /* MALLOC */
#include "setgetSCIpath.h"
/*-----------------------------------------------------------------------------------*/
#define putenv _putenv
/*-----------------------------------------------------------------------------------*/
/**
* Les variables d'environnements SCI,TCL_LIBRARY,TK_LIBRARY
* sont définies directement dans scilab
* scilex peut donc etre executé seul 
*/
void SciEnvForWindows(void)
{
	char *SCIPathName=NULL;

	SCIPathName=GetScilabDirectory(TRUE);

	/* Correction Bug 1579 */
	if (!IsTheGoodShell()) 
	{
		if ( (!Set_Shell()) || (!IsTheGoodShell()))
		{
			MessageBox(NULL,MSG_SCIMSG121,MSG_WARNING22,MB_ICONWARNING|MB_OK);
		}
	}

	set_sci_env(SCIPathName);
	if (SCIPathName) {FREE(SCIPathName);SCIPathName=NULL;}
}
/*-----------------------------------------------------------------------------------*/
/*----------------------------------------------------
* set env variables (used when calling scilab from
* other programs)
*----------------------------------------------------*/
void set_sci_env(char *DefaultSCIPATH)
{
	if (DefaultSCIPATH)
	{
		Set_SCI_PATH(DefaultSCIPATH);
		Set_HOME_PATH(DefaultSCIPATH);
		Set_TCL_LIBRARY_PATH(DefaultSCIPATH);
		Set_TK_LIBRARY_PATH(DefaultSCIPATH);
		Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB();
	}
	else
	{
		/* Error */
		exit(1);
	}

}
/*-----------------------------------------------------------------------------------*/
char *GetScilabDirectory(BOOL UnixStyle)
{
	LPSTR ScilabModuleName=NULL;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	char *SciPathName=NULL;
	char *DirTmp=NULL;

	ScilabModuleName = (LPSTR) MALLOC (MAXSTR + 1);

	if (!GetModuleFileName ((HINSTANCE)GetModuleHandle(MSG_SCIMSG9), (LPSTR) ScilabModuleName, MAXSTR))
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
/*-----------------------------------------------------------------------------------*/
BOOL ConvertPathWindowsToUnixFormat(char *pathwindows,char *pathunix)
{
	BOOL bOK=TRUE;
	if ( (pathunix) && (pathwindows) )
	{
		int i=0;
		strcpy(pathunix,pathwindows);
		for (i=0;i<(int)strlen(pathunix);i++)
		{
			if (pathunix[i]=='\\') pathunix[i]='/';
		}
	}
	else bOK=FALSE;

	return bOK;

}
/*-----------------------------------------------------------------------------------*/
BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows)
{
	BOOL bOK=TRUE;
	if ( (pathunix) && (pathwindows) )
	{
		int i=0;
		strcpy(pathwindows,pathunix);
		for (i=0;i<(int)strlen(pathwindows);i++)
		{
			if (pathwindows[i]=='/') pathwindows[i]='\\';
		}
	}
	else bOK=FALSE;

	return bOK;

}
/*-----------------------------------------------------------------------------------*/
BOOL Set_SCI_PATH(char *DefaultPath)
{
	BOOL bOK=FALSE;
	char env[MAX_PATH + 1 + 10];
	char *GetSCIpath=NULL;

	GetSCIpath=getenv ("SCI");

	if (GetSCIpath) 
	{
		char ShortPath[MAX_PATH+1];
		char *CopyOfDefaultPath=NULL;
		CopyOfDefaultPath=MALLOC(((int)strlen(GetSCIpath)+1)*sizeof(char));

		if (GetShortPathName(GetSCIpath,ShortPath,MAX_PATH) == 0)
		{
			fprintf(stderr,"\n%s%s%s.\n",MSG_ERROR19,MSG_ERROR81,DefaultPath);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
			CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

			/* to be sure that it's unix format */
			/* c:/progra~1/scilab-3.1 */
			GetShortPathName(DefaultPath,ShortPath,MAX_PATH);
			ConvertPathWindowsToUnixFormat(ShortPath,CopyOfDefaultPath);
			sprintf (env, "SCI=%s",ShortPath);
			setSCIpath(ShortPath);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
			
		}
		else
		{
			ConvertPathWindowsToUnixFormat(ShortPath,CopyOfDefaultPath);
			sprintf (env, "SCI=%s",CopyOfDefaultPath);
			setSCIpath(CopyOfDefaultPath);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
		}
	}
	else
	{
		char ShortPath[MAX_PATH+1];
		char *CopyOfDefaultPath=NULL;
		CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

		/* to be sure that it's unix format */
		/* c:/progra~1/scilab-3.1 */
		GetShortPathName(DefaultPath,ShortPath,MAX_PATH);
		ConvertPathWindowsToUnixFormat(ShortPath,CopyOfDefaultPath);
		sprintf (env, "SCI=%s",ShortPath);
		setSCIpath(ShortPath);
		
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
/*-----------------------------------------------------------------------------------*/
BOOL Set_HOME_PATH(char *DefaultPath)
{
	BOOL bOK=FALSE;
	char env[MAX_PATH + 1 + 10];
	char *GetHOMEpath=NULL;

	GetHOMEpath=getenv ("HOME");

	if (GetHOMEpath) 
	{
		char ShortPath[MAX_PATH+1];
		char *CopyOfDefaultPath=NULL;
		CopyOfDefaultPath=MALLOC(((int)strlen(GetHOMEpath)+1)*sizeof(char));
		if (GetShortPathName(GetHOMEpath,ShortPath,MAX_PATH)==0)
		{
			fprintf(stderr,"\n%s%s\n",MSG_ERROR21,MSG_ERROR82);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
			CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

			/* to be sure that it's unix format */
			/* c:/progra~1/scilab-3.1 */
			GetShortPathName(DefaultPath,ShortPath,MAX_PATH);
			ConvertPathWindowsToUnixFormat(ShortPath,CopyOfDefaultPath);
			sprintf (env, "HOME=%s",ShortPath);

			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
			
		}
		else
		{
			/* to be sure that it's unix format */
			/* c:/progra~1/scilab-3.1 */
			ConvertPathWindowsToUnixFormat(ShortPath,CopyOfDefaultPath);
			sprintf (env, "HOME=%s",CopyOfDefaultPath);
			if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
		}
	}
	else
	{
		char ShortPath[MAX_PATH+1];
		char *CopyOfDefaultPath=NULL;
		CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

		/* to be sure that it's unix format */
		/* c:/progra~1/scilab-3.1 */
		GetShortPathName(DefaultPath,ShortPath,MAX_PATH);
		ConvertPathWindowsToUnixFormat(ShortPath,CopyOfDefaultPath);
		sprintf (env, "HOME=%s",ShortPath);
		
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
/*-----------------------------------------------------------------------------------*/
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

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsTheGoodShell(void)
{
	BOOL bOK=FALSE;
	char shellCmd[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	strcpy(shellCmd,"");
	strcpy(fname,"");
	GetEnvironmentVariable("ComSpec", shellCmd, _MAX_PATH);
	_splitpath(shellCmd,drive,dir,fname,ext);

	if (_stricmp(fname,"cmd")==0) bOK=TRUE;
	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
