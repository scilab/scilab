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

	if (!GetModuleFileName ((HANDLE)GetModuleHandle(NULL), modname + 1, MAX_PATH)) return;
	if ((p = strrchr (modname + 1, '\\')) == NULL)return;
	*p = 0;

	/* Set SCI variable */
	if ((p = strrchr (modname + 1, '\\')))
	{
		*p = 0;
		for (p = modname + 1; *p; p++)
		{
			if (*p == '\\')	*p = '/';
		}

		p = modname + 1;

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

		putenv (env);
	}

	/* HOME variable Environment */
	SetHomeDirectory(CopyOfp);
		
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
BOOL SetHomeDirectory(char *DefaultPath)
{
	BOOL bOK=FALSE;
	char *environmentTemp=NULL;
	int Version=GetOSVersion();

	environmentTemp = getenv ("HOME");

	if ( (Version == OS_WIN32_WINDOWS_95) ||
		 (Version == OS_WIN32_WINDOWS_98) ||
		 (Version == OS_WIN32_WINDOWS_Me) ||
		 (Version == OS_WIN32_WINDOWS_NT_3_51) )
	{
		if ( environmentTemp == (char *) 0)
		{
			SetDefaultHome(DefaultPath);
		}
	}
	else /* NT and more ... */
	{
		if (environmentTemp == (char *) 0)
		{
			char *WindowsHomeDirectory=GenerateWindowsHomeDirectory();
			char *ScilabHomeDirectory=GenerateScilabHomeDirectory(WindowsHomeDirectory);
			if ( IsAFile(ScilabHomeDirectory) )
			{
				char *Home=GetScilabHomeDirUnixFormat(ScilabHomeDirectory);
				if (Home)
				{
					SetDefaultHome(Home);
					free(Home);
					Home=NULL;
				}
				else
				{
					SetDefaultHome(DefaultPath);
				}
			}
			else
			{
				if (CreateScilabHomeDirectory(WindowsHomeDirectory))
				{
					char *Home=GetScilabHomeDirUnixFormat(ScilabHomeDirectory);
					if (Home)
					{
						SetDefaultHome(Home);
						free(Home);
						Home=NULL;
					}
					else
					{
						SetDefaultHome(DefaultPath);
					}
				}
				else
				{
					SetDefaultHome(DefaultPath);
				}
			}
			if (WindowsHomeDirectory) { free(WindowsHomeDirectory); WindowsHomeDirectory=NULL; }
			if (ScilabHomeDirectory) { free(ScilabHomeDirectory); ScilabHomeDirectory=NULL; }
		}
		else
		{
			char *Scilab_Creator = getenv ("SCILAB_CREATOR");
			if (Scilab_Creator)
			{
				char *WindowsHomeDirectory=GenerateWindowsHomeDirectory();
				char *ScilabHomeDirectory=GenerateScilabHomeDirectory(WindowsHomeDirectory);
				char *Home=GetScilabHomeDirUnixFormat(ScilabHomeDirectory);
				if (Home)
				{
					SetDefaultHome(Home);
					free(Home);
					Home=NULL;
				}
				else
				{
					SetDefaultHome(DefaultPath);
				}
				if (WindowsHomeDirectory) { free(WindowsHomeDirectory); WindowsHomeDirectory=NULL; }
				if (ScilabHomeDirectory) { free(ScilabHomeDirectory); ScilabHomeDirectory=NULL; }
			}
			else
			{
				SetDefaultHome(DefaultPath);
			}

		}
		
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
char *GenerateWindowsHomeDirectory(void)
{
	char *HomePath=NULL;
	char *PathHomeDrive = (char*) getenv ("HOMEDRIVE");
	char *PathHomePath = (char*) getenv ("HOMEPATH");
		
	HomePath = (char *)malloc( (strlen(PathHomeDrive)+strlen(PathHomePath)+1)*sizeof(char) );
	wsprintf(HomePath,"%s%s",PathHomeDrive,PathHomePath);

	return HomePath;
}
/*-----------------------------------------------------------------------------------*/
char *GenerateScilabHomeDirectory(char *WinHomeDir)
{
	char *SciHomePath=NULL;

	SciHomePath=(char *)malloc( (strlen(WinHomeDir)+strlen("Scilab")+strlen(VERSION)+2)*sizeof(char));
	wsprintf(SciHomePath,"%s\\Scilab\\%s",WinHomeDir,VERSION);

	return SciHomePath;
}
/*-----------------------------------------------------------------------------------*/
BOOL CreateScilabHomeDirectory(char *WinHomeDir)
{
	BOOL bOK=FALSE;
	char *SciHomePath=GenerateScilabHomeDirectory(WinHomeDir);

	if ( IsAFile(SciHomePath) )
	{
		bOK=TRUE;
	}
	else
	{
		char *SciHomeLevel1=NULL;

		SciHomeLevel1=(char*)malloc( (strlen(WinHomeDir)+strlen("\\Scilab")+1)*sizeof(char) );
		wsprintf(SciHomeLevel1,"%s\\Scilab",WinHomeDir);

		if (IsAFile(SciHomeLevel1))
		{
			if (CreateDirectory(SciHomePath,NULL))
			{
				bOK=TRUE;
			}
			else
			{
				bOK=FALSE;
			}
		}
		else
		{
			CreateDirectory(SciHomeLevel1,NULL);
			CreateDirectory(SciHomePath,NULL);

			if (IsAFile(SciHomePath))
			{
				bOK=TRUE;
			}
			else
			{
				bOK=FALSE;
			}
		}

		if (SciHomeLevel1) { free(SciHomeLevel1); SciHomeLevel1=NULL; }
	}
	if (SciHomePath) { free(SciHomePath); SciHomePath=NULL; }
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
char * GetScilabHomeDirUnixFormat(char *ScilabHomeDirectory)
{
	char *UnixFormat=NULL;
	int i=0;

	UnixFormat=(char*)malloc((strlen(ScilabHomeDirectory)+1)*sizeof(char));
	wsprintf(UnixFormat,"%s",ScilabHomeDirectory);
		
	for (i=0;i<(int)strlen(UnixFormat);i++)
	{
		if (UnixFormat[i]=='\\') UnixFormat[i]='/';
	}

	return UnixFormat;
}
/*-----------------------------------------------------------------------------------*/
void SetDefaultHome(char *DefaultPath)
{
	char env[MAX_PATH + 1 + 10];
	wsprintf (env, "HOME=%s",DefaultPath);
	putenv (env);
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