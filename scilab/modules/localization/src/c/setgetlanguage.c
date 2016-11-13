/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <wchar.h>
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
#include <windows.h>
#include "getLocaleInfo_Windows.h"
#endif
#ifdef __APPLE__
#include "getLocaleInfo_Apple.h"
#endif


#include "setgetlanguage.h"
#include "sci_malloc.h"
#include "tableslanguages.h"
#include "defaultlanguage.h"
#include "scilabDefaults.h"
#include "charEncoding.h"
#include "../../../io/includes/setenvc.h"
#include "os_string.h"

/*--------------------------------------------------------------------------*/
//static wchar_t CURRENTLANGUAGESTRING[LengthAlphacode] = SCILABDEFAULTLANGUAGE;
static char CURRENTLANGUAGESTRING[LengthAlphacode] = "en_US";
static int  CURRENTLANGUAGECODE = SCILABDEFAULTLANGUAGECODE;
/*--------------------------------------------------------------------------*/
static int FindLanguageCode(const char* lang);
static BOOL setlanguagecode(const char* lang);
static const char* FindAlias(const char* lang);
static const char* GetLanguageFromAlias(const char* langAlias);
/*--------------------------------------------------------------------------*/
BOOL setlanguage(const char* lang)
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
#if !defined(_MSC_VER)
                //for mbstowcs

                char *newlang = NULL;
                char *pstLang = wide_string_to_UTF8(lang);
                char *pstRet = setlocale(LC_CTYPE, lang);
                char *ret = NULL;

#ifdef __APPLE__
                /* Load the user locale from the system */
                if (pstLang == NULL || *pstLang == 0)
                {
                    if (pstLang)
                    {
                        FREE(pstLang);
                    }
                    pstLang = wide_string_to_UTF8(getLocaleUserInfo());
                }
