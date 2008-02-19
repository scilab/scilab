/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#if _MSC_VER
#include <Windows.h>
#include "version.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loadsavelanguage.h"
#include "setgetlanguage.h"
#include "SCIHOME.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#define LINELENGTHMAX 1024
#define TAGLANGUAGE "LANGUAGE"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define SCILABREGISTRYKEY "SOFTWARE\\Scilab\\%s\\Settings"
static BOOL loadlanguagepref_windows(void);
static BOOL savelanguagepref_windows(void);
#else
static BOOL loadlanguagepref_linux(void);
static BOOL savelanguagepref_linux(void);
static char *getfilenamelanguagepref(void);
#endif
/*--------------------------------------------------------------------------*/
BOOL loadlanguagepref(void)
{
#ifdef _MSC_VER
	return loadlanguagepref_windows();
#else
	return loadlanguagepref_linux();
#endif
}
/*--------------------------------------------------------------------------*/
BOOL savelanguagepref(void)
{
#ifdef _MSC_VER
	return savelanguagepref_windows();
#else
	return savelanguagepref_linux();
#endif
}
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static BOOL loadlanguagepref_linux(void)
{
	char *filepref=NULL;
	FILE *fileR;

	filepref=getfilenamelanguagepref();

	if (filepref)
	{
		fileR= fopen(filepref, "rt");
		if (fileR)
		{
			char *LANGUAGE=NULL;
			char LINE[LINELENGTHMAX];

			LANGUAGE=(char*)MALLOC(sizeof(char)*LINELENGTHMAX);
			fscanf(fileR, "%[^\n]", LINE);
			sscanf(&LINE[strlen(TAGLANGUAGE)+1],"%s",LANGUAGE);
			setlanguage(LANGUAGE,FALSE,TRUE);

			fclose(fileR);

			if (LANGUAGE){FREE(LANGUAGE);LANGUAGE=NULL;}
			return TRUE;
		}
	}
	// TODO : if this file is not available load the locale of the system for the language
	return FALSE;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static BOOL savelanguagepref_linux(void)
{
	char *filepref=NULL;
	FILE *fileW;

	filepref=getfilenamelanguagepref();

	if (filepref)
	{
		fileW= fopen(filepref, "wt");
		if (fileW)
		{
			char LINE[LINELENGTHMAX];
			char *LANGUAGE=NULL;
			LANGUAGE=getlanguage();
			sprintf(LINE,"%s=%s",TAGLANGUAGE,LANGUAGE);
			fputs(LINE,fileW);
			fputs("\n",fileW);
			fclose(fileW);

			//			if (LANGUAGE){FREE(LANGUAGE);LANGUAGE=NULL;}
			return TRUE;
		}
	}
	return FALSE;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static char *getfilenamelanguagepref(void)
{
#if _MSC_VER
#define namelanguagepref "\\language.ini"
#else
#define namelanguagepref "/.language"
#endif

	char *retFilename=NULL;
	char *SCIHOME=NULL;

	SCIHOME=getSCIHOME();

	if (SCIHOME)
	{
		retFilename=(char*)MALLOC((strlen(namelanguagepref)+strlen(SCIHOME)+1)*sizeof(char));
		sprintf(retFilename,"%s%s",SCIHOME,namelanguagepref);
		FREE(SCIHOME);
		SCIHOME = NULL;
	}
	else
	{
		retFilename=(char*)MALLOC((strlen(namelanguagepref)+1)*sizeof(char));
		strcpy(retFilename,namelanguagepref);
	}

	return retFilename;
}
#endif
/*--------------------------------------------------------------------------*/
#if _MSC_VER
static BOOL loadlanguagepref_windows(void)
{
	BOOL bOK=FALSE;
	/* @TODO translate to english
	   Modification Restauration depuis la base de registre dans
	   HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Settings\\LANGUAGE
	   "Version" correspondant à la version de Scilab
	*/
	HKEY key;

	char Clef[LINELENGTHMAX];
	int LanguageCode=0;

	wsprintf(Clef,SCILABREGISTRYKEY,SCI_VERSION_STRING);

	if ( RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key) == ERROR_SUCCESS )
	{
		DWORD size=4;
		if ( RegQueryValueEx(key, TAGLANGUAGE, 0, NULL, (LPBYTE)&LanguageCode, &size) ==  ERROR_SUCCESS )
		{
			char *LANGUAGE=NULL;
			LANGUAGE=getlanguagefromcode(LanguageCode);

			if (LANGUAGE)
			{
				setlanguage(LANGUAGE,FALSE,TRUE);
				bOK=TRUE;
				FREE(LANGUAGE);
				LANGUAGE=NULL;
			}
		}
		RegCloseKey(key);
	}
	return bOK;
}
#endif
/*--------------------------------------------------------------------------*/
#if _MSC_VER
static BOOL savelanguagepref_windows(void)
{
	BOOL bOK=FALSE;
	/* Modification Sauvegarde dans la base de registre dans
	HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Settings\\LANGUAGE
	"Version" correspondant à la version de Scilab
	Sauvegarde dans HKEY_CURRENT_USER car données dépendant de l'utilisateur
	*/
	HKEY key;
	DWORD result,dwsize=4;
	char Clef[LINELENGTHMAX];
	char *LANGUAGE=NULL;
	int LanguageCode=0;

	LANGUAGE=getlanguage();
	LanguageCode=getcodefromlanguage(LANGUAGE);
	// FREE not need here because getcodefromlanguage returns a static array
	//if (LANGUAGE) {FREE(LANGUAGE);LANGUAGE=NULL;}

	wsprintf(Clef,SCILABREGISTRYKEY,SCI_VERSION_STRING);

	if (RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result) == ERROR_SUCCESS)
	{
		if ( RegSetValueEx(key, TAGLANGUAGE, 0, REG_DWORD, (LPBYTE)&LanguageCode, dwsize) == ERROR_SUCCESS)
		{
			bOK=TRUE;
		}
		RegCloseKey(key);
	}
	return bOK;
}
#endif
/*--------------------------------------------------------------------------*/

