#include "ConfigureW.h"

/*********************************
*  Allan CORNET INRIA 2004		 *
**********************************/

/************************************************************************/
int main(int argc, char* argv[])
/* Configure makefile.incl.mak and make-pvm3.mak 
   Verify files of WinConfigure.verif
   Verify Fortran libraries files
   Launch Visual .NET 2003
 */
{
	int mode=-1;
	/*
	mode == 0 Interactif mode
	mode == 1 build debug 
	mode == 2 build release
	mode == 3 clean release and debug
	*/

	if (argc == 2) /* One Parameter */
	{
		if ( strcmp(argv[1],switchInteractif) == 0 ) mode=0;
		if ( strcmp(argv[1],switchDebug) == 0 ) mode=1;
		if ( strcmp(argv[1],switchRelease) == 0 ) mode=2;
		if ( strcmp(argv[1],switchClean) == 0 ) mode=3;

		if (mode == -1)
		{
			help();
			return -1;
		}
	}
	else
	{
		if (argc > 2)
		{
			help();
			return -1;
		}
		else // no parameter
		{
			 mode=0;
		}
	}

	if (SearchVSNET2003() == 0 )
	{
		if ( VerifFiles() == 0 )
		{
			ConfigureMakefiles();
			if ( ConfigureHeaders() == 0)
			{
				
				if ( SearchLibsFortran() > 0)
				{
					printf("\n");
					printf("%s,\n%s,\n%s,\n%s are not found in \n%s\n",FortranLib1,FortranLib2,FortranLib3,FortranLib4,PathLibFortran);
					printf("Please change path of Fortran Libraries in LibScilab Project.\nBefore Build.\n");
					MessageBox(NULL,"Please change path of Fortran Libraries in LibScilab Project.\nBefore Build.","Warning",MB_OK);
					RunVisualDotNETProject(0);
					return -1;
				}
				else
				{
					RunVisualDotNETProject(mode);
				}
			}
			else
			{
				char LineTmp[MAX_PATH];
				wsprintf(LineTmp,"Please verify these files :\n %s ,\n %s,\n %s",ScicosVCHeader,MachineVCHeader,VersionVCHeader);
				printf("\n%s\n",LineTmp);
				MessageBox(NULL,LineTmp,"Warning",MB_OK);
			}
				
		}
	}
	else
	{
		char LineTmp[MAX_PATH];
		wsprintf(LineTmp,"Please execute WinConfigure.exe from Visual.NET 2003 Command Prompt");
		printf("\n%s\n",LineTmp);
		MessageBox(NULL,LineTmp,"File not found !!!",MB_ICONWARNING);
	}

	return 0;
}

/************************************************************************/
int VerifFiles(void)
{
	FILE *fileR;
	int Retour=0;
	char Line[MAX_PATH];
	char LineTmp[MAX_PATH];

	fileR= fopen(VERIF, "rt");
	if (!fileR)
	{
		wsprintf(LineTmp,"Don't find %s \n Please correct this !",VERIF);
		printf("\n%s\n",LineTmp);
		MessageBox(NULL,LineTmp,"File not found !!!",MB_ICONWARNING);
		return 1;
	}
	while( fgets(Line, MAX_PATH, fileR) != NULL)
	{ 
		if ( Line[strlen(Line)-1] == '\n' ) Line[strlen(Line)-1]='\0';
		if ( Line[strlen(Line)-1] == '\r' ) Line[strlen(Line)-1]='\0';

		if ( ! IsAFile(Line))
			{
				wsprintf(LineTmp,"Don't find %s \n Please correct this !",Line);
				printf("\n%s\n",LineTmp);
				MessageBox(NULL,LineTmp,"File not found !!!",MB_ICONWARNING);
				return 1;
			}
    }
			
    fclose(fileR);
	return Retour;
}
/************************************************************************/
BOOL IsAFile(char *chainefichier)
{
    BOOL retour=FALSE;	

	FILE *fileR;
		
	fileR= fopen(chainefichier, "rt");
	if (fileR)
		{
			fclose(fileR);
			retour=TRUE;
		}

    return retour;

}