#endif

                if (pstRet == NULL)
                {
                    if (pstLang == NULL || *pstLang == 0)
                    {
                        if (pstLang)
                        {
                            FREE(pstLang);
                        }

                        pstLang = strdup(getenv("LANG"));
                    }

                    pstRet = setlocale(LC_CTYPE, pstLang);
                    if (pstRet == NULL)
                    {
                        // On some OSes we need to precise the charset (e.g. on Debian, fr_FR is not accepted but fr_FR.UTF-8 is)
                        int i = 0;
                        for (; i < NumberOfCharsets; i++)
                        {
                            newlang = (char*)MALLOC(strlen(pstLang) + strlen(CHARSETS[i]) + 1 + 1);
                            sprintf(newlang, "%s.%s", pstLang, CHARSETS[i]);
                            pstRet = setlocale(LC_CTYPE, newlang);
                            if (pstRet == NULL)
                            {
                                FREE(newlang);
                                newlang = NULL;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                if (pstRet == NULL)
                {
                    fprintf(stderr,
                            "Warning: Localization issue. Failed to change the LC_CTYPE locale category. Does not support the locale '%s' %s %s.\nDid you install the system locales?\n",
                            lang, ret, setlocale(LC_CTYPE, NULL));
                }

                //for gettext
                if (newlang)
                {
                    pstRet = setlocale(LC_MESSAGES, newlang);
                }
                else
                {
                    pstRet = setlocale(LC_MESSAGES, pstLang);
                }

                ret = to_wide_string(pstRet);
#else
                /* Load the user locale from the system */
                char* ret = getLocaleUserInfo();


#endif

                // This stuff causes pb when locales have been compiled
                if (ret == NULL)
                {
#ifndef _MSC_VER
                    fprintf(stderr, "Warning: Localization issue. Does not support the locale '%s'\nReturned: NULL\nCurrent system locale: %s\nDid you install the system locales?\n", lang,
                            setlocale(LC_MESSAGES, NULL));
#else
                    fprintf(stderr, "Warning: Localization issue. Cannot detect user locale.\n");
#endif
                }

                /* change language */
                if (strcmp(lang, "C") == 0 || ret == NULL || strcmp(ret, "C") == 0)
                {
                    /* The lang is the default one... ie en_US */
                    strcpy(CURRENTLANGUAGESTRING, SCILABDEFAULTLANGUAGE);
                    exportLocaleToSystem(CURRENTLANGUAGESTRING);
                }
                else
                {
                    if (strcmp(lang, "") == 0)
                    {
                        /* The requested language is the one of the system ...
                         * which we don't really know which one is it
                         * but if setlocale worked, we get it from the return
                         */
                        strncpy(CURRENTLANGUAGESTRING, ret, 5); /* 5 is the number of char in fr_FR for example */
                        exportLocaleToSystem(ret);
                    }
                    else
                    {
#if !defined(_MSC_VER)
                        if (newlang)
                        {
                            setenvc("LANG", newlang);
                            strncpy(CURRENTLANGUAGESTRING, newlang, 5);
                            CURRENTLANGUAGESTRING[5] = '\0';
                            exportLocaleToSystem(newlang);
                        }
                        else
#endif
                        {
                            strcpy(CURRENTLANGUAGESTRING, lang);
                            exportLocaleToSystem(lang);
                        }
                    }
                }
#ifndef _MSC_VER
                setlanguagecode(CURRENTLANGUAGESTRING);
#ifndef __APPLE__
                if (newlang)
                {
                    FREE(newlang);
                }
#endif
                FREE(pstLang);
#endif
                FREE(ret);
                return TRUE;
            }
#ifndef _MSC_VER
        }
#endif
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
char* getlanguage(void)
{
    return os_strdup(CURRENTLANGUAGESTRING);
}
/*--------------------------------------------------------------------------*/
int getcurrentlanguagecode(void)
{
    return CURRENTLANGUAGECODE;
}
/*--------------------------------------------------------------------------*/
const char* getlanguagefromcode(int code)
{
    int i = 0;

    for (i = 0 ; i < NumberLanguages ; i++)
    {
        if (LANGUAGE_COUNTRY_TAB[i].code == code)
        {
            return LANGUAGE_COUNTRY_TAB[i].alphacode;
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
int getcodefromlanguage(const char* language)
{
    return FindLanguageCode(language);
}
/*--------------------------------------------------------------------------*/
BOOL LanguageIsOK(const char* lang)
{
    int i = 0;

    if (strlen(lang) == 0)
    {
        /* Empty language declaration... it is the default
        * language from the system */
        return TRUE;
    }

    for (i = 0 ; i < NumberLanguages ; i++)
    {
        if (strcmp(lang, LANGUAGE_COUNTRY_TAB[i].alphacode) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static int FindLanguageCode(const char* lang)
{
    int i = 0;

    for (i = 0 ; i < NumberLanguages ; i++)
    {
        if (strcmp(lang, LANGUAGE_COUNTRY_TAB[i].alphacode) == 0)
        {
            return LANGUAGE_COUNTRY_TAB[i].code;
        }
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
static BOOL setlanguagecode(const char* lang)
{
    int tmpCode = FindLanguageCode(lang);

    if (tmpCode > 0)
    {
        CURRENTLANGUAGECODE = tmpCode;
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static const char* FindAlias(const char* lang)
{
    int i = 0;
    for (i = 0 ; i < NumberLanguagesAlias ; i++)
    {
        if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alphacode, lang) == 0)
        {
            return LANGUAGE_LOCALE_ALIAS[i].alias;
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
static const char* GetLanguageFromAlias(const char* langAlias)
{
    int i = 0;
    for (i = 0 ; i < NumberLanguagesAlias ; i++)
    {
        if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alias, langAlias) == 0)
        {
            return LANGUAGE_LOCALE_ALIAS[i].alphacode;
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
const char* getlanguagealias(void)
{
    return FindAlias(CURRENTLANGUAGESTRING);
}
/*--------------------------------------------------------------------------*/
BOOL needtochangelanguage(const char* language)
{
    char* currentlanguage = getlanguage();
    if (strcmp(language, currentlanguage))
    {
        free(currentlanguage);
        return TRUE;
    }

    free(currentlanguage);
    return FALSE;
}
/*--------------------------------------------------------------------------*/
const char* convertlanguagealias(const char* strlanguage)
{
    const char* correctlanguage = NULL;

    if ( (strlen(strlanguage) == 2) || (strcmp(strlanguage, "en_US") == 0) ) /* If the user wants to change to en_US ... use the default locale */
    {
        correctlanguage = GetLanguageFromAlias(strlanguage);
    }
    else
    {
        if (strcmp(strlanguage, "eng") == 0) /* compatibility previous scilab */
        {
            correctlanguage = GetLanguageFromAlias("en");
        }
        else
        {
            if (strlen(strlanguage) == 5 && strlanguage[2] == '_')
            {
                /* already xx_XX (fr_FR) */
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
BOOL exportLocaleToSystem(const char* locale)
{

    if (locale == NULL)
    {
#ifdef _MSC_VER
        fprintf(stderr, "Localization: Have not been able to find a suitable locale. Remains to default %s.\n", "LC_CTYPE");
#else
        fprintf(stderr, "Localization: Have not been able to find a suitable locale. Remains to default %s.\n", EXPORTENVLOCALESTR);
#endif
        return FALSE;
    }

    /* It will put in the env something like LC_MESSAGES=fr_FR */
    if ( !setenvc(EXPORTENVLOCALESTR, locale))
    {
#ifdef _MSC_VER
        fprintf(stderr, "Localization: Failed to declare the system variable %s.\n", "LC_CTYPE");
#else
        fprintf(stderr, "Localization: Failed to declare the system variable %d.\n", EXPORTENVLOCALE);
#endif
        return FALSE;
    }

#ifdef _MSC_VER
#ifdef USE_SAFE_GETTEXT_DLL
    {
        /* gettext is buggy on Windows */
        /* We need to set a external environment variable to scilab env. */
        char env[MAX_PATH];
        sprintf_s(env, MAX_PATH, "%s=%s", EXPORTENVLOCALESTR, locale);
        gettext_putenv(env);
    }
#endif
#else
    /* Export LC_NUMERIC to the system to make sure that the rest of system
       is using the english notation (Java, Tcl ...) */
    setenvc("LC_NUMERIC", LCNUMERICVALUE);
#endif

    return TRUE;
}
/*--------------------------------------------------------------------------*/
