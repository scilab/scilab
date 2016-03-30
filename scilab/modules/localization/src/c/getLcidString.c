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
static wchar_t *getWindowsLocaleFormat(const char *pStrLocale);
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
        wchar_t *wLocale = getWindowsLocaleFormat(pStrLocale);
        LCID lcid = dllLocaleNameToLCID(wLocale, 0);

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
