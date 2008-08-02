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
#include "../../../io/includes/setenvc.h"
/*--------------------------------------------------------------------------*/
static char CURRENTLANGUAGESTRING[6] = SCILABDEFAULTLANGUAGE;
/*--------------------------------------------------------------------------*/
static BOOL exportLocaleToSystem(char *locale);
/*--------------------------------------------------------------------------*/
BOOL setlanguage(char *lang)
{
	if (lang)
	{
		/* Load the locale from the system */
		#ifndef _MSC_VER
		/* get current value LC_MESSAGES */
		char *ret = setlocale(LC_MESSAGES,"");
		if (strlen(lang) == 5) /* example : en_US */
		{
			char *langEncoding = NULL;
			char *Encoding = getEncoding(lang);
			if (Encoding)
			{
				langEncoding = (char*)MALLOC((strlen(Encoding)+strlen(".")+strlen(lang))*sizeof(char));
				if (langEncoding) 
				{
					sprintf(langEncoding,"%s.%s",lang,Encoding);
					ret = setlocale(LC_MESSAGES,langEncoding);
					FREE(langEncoding);
					langEncoding = NULL;
				}
				FREE(Encoding);
				Encoding = NULL;
			}
		}
		else
		{
			ret = setlocale(LC_MESSAGES,lang);
		}
		#else
		/* Load the user locale from the system */
		char *ret = getLocaleUserInfo();
		#endif

		/* change language */
		if (strcmp(lang,"C")==0)
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
				strcpy(CURRENTLANGUAGESTRING,lang);
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
	if ( !setenvc(EXPORTENVLOCALESTR,locale))
	{
		fprintf(stderr,"Localization: Failed to declare the system variable %s\n", EXPORTENVLOCALE);
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
