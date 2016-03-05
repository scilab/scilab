/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008-2010 - DIGITEO - Allan CORNET
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
#include <stdio.h>
#include <Windows.h>
#include <wchar.h>
#include "LanguagePreferences_Windows.h"
#include "os_string.h"
#include "charEncoding.h"
#include "setgetlanguage.h"
#include "version.h"
#include "sci_malloc.h"
#include "GetWindowsVersion.h"
/*--------------------------------------------------------------------------*/
#define HKCU_LANGUAGE_FORMAT L"SOFTWARE\\Scilab\\%s\\Settings" /* r/w registry */
#define HKCM_LANGUAGE_FORMAT L"SOFTWARE\\Scilab\\%s" /* only read registry */
#define LANGUAGE_ENTRY L"LANGUAGE"
#define DEFAULT_LANGUAGE_VALUE L"en_US"
/*--------------------------------------------------------------------------*/
static wchar_t *languageFromCommandLine = NULL;
/*--------------------------------------------------------------------------*/
static wchar_t *getLanguagePreferencesCurrentUser(void);
static wchar_t *getLanguagePreferencesAllUsers(void);
static wchar_t *readRegistryLanguage(HKEY hKeyRoot, wchar_t *keyString);
/*--------------------------------------------------------------------------*/
BOOL isValidLanguage(wchar_t *lang)
{
    if (lang)
    {
        if ( wcscmp(lang, L"") == 0 || wcscmp(lang, L"C") == 0 )
        {
            return TRUE;
        }

        /* xx_XX */
        if ( ((int) wcslen(lang) == 5) && (lang[2] == L'_') )
        {
            return TRUE;
        }
        else if ((wcslen(lang) == 2) && (convertlanguagealias(lang)))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL setLanguageFromCommandLine(wchar_t *lang)
{
    if (lang)
    {
        if (languageFromCommandLine)
        {
            FREE(languageFromCommandLine);
            languageFromCommandLine = NULL;
        }

        languageFromCommandLine = os_wcsdup(convertlanguagealias(lang));

        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
wchar_t *getLanguagePreferences(void)
{
    wchar_t *LanguageUser = NULL;

    if (languageFromCommandLine)
    {
        LanguageUser = languageFromCommandLine;
    }
    else
    {
        LanguageUser = getLanguagePreferencesCurrentUser();
    }

    if (LanguageUser == NULL)
    {
        wchar_t *LanguageAllUsers = getLanguagePreferencesAllUsers();

        if (LanguageAllUsers == NULL)
        {
            return os_wcsdup(L"");
        }
        else
        {
            if (isValidLanguage(LanguageAllUsers))
            {
                return LanguageAllUsers;
            }

            FREE(LanguageAllUsers);
        }
    }
    else
    {
        if (isValidLanguage(LanguageUser))
        {
            return LanguageUser;
        }

        FREE(LanguageUser);
    }
    return os_wcsdup(L"");
}
/*--------------------------------------------------------------------------*/
static wchar_t *readRegistryLanguage(HKEY hKeyRoot, wchar_t *keyStringFormat)
{
#define LENGTH_LANGUAGE_REGISTRY 64
    wchar_t LANGUAGE_REGISTRY[LENGTH_LANGUAGE_REGISTRY] = DEFAULT_LANGUAGE_VALUE;
    wchar_t *keyString = NULL;
    int lenkeyString = (int)(wcslen(keyStringFormat) + wcslen(SCI_VERSION_WIDE_STRING)) + 1;

    keyString = (wchar_t*) MALLOC(sizeof(wchar_t) * lenkeyString);

    if (keyString)
    {

        DWORD OpensKeyOptions = 0;
        HKEY hKey;
        int length = LENGTH_LANGUAGE_REGISTRY;
        wsprintfW(keyString, keyStringFormat, SCI_VERSION_WIDE_STRING);
#ifdef _WIN64 /* Scilab x64 on x64 windows */
        OpensKeyOptions = KEY_READ | KEY_WOW64_64KEY;
#else
        if (IsWow64()) /* Scilab 32 bits on x64 windows */
        {
            OpensKeyOptions = KEY_READ | KEY_WOW64_32KEY;
        }
        else /* Scilab 32 bits on windows 32 bits */
        {
            OpensKeyOptions = KEY_READ;
        }
#endif
        if ( RegOpenKeyExW(hKeyRoot, keyString, 0, OpensKeyOptions, &hKey) != ERROR_SUCCESS )
        {
            RegCloseKey(hKey);
            if (keyString)
            {
                FREE(keyString);
                keyString = NULL;
            }
            return NULL;
        }

        if ( RegQueryValueExW(hKey, LANGUAGE_ENTRY, 0, NULL , (LPBYTE)LANGUAGE_REGISTRY, &length)  !=  ERROR_SUCCESS )
        {
            RegCloseKey(hKey);
            if (keyString)
            {
                FREE(keyString);
                keyString = NULL;
            }
            return NULL;
        }

        RegCloseKey(hKey);
        if (keyString)
        {
            FREE(keyString);
            keyString = NULL;
        }
    }
    return os_wcsdup(LANGUAGE_REGISTRY);
}
/*--------------------------------------------------------------------------*/
static wchar_t *getLanguagePreferencesCurrentUser(void)
{
    return readRegistryLanguage(HKEY_CURRENT_USER, HKCU_LANGUAGE_FORMAT);
}
/*--------------------------------------------------------------------------*/
static wchar_t *getLanguagePreferencesAllUsers(void)
{
    return readRegistryLanguage(HKEY_LOCAL_MACHINE, HKCM_LANGUAGE_FORMAT);
}
/*--------------------------------------------------------------------------*/
BOOL setLanguagePreferences(void)
{
    wchar_t* pwstLang = getlanguage();
    DWORD length = ((DWORD)wcslen(pwstLang) + 1) * 2;

    if (pwstLang)
    {
        wchar_t *keyString = NULL;
        int lenkeyString = (int)(wcslen(HKCU_LANGUAGE_FORMAT) + wcslen(SCI_VERSION_WIDE_STRING)) + 1;
        keyString = (wchar_t*) MALLOC(sizeof(wchar_t) * lenkeyString);
        if (keyString)
        {
            DWORD OpensKeyOptions = 0;
            HKEY hKey;
            DWORD result = 0;
            wsprintfW(keyString, HKCU_LANGUAGE_FORMAT, SCI_VERSION_WIDE_STRING);

#ifdef _WIN64 /* Scilab x64 on x64 windows */
            OpensKeyOptions = KEY_ALL_ACCESS | KEY_WOW64_64KEY;
#else
            if ( IsWow64() ) /* Scilab 32 bits on x64 windows */
            {
                OpensKeyOptions = KEY_ALL_ACCESS | KEY_WOW64_32KEY;
            }
            else /* Scilab 32 bits on windows 32 bits */
            {
                OpensKeyOptions = KEY_ALL_ACCESS;
            }
#endif
            if ( RegCreateKeyExW(HKEY_CURRENT_USER, keyString, 0, NULL, REG_OPTION_NON_VOLATILE, OpensKeyOptions, NULL, &hKey, &result) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                if (keyString)
                {
                    FREE(keyString);
                    keyString = NULL;
                }

                free(pwstLang);
                return FALSE;
            }

            if (RegSetValueExW(hKey, LANGUAGE_ENTRY, 0, REG_SZ, (LPBYTE)pwstLang, length) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                if (keyString)
                {
                    FREE(keyString);
                    keyString = NULL;
                }

                free(pwstLang);
                return FALSE;
            }

            RegCloseKey(hKey);
            if (keyString)
            {
                FREE(keyString);
                keyString = NULL;
            }

            free(pwstLang);
            return TRUE;
        }

        free(pwstLang);
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