/************************************************************************/
void ExecuteCommand(char *cmdline,BOOL WaitEnd)
{
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    
    if( !CreateProcess( NULL, // No module name (use command line). 
        cmdline, // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
        printf("CreateProcess failed. %s \n",cmdline );
    }

    // Wait until child process exits.
    if (WaitEnd) WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

}

/************************************************************************/
int ConfigureMakefiles(void)
{
	ConfigureMakefileIncl();	
	printf("\n %s is up to date.\n",MakefileInclMak);

	ConfigureMakefilePVM3();
	printf("\n %s is up to date.\n",MakefilePVM3);

	return 0;
}
/************************************************************************/
int ConfigureMakefilePVM3(void)
{
	char Path[MAX_PATH];
	char Line[MAX_PATH];
	char ShortPath[MAX_PATH];
	BOOL SpaceInPath=FALSE;
	unsigned int i=0;

	GetCurrentDirectory(MAX_PATH,Path);
	GetShortPathName(Path,ShortPath,MAX_PATH);

	for(i=0;i<strlen(Path);i++)
		{
			if (Path[i]==' ') SpaceInPath=TRUE;
		}
	
	strcpy(Line,"PVM_ROOT=");
	if ( SpaceInPath )strcat(Line,ShortPath);
	else strcat(Line,Path);

	strcat(Line,"\\pvm3\n");
	ModifyFile(MakefilePVM3,"PVM_ROOT=",Line);
	DeleteFile(MakefilePVM3);
	MoveFile(MakefilePVM3TMP,MakefilePVM3);

	return 0;
}
/************************************************************************/
int ConfigureMakefileIncl(void)
{
    char Path[MAX_PATH];
	char Line[MAX_PATH];
	char ShortPath[MAX_PATH];
	BOOL SpaceInPath=FALSE;
	unsigned int i=0;

	GetCurrentDirectory(MAX_PATH,Path);
	GetShortPathName(Path,ShortPath,MAX_PATH);

	for(i=0;i<strlen(Path);i++)
		{
			if (Path[i]==' ') SpaceInPath=TRUE;
		}

	strcpy(Line,"TCLTK=");
	if ( SpaceInPath )strcat(Line,ShortPath);
	else strcat(Line,Path);

	strcat(Line,"\\tcl\n");
	ModifyFile(MakefileInclMak,"TCLTK=",Line);
	DeleteFile(MakefileInclMak);
	MoveFile(MakefileInclMakTMP,MakefileInclMak);

	strcpy(Line,"PVM_ROOT=");
	if ( SpaceInPath )strcat(Line,ShortPath);
	else strcat(Line,Path);

	strcat(Line,"\\pvm3\n");
	ModifyFile(MakefileInclMak,"PVM_ROOT=",Line);
	DeleteFile(MakefileInclMak);
	MoveFile(MakefileInclMakTMP,MakefileInclMak);

	return 0;
}
/************************************************************************/
int SearchVSNET2003(void)
{
	char *env=NULL;
	char *filepart=NULL;
	int len=0;

	strcpy(VC7Path,"empty");
	len=SearchPath(NULL,MSVC71EXE,NULL,MAX_PATH,VC7Path,&filepart);

	env = getenv (ENVMSVC71);

	if (env == NULL) return 1;

    if (strcmp(VC7Path,"empty") == 0) return 1;

	return 0;

}
/************************************************************************/
int RunVisualDotNETProject(int mode)
{
	if (SearchVSNET2003() == 0 )
	{
		switch (mode)
		{
		/* mode == 0 Interactif mode
		   mode == 1 build debug 
		   mode == 2 build release
	       mode == 3 clean release and debug */

		default: case 0: 
			strcat(VC7Path,MSVCPROJECT);
			ExecuteCommand(VC7Path,FALSE);
			break;
		case 1:
			strcat(VC7Path,MSVCPROJECT);
			strcat(VC7Path,DebugMode);
			printf("\n\nPlease execute :\n");
			printf("%s%s%s\n","devenv",MSVCPROJECT,DebugMode);
			// ExecuteCommand(VC7Path,FALSE);
			break;
		case 2:
			strcat(VC7Path,MSVCPROJECT);
			strcat(VC7Path,ReleaseMode);
			printf("\n\nPlease execute :\n");
			printf("	%s%s%s\n","devenv",MSVCPROJECT,ReleaseMode);
			// ExecuteCommand(VC7Path,FALSE);
			break;
		case 3:
			{
				char VC7PathTemp[MAX_PATH];
				strcpy(VC7PathTemp,VC7Path);

				strcat(VC7Path,MSVCPROJECT);
				strcat(VC7Path,CleanReleaseMode);
				printf("\n\nPlease execute :\n");
				printf("	%s%s%s\n","devenv",MSVCPROJECT,CleanReleaseMode);
				// ExecuteCommand(VC7Path,TRUE);

				strcat(VC7PathTemp,MSVCPROJECT);
				strcat(VC7PathTemp,CleanDebugMode);
				printf("	or %s%s%s\n\n","devenv",MSVCPROJECT,CleanDebugMode);
				// ExecuteCommand(VC7PathTemp,FALSE);

			}
			break;
		
		}
	}
	else return 1;
	return 0;
}
/************************************************************************/
int ModifyFile(char *fichier,char *motclef,char *chaine)
{
		int Retour=1;
		FILE *fileR,*fileW;
		char Ligne[MAX_PATH];
		char cmpchaine[MAX_PATH];
		
		fileR= fopen(fichier, "rt");
		strcpy(cmpchaine,fichier);
		strcat(cmpchaine,".tmp");
		fileW= fopen(cmpchaine, "wt");
		strcpy(cmpchaine,"");

		while( fgets(Ligne, MAX_PATH, fileR) != NULL)
		{ 
			strncpy(cmpchaine,Ligne,strlen(motclef));
			cmpchaine[strlen(motclef)]='\0';
			if (strcmp(cmpchaine,motclef)==0)
			{
				fputs(chaine,fileW);
				Retour=0;
			}
			else
			{
				fputs(Ligne,fileW);
			}
      		strcpy(Ligne,"\0");
      	}
			
    	fclose(fileR);
		fclose(fileW);
		return Retour;
}
/************************************************************************/
int SearchLibsFortran(void)
{
	int Retour=0;
	char Line[MAX_PATH];

	strcpy(Line,PathLibFortran);
	strcat(Line,FortranLib1);

	if ( !IsAFile(Line) ) Retour++;

	strcpy(Line,PathLibFortran);
	strcat(Line,FortranLib2);

	if ( !IsAFile(Line) ) Retour++;

	strcpy(Line,PathLibFortran);
	strcat(Line,FortranLib3);

	if ( !IsAFile(Line) ) Retour++;

	strcpy(Line,PathLibFortran);
	strcat(Line,FortranLib4);

	if ( !IsAFile(Line) ) Retour++;

	return Retour;
}
/************************************************************************/
int ConfigureHeaders(void)
{
	int Retour=0;

	if ( ! CopyFile(ScicosVCHeader,ScicosHeader,FALSE) ) Retour=1;
	if ( ! CopyFile(MachineVCHeader,MachineHeader,FALSE) ) Retour=1;
	if ( ! CopyFile(VersionVCHeader,VersionHeader,FALSE) ) Retour=1;

	return Retour;

}
/************************************************************************/
void help(void)
{
	printf("\n -I just open Visual Studio .NET 2003\n");
	printf("\n -C Clean Scilab Project\n");
	printf("\n -D Build in Debug mode\n");
	printf("\n -R Build in Release mode\n\n");
}
/************************************************************************/