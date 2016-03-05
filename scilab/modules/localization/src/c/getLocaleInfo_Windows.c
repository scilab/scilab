/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - Yung-Jang Lee
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
#include <windows.h>
#include <stdio.h>
#include "getLocaleInfo_Windows.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
char* getLocaleSystemInfo(void)
{
#define LENGTH_BUFFER 1024
    char buffer_LOCALE_SISO639LANGNAME[LENGTH_BUFFER];
    char buffer_LOCALE_SISO3166CTRYNAME[LENGTH_BUFFER];
    char *localeStr = NULL;
    int ret = 0;
    ret = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,
                        LOCALE_SISO639LANGNAME,
                        &buffer_LOCALE_SISO639LANGNAME[0],
                        LENGTH_BUFFER);
    if (ret > 0)
    {
        ret = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,
                            LOCALE_SISO3166CTRYNAME,
                            &buffer_LOCALE_SISO3166CTRYNAME[0],
                            LENGTH_BUFFER);
        if (ret > 0)
        {
            int length_localeStr = (int)(strlen(buffer_LOCALE_SISO639LANGNAME) +
                                         strlen(buffer_LOCALE_SISO3166CTRYNAME) +
                                         strlen("_"));
            localeStr = (char*)MALLOC(sizeof(char) * (length_localeStr) + 1);
            if (localeStr)
            {
#ifdef FORMAT_LOCALE
#undef FORMAT_LOCALE
#endif
#define FORMAT_LOCALE "%s_%s"
                sprintf(localeStr, FORMAT_LOCALE, buffer_LOCALE_SISO639LANGNAME, buffer_LOCALE_SISO3166CTRYNAME);
            }
        }
    }
    return localeStr;
}
/*--------------------------------------------------------------------------*/
wchar_t* getLocaleUserInfo(void)
{
#define LENGTH_BUFFER 1024
    wchar_t buffer_LOCALE_SISO639LANGNAME[LENGTH_BUFFER];
    wchar_t buffer_LOCALE_SISO3166CTRYNAME[LENGTH_BUFFER];
    wchar_t buffer_LOCALE_IDEFAULTANSICODEPAGE[LENGTH_BUFFER];
    wchar_t *localeStr = NULL;
    int ret = 0;
    ret = GetLocaleInfoW(LOCALE_USER_DEFAULT,
                         LOCALE_SISO639LANGNAME,
                         buffer_LOCALE_SISO639LANGNAME,
                         LENGTH_BUFFER);
    if (ret > 0)
    {

        ret = GetLocaleInfoW(LOCALE_USER_DEFAULT,
                             LOCALE_SISO3166CTRYNAME,
                             buffer_LOCALE_SISO3166CTRYNAME,
                             LENGTH_BUFFER);
        if (ret > 0)
        {
            // BY YJLee get Windows CODE Page Information
            int ret = GetLocaleInfoW(LOCALE_USER_DEFAULT,
                                     LOCALE_IDEFAULTANSICODEPAGE,
                                     buffer_LOCALE_IDEFAULTANSICODEPAGE,
                                     LENGTH_BUFFER);

            int length_localeStr = (int)(wcslen(buffer_LOCALE_SISO639LANGNAME) +
                                         wcslen(buffer_LOCALE_SISO3166CTRYNAME) +
                                         wcslen(buffer_LOCALE_IDEFAULTANSICODEPAGE) +
                                         wcslen(L"_.CP") );
            localeStr = (wchar_t*)MALLOC(sizeof(wchar_t) * (length_localeStr + 1));
            if (localeStr)
            {
#ifdef FORMAT_LOCALE
#undef FORMAT_LOCALE
#endif
#define FORMAT_LOCALE L"%s_%s.CP%s"
                // in lang_contry.CPxxx format
                os_swprintf(localeStr, length_localeStr + 1, FORMAT_LOCALE,
                            buffer_LOCALE_SISO639LANGNAME,
                            buffer_LOCALE_SISO3166CTRYNAME,
                            buffer_LOCALE_IDEFAULTANSICODEPAGE);
            }
        }
    }
    return localeStr;
}
/*--------------------------------------------------------------------------*/
