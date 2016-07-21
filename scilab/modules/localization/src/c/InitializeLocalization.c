/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include <libintl.h>
#include <locale.h>
#endif

#include "sci_malloc.h"
#include "InitializeLocalization.h"
#include "localization.h"
#include "sci_path.h"
#include "scilabDefaults.h"
#include "setgetlanguage.h"
#include "isdir.h"
#include "os_string.h"
#ifdef _MSC_VER
#include "LanguagePreferences_Windows.h"
#endif

//#define FORCE_LOCALE_EN_US
/*--------------------------------------------------------------------------*/

BOOL InitializeLocalization(void)
{
#ifdef HAVE_LIBINTL_H

    char *SCIpath = getSCI();
    char *pathLocales = NULL;
    char *previousPathLocales = NULL;
    char *ret = NULL;

    /* set directory containing message catalogs */
    pathLocales = (char *)MALLOC(sizeof(char) * (strlen(SCIpath) + strlen(PATHLOCALIZATIONFILE) + 1));

    strcpy(pathLocales, SCIpath);
    strcat(pathLocales, PATHLOCALIZATIONFILE);
    if (bindtextdomain(NAMELOCALIZATIONDOMAIN, pathLocales) == NULL || !isdir(pathLocales))
    {
        /* source tree and classic build */
        previousPathLocales = os_strdup(pathLocales);
        FREE(pathLocales);
        pathLocales = NULL;

        pathLocales = (char *)MALLOC(sizeof(char) * (strlen(SCIpath) + strlen("/..") + strlen(PATHLOCALIZATIONFILE) + 1));
        strcpy(pathLocales, SCIpath);
        strcat(pathLocales, "/..");
        strcat(pathLocales, PATHLOCALIZATIONFILE);
        if (bindtextdomain(NAMELOCALIZATIONDOMAIN, pathLocales) == NULL || !isdir(pathLocales))
        {
            /* when it is installed on the system for example /usr/share/locale/ */
            fprintf(stderr, "Warning: Localization issue: Error while binding the domain from %s or %s: Switch to the default language (English).\n", pathLocales, previousPathLocales);

            // Set default behaviour
            textdomain(NAMELOCALIZATIONDOMAIN);
            bind_textdomain_codeset (NAMELOCALIZATIONDOMAIN, "UTF-8");
#ifndef _MSC_VER
            setlanguage(L"");
#else
            /* We look if registry value LANGUAGE exists */
            /* If not exists the "" means that we will try to use the language of the system.*/
            {
                wchar_t* loadLanguage = getLanguagePreferences();
                setlanguage(loadLanguage);
                if (loadLanguage)
                {
                    FREE(loadLanguage);
                    loadLanguage = NULL;
                }
            }
#endif

            if (previousPathLocales)
            {
                FREE(previousPathLocales);
                previousPathLocales = NULL;
            }
            FREE(pathLocales);
            pathLocales = NULL;
            FREE(SCIpath);
            SCIpath = NULL;
            return FALSE;
        }
        if (previousPathLocales)
        {
            FREE(previousPathLocales);
            previousPathLocales = NULL;
        }
    }

    FREE(SCIpath);
    SCIpath = NULL;
    FREE(pathLocales);
    pathLocales = NULL;

    /* set domain for future gettext() calls */
    ret = textdomain(NAMELOCALIZATIONDOMAIN);
    if (ret == NULL)
    {
        fprintf(stderr, "Localization: Error while declaring the text domain %s\n", NAMELOCALIZATIONDOMAIN);
        return FALSE;
    }
    bind_textdomain_codeset (NAMELOCALIZATIONDOMAIN, "UTF-8"); /*such that gettext and dgettext return UTF8 string*/
#ifndef _MSC_VER
    /* Here, the "" means that we will try to use the language of the system
     * first. If it doesn't work, we switch back to default (English) */
#ifdef FORCE_LOCALE_EN_US
    setlanguage(L"C");
#else
    setlanguage(L"");
#endif
#else
    /* We look if registry value LANGUAGE exists */
    /* If not exists the "" means that we will try to use the language of the system.*/
    {
#ifdef FORCE_LOCALE_EN_US
        setlanguage(L"en_US");
#else
        wchar_t *loadLanguage = getLanguagePreferences();
        setlanguage(loadLanguage);
        if (loadLanguage)
        {
            FREE(loadLanguage);
            loadLanguage = NULL;
        }
#endif
    }
#endif
    return TRUE;
#else
    fprintf(stderr, "Localization: setlocale didn't exist on the computer used to compile Scilab ! This is abnormal ! No localization will be working for this distribution of Scilab.\n");
    return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/

