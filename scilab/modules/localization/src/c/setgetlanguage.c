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

#include "machine.h"

#ifndef _MSC_VER
 #ifdef HAVE_LIBINTL_H
  #include <libintl.h>
  #ifdef HAVE_LOCALE_H
   #include <locale.h>
  #else
   #error "Cannot find locale.h despite that libintl.h is available"
  #endif
 #endif
#else
	#include <locale.h>
	#include <libintl.h>
#endif

#ifdef _MSC_VER
#include "getLocaleInfo_Windows.h"
#include "strdup_windows.h"
#endif

#include "localetoutf.h"

#include "setgetlanguage.h"
#include "MALLOC.h"
#include "defaultlanguage.h"
#include "scilabDefaults.h"
#include "setenvc.h"
/*--------------------------------------------------------------------------*/
static char CURRENTLANGUAGESTRING[6] = SCILABDEFAULTLANGUAGE;
/*--------------------------------------------------------------------------*/
static BOOL exportLocaleToSystem(char *locale);
static char* addCodePage(char *lang);
/*--------------------------------------------------------------------------*/
BOOL setlanguage(char *lang)
{
	if (lang)
	{
		/* Load the locale from the system */
		char *ret = NULL;

		#ifndef _MSC_VER
		char *LANG = addCodePage(lang);
		ret = setlocale(LC_MESSAGES,LANG);
		if (LANG) {FREE(LANG);LANG = NULL;}
		#else
		/* Load the user locale from the system */
		ret = getLocaleUserInfo();
		#endif

		/* change language */
		if (strcmp(lang,"C") ==0 )
		{
			/* The lang is the default one... ie en_US */
			strcpy(CURRENTLANGUAGESTRING,SCILABDEFAULTLANGUAGE);
		}
		else
		{
			if ( (strcmp(lang,"") == 0) && (ret != NULL))
			{
				/* The requested language is the one of the system ... 
				 * which we don't really know which one is it
				 * but if setlocale worked, we get it from the return 
				 */

				/* 5 is the number of char in fr_FR for example */
				strncpy(CURRENTLANGUAGESTRING,ret,5); 
			}
			else
			{
				strncpy(CURRENTLANGUAGESTRING,lang,5);
			}
		}

		exportLocaleToSystem(CURRENTLANGUAGESTRING);

		/*open a localeToUTF converter if needed*/
		openLocaleToUTFConverter(ret,CURRENTLANGUAGESTRING);

		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getlanguage(void)
{
	return CURRENTLANGUAGESTRING;
}
/*--------------------------------------------------------------------------*/
/**
 * Export the variable LC_XXXX to the system
 *
 * @param locale the locale (ex : fr_FR or en_US)
 */
static BOOL exportLocaleToSystem(char *locale)
{
	/* It will put in the env something like LC_MESSAGES=fr_FR */

	#ifndef _MSC_VER
	char *LANG = addCodePage(locale);
	#endif

	/* on linux , we want to export something like LC_MESSAGES = fr_FR.utf-8 */
	/* on windows, we want to export something like LC_MESSAGES = fr_FR */

	#ifndef _MSC_VER
	if ( !setenvc(EXPORTENVLOCALESTR,LANG))
	#else
	if ( !setenvc(EXPORTENVLOCALESTR,locale))
	#endif
	{
		fprintf(stderr,"Localization: Failed to declare the system variable %s\n", EXPORTENVLOCALE);
		#ifndef _MSC_VER
		if (LANG) { FREE(LANG);LANG = NULL;}
		#endif
		return FALSE;
	}

	#ifndef _MSC_VER
	if (LANG) { FREE(LANG);LANG = NULL;}
	#endif

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
	#endif
	return TRUE;
}
/*--------------------------------------------------------------------------*/
char *getLanguageFromAlias(char *alias)
{
	if (alias)
	{
		if ( strcmp(alias,"en") == 0 )
		{
			return strdup("en_US");
		}

		if ( strcmp(alias,"fr") == 0 )
		{
			return strdup("fr_FR");
		}

		return strdup(alias);
	}
	/* "" value fixed by system */
	else return strdup("");
}
/*--------------------------------------------------------------------------*/
BOOL isValidLanguage(char *lang)
{
	/* Try to detect if it is a correct language */
	if (lang)
	{
		if ( strcmp(lang,"") == 0) return TRUE;
		if ( strcmp(lang,"C") == 0) return TRUE;
		if ( strcmp(lang,"en") == 0) return TRUE;
		if ( strcmp(lang,"fr") == 0) return TRUE;
		if ( ((int) strlen(lang) == 5) && (lang[2] == '_') ) return TRUE; /* xx_XX */
	}
	return FALSE;

}
/*--------------------------------------------------------------------------*/
/* On linux (Ubuntu) , we have this message :
	(<unknown>:25838): Gtk-WARNING **: Locale not supported by C library.
	Using the fallback 'C' locale.
	because LC_MESSAGES is only defined by Language_Country and not 
	Language_Country.CodePage (fr_FR.utf8)
*/
static char* addCodePage(char *lang)
{
	char *languageCountryCodePage = NULL;

	if ( strcmp(lang,"") == 0 ) /* default system */
	{
		#ifndef _MSC_VER
		languageCountryCodePage = strdup(setlocale(LC_MESSAGES,""));
		#else
		languageCountryCodePage = getLocaleUserInfo();
		#endif
	}
	else
	{
		#ifndef _MSC_VER
		char *retCTYPE = setlocale(LC_MESSAGES,"");
		#else
		char *retCTYPE = setlocale(LC_CTYPE,"");
		#endif
		char *encoding = getEncoding(retCTYPE);
		int sizelanguageCountryCodePage = (int)(strlen(lang) + strlen(encoding) + strlen(".") + 1);

		languageCountryCodePage = (char*)MALLOC(sizeof(char) * sizelanguageCountryCodePage);
		if (languageCountryCodePage)
		{
			strcpy(languageCountryCodePage,lang);
			strcat(languageCountryCodePage,".");
			strcat(languageCountryCodePage,encoding);
		}
		if (encoding) {FREE(encoding); encoding = NULL;}
	}
	return languageCountryCodePage;
}
/*--------------------------------------------------------------------------*/
