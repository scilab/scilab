/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "LocaleNameToLCID_Windows.h"
#include "strdup_windows.h"
#endif
#include "getLcidString.h"
#include "MALLOC.h"
#include "charEncoding.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/
#define DEFAULT_EN_LCID "0x0409"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* replaces '_' by '-' windows local format and convert to wide string */
static wchar_t *getWindowsLocaleFormat(const char *pStrLocale);
#endif
/*--------------------------------------------------------------------------*/
char *getLcidString(const char *pStrLocale)
{
    char *pStrLCID = NULL;

    if (pStrLocale == NULL) 
    {
        return strdup(DEFAULT_EN_LCID);
    }
#ifdef _MSC_VER
    if ((strcmp(pStrLocale, "") == 0) || (strcmp(pStrLocale, "C") == 0))
    {
        return strdup(DEFAULT_EN_LCID);
    }
    else
    {
        wchar_t *wLocale = getWindowsLocaleFormat(pStrLocale);
        LCID lcid = dllLocaleNameToLCID(wLocale, 0);

        if (lcid == 0)
        {
            pStrLCID = strdup(DEFAULT_EN_LCID);
        }
        else
        {
            pStrLCID = (char *)MALLOC(sizeof(char)*bsiz);
            if (pStrLCID)
            {
                sprintf(pStrLCID, "0x0%x", lcid);
            }
        }
    }
#else
    /* this routines not used on others platforms */
    pStrLCID = strdup(DEFAULT_EN_LCID);
#endif
    return pStrLCID;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static wchar_t *getWindowsLocaleFormat(const char *pStrLocale)
{
    wchar_t *Locale = NULL;
    if (pStrLocale)
    {
        wchar_t *pwStrLocale = to_wide_string(pStrLocale);
        if (pwStrLocale)
        {
            wchar_t *pos = wcschr(pwStrLocale, L'_');
            if (pos)
            {
                *pos = L'-';
            }
            Locale = pwStrLocale;
        }
    }
    return Locale;
}
#endif
/*--------------------------------------------------------------------------*/
