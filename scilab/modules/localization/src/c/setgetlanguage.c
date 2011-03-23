/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "machine.h" /*  HAVE_LIBINTL_H &  HAVE_LOCALE_H */

#ifndef _MSC_VER
#ifdef __APPLE__
#include <locale.h>
#else
 #ifdef HAVE_LIBINTL_H
  #include <libintl.h>
  #ifdef HAVE_LOCALE_H
   #include <locale.h>
  #else
   #error "Cannot find locale.h despite that libintl.h is available"
  #endif
 #endif
#endif
#else
	#include <locale.h>
	#include <libintl.h>
#endif

#ifdef _MSC_VER
#include "getLocaleInfo_Windows.h"
#endif
#ifdef __APPLE__
#include "getLocaleInfo_Apple.h"
#endif


#include "setgetlanguage.h"
#include "MALLOC.h"
#include "tableslanguages.h"
#include "defaultlanguage.h"
#include "scilabDefaults.h"
#include "../../../io/includes/setenvc.h"

/*--------------------------------------------------------------------------*/
static char CURRENTLANGUAGESTRING[LengthAlphacode]=SCILABDEFAULTLANGUAGE;
static int  CURRENTLANGUAGECODE=SCILABDEFAULTLANGUAGECODE;
/*--------------------------------------------------------------------------*/
static int FindLanguageCode(char *lang);
static BOOL setlanguagecode(char *lang);
static char *FindAlias(char *lang);
static char *GetLanguageFromAlias(char *langAlias);
/*--------------------------------------------------------------------------*/
BOOL setlanguage(char *lang)
{
	if (lang)
	{
		if (LanguageIsOK(lang))
		{
			#ifndef _MSC_VER
			if (needtochangelanguage(lang))
			{
			#endif
				/* Load the locale from the system */
#if !defined(_MSC_VER) && !defined(__APPLE__)
				//for mbstowcs
				char *ret=setlocale(LC_CTYPE,lang);
				//for gettext
				ret=setlocale(LC_MESSAGES,lang);
				#else
				/* Load the user locale from the system */
				char *ret = getLocaleUserInfo();
				#endif

				//				  This stuff causes pb when locales have been compiled
				if (ret==NULL)
				{
					#ifndef _MSC_VER
					fprintf(stderr, "Warning: Localization issue. Does not support the locale '%s' %s %s.\n",lang,ret,setlocale(LC_MESSAGES,NULL));
					#else
					fprintf(stderr, "Warning: Localization issue. Cannot detect user locale.\n");
					#endif
				}

				/* change language */
				if (strcmp(lang,"C")==0 || ret==NULL || strcmp(ret,"C")==0)
				{
					/* The lang is the default one... ie en_US */
					strcpy(CURRENTLANGUAGESTRING,SCILABDEFAULTLANGUAGE);
				}
				else
				{
					if (strcmp(lang,"")==0 && ret!=NULL)
					{
						/* The requested language is the one of the system ...
						 * which we don't really know which one is it
						 * but if setlocale worked, we get it from the return
						 */
						strncpy(CURRENTLANGUAGESTRING,ret,5); /* 5 is the number of char in fr_FR for example */
					}
					else
					{
						strcpy(CURRENTLANGUAGESTRING,lang);
					}
				}
				#ifndef _MSC_VER
				setlanguagecode(CURRENTLANGUAGESTRING);
				#endif

				exportLocaleToSystem(CURRENTLANGUAGESTRING);
				return TRUE;
			}
		#ifndef _MSC_VER
		}
		#endif
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getlanguage(void)
{
	return CURRENTLANGUAGESTRING;
}
/*--------------------------------------------------------------------------*/
int getcurrentlanguagecode(void)
{
	return CURRENTLANGUAGECODE;
}
/*--------------------------------------------------------------------------*/
char *getlanguagefromcode(int code)
{
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (LANGUAGE_COUNTRY_TAB[i].code == code)
		{
			return LANGUAGE_COUNTRY_TAB[i].alphacode;
		}
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
int getcodefromlanguage(char *language)
{
	return FindLanguageCode(language);
}
/*--------------------------------------------------------------------------*/
BOOL LanguageIsOK(char *lang)
{
	int i=0;

	if (strlen(lang)==0){ /* Empty language declaration... it is the default
						   * language from the system */
		return TRUE;
	}

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
		  return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
static int FindLanguageCode(char *lang)
{
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
			return LANGUAGE_COUNTRY_TAB[i].code;
		}
	}
	return -1;
}
/*--------------------------------------------------------------------------*/
static BOOL setlanguagecode(char *lang)
{
	int tmpCode=FindLanguageCode(lang);

	if (tmpCode>0)
	{
		CURRENTLANGUAGECODE=tmpCode;
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *FindAlias(char *lang)
{
	int i=0;
	for(i=0;i<NumberLanguagesAlias;i++)
	{
		if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alphacode,lang)==0)
		{
		  return LANGUAGE_LOCALE_ALIAS[i].alias;
		}
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
static char *GetLanguageFromAlias(char *langAlias)
{
	int i=0;
	for(i=0;i<NumberLanguagesAlias;i++)
	{
		if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alias,langAlias)==0)
		{
			return LANGUAGE_LOCALE_ALIAS[i].alphacode;
		}
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char *getlanguagealias(void)
{
	return FindAlias(CURRENTLANGUAGESTRING);
}
/*--------------------------------------------------------------------------*/
BOOL needtochangelanguage(char *language)
{
  char *currentlanguage=getlanguage();

  if (strcmp(language,currentlanguage)) {
    return TRUE;
  }

  return FALSE;
}
/*--------------------------------------------------------------------------*/
char *convertlanguagealias(char *strlanguage)
{
	char *correctlanguage=NULL;

	if ( (strlen(strlanguage)==2) || (strcmp(strlanguage,"en_US")==0) ) /* If the user wants to change to en_US ... use the default locale */
	{
		correctlanguage=GetLanguageFromAlias(strlanguage);
	}
	else {
		if (strcmp(strlanguage,"eng")==0) /* compatibility previous scilab */
			{
				correctlanguage=GetLanguageFromAlias("en");
			} else {
				if (strlen(strlanguage)==5 && strlanguage[2]=='_') { /* already xx_XX (fr_FR) */
					return strlanguage;
				}
			}
	}
	return correctlanguage;
}
/*--------------------------------------------------------------------------*/
/**
 * Export the variable LC_XXXX to the system
 *
 * @param locale the locale (ex : fr_FR or en_US)
 */
BOOL exportLocaleToSystem(char *locale){

	if (locale==NULL)
	{
#ifdef _MSC_VER
		fprintf(stderr,"Localization: Have not been able to find a suitable locale. Remains to default %s.\n", "LC_CTYPE");
#else
		fprintf(stderr,"Localization: Have not been able to find a suitable locale. Remains to default %s.\n", EXPORTENVLOCALE);
#endif
		return FALSE;
	}

	/* It will put in the env something like LC_MESSAGES=fr_FR */
	if ( !setenvc(EXPORTENVLOCALESTR,locale))
	{
#ifdef _MSC_VER
		fprintf(stderr,"Localization: Failed to declare the system variable %s.\n", "LC_CTYPE");
#else
		fprintf(stderr,"Localization: Failed to declare the system variable %d.\n", EXPORTENVLOCALE);
#endif
		return FALSE;
	}

#ifdef _MSC_VER
#ifdef USE_SAFE_GETTEXT_DLL
	{
		/* gettext is buggy on Windows */
		/* We need to set a external environment variable to scilab env. */

		char env[MAX_PATH];
		sprintf(env,"%s=%s",EXPORTENVLOCALESTR,locale);
		gettext_putenv(env);
	}
#endif
#else
	/* Export LC_NUMERIC to the system to make sure that the rest of system
	   is using the english notation (Java, Tcl ...) */
	setenvc("LC_NUMERIC",LCNUMERICVALUE);
#endif

	return TRUE;
}
/*--------------------------------------------------------------------------*/
