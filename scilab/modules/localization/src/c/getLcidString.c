/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
