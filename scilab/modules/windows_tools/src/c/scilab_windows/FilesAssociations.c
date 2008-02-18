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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>
#include <shlwapi.h>
#include "version.h"
#include "FilesAssociations.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "FindScilab.h"
#include "wmcopydata.h"
/*--------------------------------------------------------------------------*/
extern void PrintFile(char *filename);
/*--------------------------------------------------------------------------*/
static void ReplaceSlash(char *pathout,char *pathin);
/*--------------------------------------------------------------------------*/
#define MSG_SCIMSG1 "%s -e load(getlongpathname('%s'));disp(getlongpathname('%s')+ascii(32)+'loaded');"
#define MSG_SCIMSG2 "%s -e scicos(getlongpathname('%s'));"
#define MSG_SCIMSG3 "%s -e edit_graph(getlongpathname('%s'));"
#define MSG_SCIMSG4 "%s -e exec(getlongpathname('%s'));"
#define MSG_SCIMSG5 "%s -e scipad(getlongpathname('%s')); "
#define MSG_SCIMSG6 "scipad('%s'); "
#define MSG_SCIMSG7 "Scilab Communication"
#define MSG_SCIMSG8 "%s -e scipad(getlongpathname('%s')); "
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
/* Teste si le fichier a une extension .sav ou .bin*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsABinOrSavFile(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=PathFindExtension(chainefichier);
	
	/* Comparaison avec les extension BIN et SAV */
	if ( (_stricmp(ExtensionFilename,".BIN")==0) || (_stricmp(ExtensionFilename,".SAV")==0) ) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;

}
/*--------------------------------------------------------------------------*/
BOOL IsAGraphFile(char *chainefichier)
{
	BOOL retour=FALSE;
	
	if ( IsAGraphFilegraphb(chainefichier) || IsAGraphFilegraph(chainefichier) ) retour=TRUE;
	
	return retour;
}
/*--------------------------------------------------------------------------*/
BOOL IsAGraphFilegraph(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=PathFindExtension(chainefichier);

	/* Comparaison avec l'extension Graph */
	if (_stricmp(ExtensionFilename,".GRAPH")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*--------------------------------------------------------------------------*/
BOOL IsAGraphFilegraphb(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=PathFindExtension(chainefichier);

	/* Comparaison avec l'extension Graphb */
	if (_stricmp(ExtensionFilename,".GRAPHB")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFile(char *chainefichier)
{
	BOOL retour=FALSE;
	
	if ( IsAScicosFileCOS(chainefichier) || IsAScicosFileCOSF(chainefichier) ) retour=TRUE;
	
	return retour;
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFileCOS(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=PathFindExtension(chainefichier);

	/* Comparaison avec l'extension cos */
	if (_stricmp(ExtensionFilename,".COS")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFileCOSF(char *chainefichier)
{
	BOOL retour=FALSE;
	char *ExtensionFilename=NULL;

	ExtensionFilename=PathFindExtension(chainefichier);

	/* Comparaison avec l'extension cosf */
	if (_stricmp(ExtensionFilename,".COSF")==0) retour=TRUE;

	if (ExtensionFilename) {FREE(ExtensionFilename);ExtensionFilename=NULL;}
	return retour;
}
/*--------------------------------------------------------------------------*/
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
				if ( IsABinOrSavFile(FinalFileName) == TRUE )
				{
					/* C'est un fichier .BIN ou .SAV d'ou load */
					wsprintf(Cmd,MSG_SCIMSG1,PathWScilex,FinalFileName,FinalFileName);
				}
				else
				if  ( IsAScicosFile(fichier) == TRUE )
				{
					ExtensionFileIntoLowerCase(FinalFileName);	
					wsprintf(Cmd,MSG_SCIMSG2,PathWScilex,FinalFileName);
				}
				else
				if ( IsAGraphFile(fichier) == TRUE )
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
				/* open a new scilab for each files */
				wsprintf(Cmd,MSG_SCIMSG5,PathWScilex,FinalFileName);

				/*
				if (! HaveAnotherWindowScilab())
				{
					wsprintf(Cmd,MSG_SCIMSG5,PathWScilex,FinalFileName);
				}
				else
				{
					char *ScilabDestination=NULL;

					wsprintf(Cmd,MSG_SCIMSG6,FinalFileName);
					ScilabDestination = (char*)ChooseAnotherWindowScilab();
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
				*/
			}
			break;
		}
	}	
	return Retour;
}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
static void ReplaceSlash(char *pathout,char *pathin)
{
	int i=0;
	for (i=0;i < (int)strlen(pathin);i++)
	{
		if (pathin[i]=='\\') pathout[i]='/';
		else pathout[i]=pathin[i];
	}
	pathout[i]='\0';
}
/*--------------------------------------------------------------------------*/
