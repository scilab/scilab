/* Allan CORNET */
/* INRIA 2005 */

#include "SciEnv.h"
/********************************************************************************************************/
/* Les variables d'environnements SCI,TCL_LIBRARY,TK_LIBRARY */
/* sont définies directement dans scilex */
/* scilex peut donc etre executé seul */
/********************************************************************************************************/
void SciEnv ()
{
	char *p;
	char modname[MAX_PATH + 1];

	if (!GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), modname, MAX_PATH)) return;
	if ((p = strrchr (modname, '\\')) == NULL)return;
	*p = 0;

	/* Set SCI variable */
	if ((p = strrchr (modname, '\\')))
	{
		*p = 0;
		for (p = modname ; *p; p++)
		{
			if (*p == '\\')	*p = '/';
		}

		p = modname;

		set_sci_env(p);
	}
}
/*----------------------------------------------------
* set env variables (used when calling scilab from
* other programs)
*----------------------------------------------------*/
void set_sci_env(char *p)
{
	char *environmentTemp=NULL;
	char *CopyOfp=NULL;
	char env[MAX_PATH + 1 + 10];

	if (p)
	{
		CopyOfp=(char*)malloc(sizeof(char)*(strlen(p)+1));
	}
	else
	{
		/* Error */
		exit(1);
	}

	/* SCI variable Environment */
	if ((environmentTemp = getenv ("SCI")) == (char *) 0)
	{
		if ( GetVersion() < 0x80000000 ) /* Windows NT */
		{
			char ShortPath[MAX_PATH+1];
			/* to be sure that it's unix format */
			/* c:/progra~1/scilab-3.1 */
			ConvertPathWindowsToUnixFormat(p,CopyOfp);
			GetShortPathName(CopyOfp ,ShortPath,MAX_PATH);
			sprintf (env, "SCI=%s",ShortPath);
		}
		else
		{
			/* Win32s, Win95,Win98,WinME */
			ConvertPathUnixToWindowsFormat(p,CopyOfp);
			/* to be sure that it's windows format 8.3 */
			/* c:\progra~1\scilab-3.1 */
			sprintf (env, "SCI=%s",CopyOfp);
		}

		_putenv (env);
	}

	/* HOME variable Environment */
	sprintf (env, "HOME=%s",CopyOfp);
	putenv (env);
		
	/* TK_LIBRARY variable Environment */
	if ((environmentTemp = getenv ("TK_LIBRARY")) == (char *) 0)
	{
		sprintf (env, "TK_LIBRARY=%s\\tcl\\tk8.4", CopyOfp);
		putenv (env);
	}

	/* TCL_LIBRARY variable Environment */
	if ((environmentTemp = getenv ("TCL_LIBRARY")) == (char *) 0)
	{
		sprintf (env, "TCL_LIBRARY=%s\\tcl\\tcl8.4", CopyOfp);
		putenv (env);
	}

	/* COMPILER variable Environment */
	#ifdef __MSC__
	putenv ("COMPILER=VC++");
	#endif
	#if (defined __CYGWIN32__ ) || (defined __MINGW32__)
	putenv ("COMPILER=gcc");
	#endif
	#ifdef __ABSC__
	putenv ("COMPILER=ABSOFT");
	#endif

	/* WIN32 variable Environment */
    #ifdef _WIN32
	putenv ("WIN32=OK");
	#endif

	/* WIN64 variable Environment */
    #ifdef _WIN64
	putenv ("WIN64=OK");
	#endif

/* Add lcc to path */
	if ((environmentTemp = getenv ("PATH")) == (char *) 0)
	{
		MessageBox(NULL,"No PATH environment ...","Error",MB_ICONWARNING);
		exit(1);
	}
	else
	{
		char *NewPath=NULL;
		char *PathWsci= getenv ("SCI");

		if ( PathWsci == (char *)0 )
		{
			MessageBox(NULL,"SCI not defined","Error",MB_ICONWARNING);
			exit(1);
		}
		else
		{
			char PathsLCC[1024];
			char LCCFILE[MAX_PATH];

			wsprintf(LCCFILE,"%s%s",PathWsci,LCCEXE);
			if ( IsAFile(LCCFILE) )
			{
			wsprintf(PathsLCC,"%s%s;%s%s;%s%s",PathWsci,LCCBIN,PathWsci,LCCINCLUDE,PathWsci,LCCLIB);
			NewPath=(char*)malloc( (strlen("PATH=;;")+strlen(environmentTemp)+strlen(PathsLCC)+1)*sizeof(char));
			wsprintf(NewPath,"PATH=%s;%s;",environmentTemp,PathsLCC);
			putenv (NewPath);

			free(NewPath);
			NewPath=NULL;
			}
		}
	
	}

	if (CopyOfp)
	{
		free(CopyOfp);
		CopyOfp=NULL;
	}
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