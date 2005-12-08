/* Allan CORNET INRIA 2005 */
#include "FilesAssociations.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
typedef  LPCTSTR (WINAPI * PathFindExtensionPROC) (LPCTSTR); 
/*-----------------------------------------------------------------------------------*/
extern void ReplaceSlash(char *pathout,char *pathin);
extern void PrintFile(char *filename);
extern BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine);
extern BOOL HaveAnotherWindowScilab(void);
extern char * ChooseAnotherWindowScilab(void);
/*-----------------------------------------------------------------------------------*/
/* teste si la chaine de caractere correspond à un fichier*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsAFile(char *chainefichier)
{
     BOOL retour=FALSE;	
	
     WIN32_FIND_DATA FindFileData;
     HANDLE handle = FindFirstFile (chainefichier, &FindFileData);
     if (handle != INVALID_HANDLE_VALUE)
     {
         FindClose (handle);
         retour=TRUE;
     }
     else retour=FALSE;

     return retour;
}
/*-----------------------------------------------------------------------------------*/
/* Teste si le fichier a une extension .sav ou .bin*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsABinOrSavFile(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=ScilabPathFindExtension(chainefichier);
	
	/* Comparaison avec les extension BIN et SAV */
	if ( (_stricmp(ExtensionFilename,".BIN")==0) || (_stricmp(ExtensionFilename,".SAV")==0) ) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;

}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFile(char *chainefichier)
{
	BOOL retour=FALSE;
	
	if ( IsAGraphFilegraphb(chainefichier) || IsAGraphFilegraph(chainefichier) ) retour=TRUE;
	
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFilegraph(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=ScilabPathFindExtension(chainefichier);

	/* Comparaison avec l'extension Graph */
	if (_stricmp(ExtensionFilename,".GRAPH")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFilegraphb(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=ScilabPathFindExtension(chainefichier);

	/* Comparaison avec l'extension Graphb */
	if (_stricmp(ExtensionFilename,".GRAPHB")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFile(char *chainefichier)
{
	BOOL retour=FALSE;
	
	if ( IsAScicosFileCOS(chainefichier) || IsAScicosFileCOSF(chainefichier) ) retour=TRUE;
	
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFileCOS(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=ScilabPathFindExtension(chainefichier);

	/* Comparaison avec l'extension cos */
	if (_stricmp(ExtensionFilename,".COS")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFileCOSF(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=ScilabPathFindExtension(chainefichier);

	/* Comparaison avec l'extension cosf */
	if (_stricmp(ExtensionFilename,".COSF")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*-----------------------------------------------------------------------------------*/
int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd)
{
	int Retour=FALSE;
	char FinalFileName[MAX_PATH];
	char ShortPath[MAX_PATH];
	char PathWScilex[MAX_PATH];
	
	if (fichier[0]=='\"')
	{
		char buffertemp[MAX_PATH];
		int i=1;
		
		while (fichier[i] != '"')
		{
			buffertemp[i-1]=fichier[i];
			i++;
			if (i> (int)strlen(fichier))
			{
				i=(int)strlen(fichier);
				break;
			}
		}
		buffertemp[i-1]='\0';
		strcpy(fichier,buffertemp);
	}
	if (fichier[strlen(fichier)-1]=='\"') fichier[strlen(fichier)-1]='\0';

	if (IsAFile(fichier))
	{
		GetShortPathName(fichier,ShortPath,MAX_PATH); /* Recuperation du nom du fichier au format 8.3 */
		ReplaceSlash(FinalFileName,ShortPath);
		GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), PathWScilex, MAX_PATH);
		Retour=TRUE;

		switch (OpenCode)
		   {
			case 1: /* Execute -X*/
				{
					if ( IsABinOrSavFile(FinalFileName)== TRUE )
					{
						/* C'est un fichier .BIN ou .SAV d'ou load */
						wsprintf(Cmd,MSG_SCIMSG1,PathWScilex,FinalFileName,FinalFileName);
					}
					else
					if  ( IsAScicosFile(fichier)== TRUE )
					{
					  ExtensionFileIntoLowerCase(FinalFileName);	
						wsprintf(Cmd,MSG_SCIMSG2,PathWScilex,FinalFileName);
					}
					else
					if ( IsAGraphFile(fichier)== TRUE )
					{
						ExtensionFileIntoLowerCase(FinalFileName);	
						wsprintf(Cmd,MSG_SCIMSG3,PathWScilex,FinalFileName);
					}
					else wsprintf(Cmd,MSG_SCIMSG4,PathWScilex,FinalFileName);
				}
			break;
			case 2: /* Print -P*/
				{
					PrintFile(fichier);
					strcpy(Cmd," ");
					exit(0);
				}
			break;
			case 0:default: /* Open -O*/
				{
					if (! HaveAnotherWindowScilab())
					{
						wsprintf(Cmd,MSG_SCIMSG5,PathWScilex,FinalFileName);
					}
					else
					{
						char *ScilabDestination=NULL;
						wsprintf(Cmd,MSG_SCIMSG6,FinalFileName);

						ScilabDestination=(char*)ChooseAnotherWindowScilab();
						if (ScilabDestination)
						{
							SendCommandToAnotherScilab(MSG_SCIMSG7,ScilabDestination,Cmd);
							FREE(ScilabDestination);
							exit(0);
						}
						else
						{
							wsprintf(Cmd,MSG_SCIMSG8,PathWScilex,FinalFileName);
						}
						
					}
				}
			break;
		
	}
	
	}	
	
	
	return Retour;
}
/*-----------------------------------------------------------------------------------*/
void ExtensionFileIntoLowerCase(char *fichier)
{
	char *tmpfile=NULL;
	char *buffer=NULL;
	char *lastdot=NULL;
	char *ext=NULL;
	
	tmpfile=(char*)MALLOC(strlen(fichier)*sizeof(char));
	strcpy(tmpfile,fichier);
	
	buffer=strtok(tmpfile,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}
	/* le dernier . permet d'avoir l'extension */
	ext=_strlwr(lastdot); /* Fichier en Majuscule */
	
	strcpy(&fichier[strlen(fichier)-strlen(ext)],ext);
	
	FREE(tmpfile);
}
/*-----------------------------------------------------------------------------------*/
LPTSTR ScilabPathFindExtension(LPCTSTR pPath)
{
	LPTSTR StrReturn=NULL;
	HINSTANCE ShlApiDll = LoadLibrary ("shlwapi.dll"); 

	if ( ShlApiDll ) 
	{ 
		PathFindExtensionPROC myPathFindExtension = (PathFindExtensionPROC) GetProcAddress(ShlApiDll,"PathFindExtensionA"); 

		if ( myPathFindExtension ) 
		{ 
			LPTSTR Extension=NULL;
			Extension = (LPTSTR )(myPathFindExtension)(pPath); 
			StrReturn = (char *) MALLOC ((strlen(Extension)+1)*sizeof(char));
			wsprintf(StrReturn,"%s",Extension);
		} 
		else 
		{ 
			char ChaineTemp[MAX_PATH];
			char *buffer=NULL;
			char *lastdot=NULL;

			sciprint("Please update your shlwapi.dll file.\n");
			strcpy(ChaineTemp,pPath);
			/* Recherche de l'extension du fichier */
			buffer=strtok(ChaineTemp,".");
			while ( buffer = strtok(NULL,"."))
			{
				lastdot=buffer;
			}
			if (lastdot)
			{
				StrReturn = (char *) MALLOC ((strlen(lastdot)+2)*sizeof(char));
				strcpy(StrReturn,".");
				strcat(StrReturn,lastdot);
			}
			else
			{
				StrReturn =NULL;
			}
		} 
	}
	else
	{
		char ChaineTemp[MAX_PATH];
		char *buffer=NULL;
		char *lastdot=NULL;

		sciprint("Please update your shlwapi.dll file.\n");
		strcpy(ChaineTemp,pPath);
		/* Recherche de l'extension du fichier */
		buffer=strtok(ChaineTemp,".");
		while ( buffer = strtok(NULL,"."))
		{
			lastdot=buffer;
		}
		if (lastdot)
		{
			StrReturn = (char *) MALLOC ((strlen(lastdot)+2)*sizeof(char));
			strcpy(StrReturn,".");
			strcat(StrReturn,lastdot);
		}
		else
		{
			StrReturn =NULL;
		}
	}

	FreeLibrary( ShlApiDll ); 

	return StrReturn;
}
/*-----------------------------------------------------------------------------------*/

