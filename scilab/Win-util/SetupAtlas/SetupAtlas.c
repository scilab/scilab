/*************************************************************************************************/
/* Allan CORNET */
/* INRIA 2004   */
/*************************************************************************************************/

#include <windows.h>
#include <stdio.h>

#define KeyCpuIdentifer "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define LenLine 255
#define LogName "SetupAtlas.log"
#define LineMaxSize 512
#define MaxCPU 64
#define DefaultDllName "ATLAS_P1.DLL"

char ArrayLinesCPU[MaxCPU][LineMaxSize]; /* 64 cpus max in atlas.spec*/
int NbrLinesCPU=0;

char *GetWhereIsThisExe(void);
/*************************************************************************************************/
BOOL AppendMessageToLog(char *Message)
{
	BOOL bOK=FALSE;
	FILE *pFile;
	char *Path=NULL;
	char PathAndFilename[MAX_PATH];

	Path=GetWhereIsThisExe();
	wsprintf(PathAndFilename,"%s%s",Path,LogName);
	free(Path);
	Path=NULL;

	pFile=fopen(PathAndFilename,"at");
	if (pFile)
	{
		fprintf(pFile,"%s\n",Message);
		fclose(pFile);
		bOK=TRUE;
	}

	return bOK;
}
/*************************************************************************************************/
char * GetRegKeyIdentifier(void)
{
	HKEY key;
	DWORD result;
	char *LineIdentifier;
	ULONG length = LenLine,Type;
	
  	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyCpuIdentifer, 0, KEY_QUERY_VALUE , &key);

	LineIdentifier=(char*)malloc(sizeof(char)*length);

	if ( RegQueryValueEx(key, "Identifier", 0, &Type, (LPBYTE)LineIdentifier, &length) !=  ERROR_SUCCESS )
	{
		wsprintf(LineIdentifier,"ERROR");
	}

	if( Type != REG_SZ )
    {
       wsprintf(LineIdentifier,"ERROR");
    }

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);
	
	return (char *)LineIdentifier;
}
/*************************************************************************************************/
char *GetRegKeyVendorIdentifier(void)
{
	HKEY key;
	DWORD result;
	ULONG length = LenLine,Type;

	char *LineVendorIdentifier;

	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE, KeyCpuIdentifer, 0, KEY_QUERY_VALUE , &key);

	LineVendorIdentifier=(char*)malloc(sizeof(char)*length);

	if ( RegQueryValueEx(key, "VendorIdentifier", 0, &Type, (LPBYTE)LineVendorIdentifier, &length) !=  ERROR_SUCCESS )
	{
		wsprintf(LineVendorIdentifier,"ERROR");
	}

	if( Type != REG_SZ )
    {
       wsprintf(LineVendorIdentifier,"ERROR");
    }

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);
	
	return (char *)LineVendorIdentifier;
}
/*************************************************************************************************/
char * GetWhereIsThisExe(void)
{
	LPSTR tail;
	char *fullfilename=NULL;
	fullfilename=(char*)malloc(sizeof(char)*MAX_PATH);

	GetModuleFileName(GetModuleHandle(NULL),fullfilename,MAX_PATH);

	if ((tail = strrchr (fullfilename, '\\')) != (LPSTR) NULL)
    {
      tail++;
      *tail = '\0';
    }
  	
	return (char*)fullfilename;
}
/*************************************************************************************************/
BOOL FileExist(char *filename)
{
     BOOL retour=FALSE;	
	
     WIN32_FIND_DATA FindFileData;
     HANDLE handle = FindFirstFile (filename, &FindFileData);
     if (handle != INVALID_HANDLE_VALUE)
     {
         FindClose (handle);
         retour=TRUE;
     }
     else retour=FALSE;

     return retour;
}
/*************************************************************************************************/
BOOL LoadArrayCPUFromFile(char *path)
{
	#define ATLASSPEC "atlas.spec"
	BOOL vOK=FALSE;
	char *fullfilename=NULL;
    FILE *pFile;

    fullfilename=(char*)malloc(sizeof(char)*(strlen(path)+strlen(ATLASSPEC)+2));
	wsprintf(fullfilename,"%s\\%s",path,ATLASSPEC);
	if (FileExist(fullfilename))
	{
		char Line[LineMaxSize];
		int nbrLines=0;
		pFile=fopen(fullfilename,"rt");
		while (fgets(Line, LineMaxSize,pFile))
		{
			if (nbrLines >= MaxCPU)
			{
				AppendMessageToLog("See LoadArrayCPUFromFile function in SetupAtlas.c");
				return FALSE;
			}
			Line[strlen(Line)-1]='\0';
			wsprintf(ArrayLinesCPU[nbrLines],"%s",Line);
			nbrLines++;
			NbrLinesCPU=nbrLines;
		}
		fclose(pFile);
		vOK=TRUE;
	}

	free(fullfilename);
	fullfilename=NULL;
	
	return vOK;

}
/*************************************************************************************************/
char *GetLocalCPU(void)
{
	char *CPUString=NULL;
	char *Vendor=NULL;
	char *Identifier=NULL;

	Identifier=GetRegKeyIdentifier();
	Vendor=GetRegKeyVendorIdentifier();
	CPUString=(char *)malloc(sizeof(char)*LineMaxSize);

	if ( (strcmp(Identifier,"ERROR")!=0) && (strcmp(Vendor,"ERROR")!=0) )
	{
		wsprintf(CPUString,"%s %s",Vendor,Identifier);
	}
	else wsprintf(CPUString,"ERROR");
	
	return (char*)CPUString;
}
/*************************************************************************************************/
BOOL GetInfoCPUfromLine(char * line,BOOL FromFile,char *Vendor,int *Family,int *Model,int *Step,char *DLLName)
{
	BOOL vOK=FALSE;
	
	char Architecture[32];
	char wFamily[32];
	char wModel[32];
	char wStep[32];
	char wRemark[32];

	if (FromFile)
	{
		char F[32],M[32];
		
		sscanf(line,"%s %s %s %s %s %s %s",Vendor,wFamily,F,wModel,M,DLLName,wRemark);

		if (strcmp(F,"*")==0)
		{
			*Family=-1;
		}
		else *Family=(int)strtol(F, (char **)NULL, 10);

		if (strcmp(M,"*")==0)
		{
			*Model=-1;
		}
		else *Model=(int)strtol(M, (char **)NULL, 10);
		
		*Step=-1;
		
		if ( (strcmp(wFamily,"Family")==0) &&
			 (strcmp(wModel,"Model")==0) )
		{
			vOK=TRUE;
		}
	}
	else
	{
		char F[32],M[32],S[32];

		sscanf(line,"%s %s %s %s %s %s %s %s",
			         Vendor,Architecture,wFamily,F,wModel,M,wStep,S);

		*Family=(int)strtol(F, (char **)NULL, 10);
		*Model=(int)strtol(M, (char **)NULL, 10);
		*Step=(int)strtol(S, (char **)NULL, 10);

		if ( (strcmp(wFamily,"Family")==0) &&
			 (strcmp(wModel,"Model")==0) &&
			 (strcmp(wStep,"Stepping")==0) )
		{
			vOK=TRUE;
		}
		wsprintf(DLLName,"EMPTY");
	}
	
	return vOK;
}
/*************************************************************************************************/
char *GetDLLFilenameAtlas(void)
{
	#define PathAtlas  "Atlas" 
	char *PathOfThisExe=NULL;
	char *FilenameAtlasDLL=NULL;
	BOOL FindCPU=FALSE;

	FilenameAtlasDLL=malloc(sizeof(char)*MAX_PATH);
	wsprintf(FilenameAtlasDLL,"%s",DefaultDllName);

	PathOfThisExe=GetWhereIsThisExe();

	if ( LoadArrayCPUFromFile(PathOfThisExe) )
	{
		char *LocalCPU=NULL;
		char LocalVendor[32];
		char LocalFilenameNotUse[32];
		int LocalFamily,LocalModel,LocalStep;
		
		LocalCPU=GetLocalCPU();
		AppendMessageToLog(LocalCPU);
		GetInfoCPUfromLine(LocalCPU,FALSE,LocalVendor,&LocalFamily,&LocalModel,&LocalStep,LocalFilenameNotUse);
        if ( strcmp(LocalCPU,"ERROR")!=0 )
		{
			int i=0;
			for (i=0;i<NbrLinesCPU;i++)
			{
				int F,M,S;
				char Vendor[32];
				char Filename[MAX_PATH];

				if ( GetInfoCPUfromLine(ArrayLinesCPU[i],TRUE,Vendor,&F,&M,&S,Filename) )
				{
					if ( (!FindCPU) && (strcmp(LocalVendor,Vendor)==0) )
					{
						if ( (LocalFamily == F) && (LocalModel == M) )
						{
							FindCPU=TRUE;
							wsprintf(FilenameAtlasDLL,"%s",Filename);
							AppendMessageToLog(ArrayLinesCPU[i]);
						}
						else if ( LocalModel == -1 )
						{
							FindCPU=TRUE;
							wsprintf(FilenameAtlasDLL,"%s",Filename);
							AppendMessageToLog(ArrayLinesCPU[i]);
						}
					}
				}
				else
				{
					AppendMessageToLog("Error in Atlas.spec");
				}
			}
		}
		else
		{
			AppendMessageToLog("Error Detection CPU from Registry key.");
		}
		free(LocalCPU);
		LocalCPU=NULL;
	}
	
	if (!FindCPU)
	{
		AppendMessageToLog("Use default Atlas library for Pentium");
	}

	free(PathOfThisExe);
	PathOfThisExe=NULL;

	return (char*)FilenameAtlasDLL;
}
/*************************************************************************************************/
BOOL CopyAtlas(char *AtlasSourceFilename)
{
	#define PathAtlas	  "Atlas"
	#define AtlasFilename "atlas.dll"
	BOOL bOK=FALSE;
	char *PathOfThisExe=NULL;
	char *FullDestinationFilenameAtlas=NULL;
	char *FullSourceFilenameAtlas=NULL;

	PathOfThisExe=GetWhereIsThisExe();

	FullSourceFilenameAtlas=malloc( sizeof(char) * (strlen(PathOfThisExe)+strlen(PathAtlas)+strlen(AtlasSourceFilename)+2) );
	wsprintf(FullSourceFilenameAtlas,"%s%s\\%s",PathOfThisExe,PathAtlas,AtlasSourceFilename);
	AppendMessageToLog(FullSourceFilenameAtlas);

	FullDestinationFilenameAtlas=malloc( sizeof(char) * (strlen(PathOfThisExe)+strlen(AtlasSourceFilename)+2) );
	wsprintf(FullDestinationFilenameAtlas,"%s%s",PathOfThisExe,AtlasFilename);
	AppendMessageToLog(FullDestinationFilenameAtlas);

	if ( FileExist(FullSourceFilenameAtlas) )
	{
		if ( !CopyFile(FullSourceFilenameAtlas,FullDestinationFilenameAtlas,FALSE) )
		{
			char msg[512];

			wsprintf(msg,"Impossible to copy %s.",FullDestinationFilenameAtlas);
			// MessageBox(NULL,msg,"Error",MB_ICONWARNING|MB_OK);
			AppendMessageToLog(msg);
		}
		else bOK=TRUE;
	}
	else
	{
		char msg[512];
		wsprintf(msg,"File %s not found.",FullSourceFilenameAtlas);
		// MessageBox(NULL,msg,"Error",MB_ICONWARNING|MB_OK);
		AppendMessageToLog(msg);
	}

	if (FullDestinationFilenameAtlas)
	{
		free(FullDestinationFilenameAtlas);
		FullDestinationFilenameAtlas=NULL;
	}

	if (FullSourceFilenameAtlas)
	{
		free(FullSourceFilenameAtlas);
		FullSourceFilenameAtlas=NULL;
	}

	free(PathOfThisExe);
	PathOfThisExe=NULL;

	return bOK;
}
/*************************************************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	char *DLLFilename=NULL;

	AppendMessageToLog("**************************************************");
	DLLFilename=GetDLLFilenameAtlas();
	AppendMessageToLog(DLLFilename);
	if ( CopyAtlas(DLLFilename) ) AppendMessageToLog("Copy file OK");
	else AppendMessageToLog("Copy file Not OK");
	AppendMessageToLog("**************************************************");

	free(DLLFilename);
	DLLFilename=NULL;
	return 0;
}
/*************************************************************************************************/
