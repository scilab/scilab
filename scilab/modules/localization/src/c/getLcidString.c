/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "LocaleNameToLCID_Windows.h"
#endif
#include "os_string.h"
#include "getLcidString.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "machine.h" /* bsiz */
/*--------------------------------------------------------------------------*/
#define DEFAULT_EN_LCID "0x0409"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* replaces '_' by '-' windows local format and convert to wide string */
static char* getWindowsLocaleFormat(const char *pStrLocale);
#endif
/*--------------------------------------------------------------------------*/
char *getLcidString(const char *pStrLocale)
{
    char *pStrLCID = NULL;

    if (pStrLocale == NULL)
    {
        return os_strdup(DEFAULT_EN_LCID);
    }
#ifdef _MSC_VER
    if ((strcmp(pStrLocale, "") == 0) || (strcmp(pStrLocale, "C") == 0))
    {
        return os_strdup(DEFAULT_EN_LCID);
    }
    else
    {
        char *locale = getWindowsLocaleFormat(pStrLocale);
        LCID lcid = dllLocaleNameToLCID(locale, 0);

        if (lcid == 0)
        {
            pStrLCID = os_strdup(DEFAULT_EN_LCID);
        }
        else
        {
            pStrLCID = (char *)MALLOC(sizeof(char) * bsiz);
            if (pStrLCID)
            {
                sprintf(pStrLCID, "0x0%x", lcid);
            }
        }
    }
#else
    /* this routines not used on others platforms */
    pStrLCID = os_strdup(DEFAULT_EN_LCID);
#endif
    return pStrLCID;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char* getWindowsLocaleFormat(const char *pStrLocale)
{
    char* ret = NULL;
    if (pStrLocale)
    {
        char* locale = os_strdup(pStrLocale);
        if (locale)
        {
            char* pos = strchr(locale, '_');
            if (pos)
            {
                *pos = '-';
            }
            ret = locale;
        }
    }
    return ret;
}
#endif
/*--------------------------------------------------------------------------*/
