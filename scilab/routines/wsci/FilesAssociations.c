/* Allan CORNET INRIA 2005 */
#include "FilesAssociations.h"

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
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec les extension BIN et SAV */
	if ( (strcmp(lastdot,"BIN")==0) || (strcmp(lastdot,"SAV")==0) ) retour=TRUE;

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
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	unsigned int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}

	/* Comparaison avec l'extension Graph */
	if ( strcmp(lastdot,"GRAPH")==0) retour=TRUE;

	return retour;

}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFilegraphb(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	
	/* Comparaison avec l'extension Graph */
	if ( strcmp(lastdot,"GRAPHB")==0) retour=TRUE;

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
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}
	/* Mise en majuscule de l'extension du fichier*/
	if (lastdot == NULL) return retour;

	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec l'extension COS */
	
	if ( strcmp(lastdot,"COS")==0 ) retour=TRUE;

	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFileCOSF(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec l'extension COSF */
	if ( strcmp(lastdot,"COSF")==0 ) retour=TRUE;

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
						wsprintf(Cmd,"%s -e load(getlongpathname('%s'));disp(getlongpathname('%s')+ascii(32)+'loaded');",PathWScilex,FinalFileName,FinalFileName);
					}
					else
					if  ( IsAScicosFile(fichier)== TRUE )
					{
					    ExtensionFileIntoLowerCase(FinalFileName);	
						wsprintf(Cmd,"%s -e scicos(getlongpathname('%s'));",PathWScilex,FinalFileName);
					}
					else
					if ( IsAGraphFile(fichier)== TRUE )
					{
						ExtensionFileIntoLowerCase(FinalFileName);	
						wsprintf(Cmd,"%s -e edit_graph(getlongpathname('%s'));",PathWScilex,FinalFileName);
					}
					else wsprintf(Cmd,"%s -e exec(getlongpathname('%s'));",PathWScilex,FinalFileName);
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
						wsprintf(Cmd,"%s -e scipad(getlongpathname('%s')); ",PathWScilex,FinalFileName);
					}
					else
					{
						char *ScilabDestination=NULL;
						wsprintf(Cmd,"scipad('%s'); ",FinalFileName);

						ScilabDestination=(char*)ChooseAnotherWindowScilab();
						if (ScilabDestination)
						{
							SendCommandToAnotherScilab("Scilab Communication",ScilabDestination,Cmd);
							free(ScilabDestination);
							exit(0);
						}
						else
						{
							wsprintf(Cmd,"%s -e scipad(getlongpathname('%s')); ",PathWScilex,FinalFileName);
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
	
	tmpfile=(char*)malloc(strlen(fichier)*sizeof(char));
	strcpy(tmpfile,fichier);
	
	buffer=strtok(tmpfile,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}
	/* le dernier . permet d'avoir l'extension */
	ext=_strlwr(lastdot); /* Fichier en Majuscule */
	
	strcpy(&fichier[strlen(fichier)-strlen(ext)],ext);
	
	free(tmpfile);
}
/*-----------------------------------------------------------------------------------*/
