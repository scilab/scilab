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
#include "LanguagePreferences_Windows.h"
#include "os_string.h"
#include "charEncoding.h"
#include "setgetlanguage.h"
#include "version.h"
#include "sci_malloc.h"
#include "GetWindowsVersion.h"
/*--------------------------------------------------------------------------*/
#define HKCU_LANGUAGE_FORMAT "SOFTWARE\\Scilab\\%s\\Settings" /* r/w registry */
#define HKCM_LANGUAGE_FORMAT "SOFTWARE\\Scilab\\%s" /* only read registry */
#define LANGUAGE_ENTRY "LANGUAGE"
#define DEFAULT_LANGUAGE_VALUE "en_US"
/*--------------------------------------------------------------------------*/
static char* languageFromCommandLine = NULL;
/*--------------------------------------------------------------------------*/
static char* getLanguagePreferencesCurrentUser(void);
static char* getLanguagePreferencesAllUsers(void);
static char* readRegistryLanguage(HKEY hKeyRoot, const char* keyString);
/*--------------------------------------------------------------------------*/
BOOL isValidLanguage(const char* lang)
{
    if (lang)
    {
        if (strcmp(lang, "") == 0 || strcmp(lang, "C") == 0)
        {
            return TRUE;
        }

        /* xx_XX */
        if (((int)strlen(lang) == 5) && (lang[2] == L'_'))
        {
            return TRUE;
        }
        else if ((strlen(lang) == 2) && (convertlanguagealias(lang)))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL setLanguageFromCommandLine(const char* lang)
{
    if (lang)
    {
        if (languageFromCommandLine)
        {
            FREE(languageFromCommandLine);
            languageFromCommandLine = NULL;
        }

        languageFromCommandLine = os_strdup(convertlanguagealias(lang));

        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
char* getLanguagePreferences(void)
{
    char* LanguageUser = NULL;

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
        char*LanguageAllUsers = getLanguagePreferencesAllUsers();

        if (LanguageAllUsers == NULL)
        {
            return os_strdup("");
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
    return os_strdup("");
}
/*--------------------------------------------------------------------------*/
static char* readRegistryLanguage(HKEY hKeyRoot, const char* keyStringFormat)
{
#define LENGTH_LANGUAGE_REGISTRY 64
    char LANGUAGE_REGISTRY[LENGTH_LANGUAGE_REGISTRY] = DEFAULT_LANGUAGE_VALUE;
    char* keyString = NULL;
    int lenkeyString = (int)(strlen(keyStringFormat) + strlen(SCI_VERSION_STRING)) + 1;

    keyString = (char*)MALLOC(sizeof(char) * lenkeyString);

    if (keyString)
    {

        DWORD OpensKeyOptions = 0;
        HKEY hKey;
        int length = LENGTH_LANGUAGE_REGISTRY;
        sprintf_s(keyString, lenkeyString, keyStringFormat, SCI_VERSION_STRING);
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
        if (RegOpenKeyExA(hKeyRoot, keyString, 0, OpensKeyOptions, &hKey) != ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            if (keyString)
            {
                FREE(keyString);
                keyString = NULL;
            }
            return NULL;
        }

        if (RegQueryValueExA(hKey, LANGUAGE_ENTRY, 0, NULL, (LPBYTE)LANGUAGE_REGISTRY, &length) != ERROR_SUCCESS)
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
    return os_strdup(LANGUAGE_REGISTRY);
}
/*--------------------------------------------------------------------------*/
static char*getLanguagePreferencesCurrentUser(void)
{
    return readRegistryLanguage(HKEY_CURRENT_USER, HKCU_LANGUAGE_FORMAT);
}
/*--------------------------------------------------------------------------*/
static char*getLanguagePreferencesAllUsers(void)
{
    return readRegistryLanguage(HKEY_LOCAL_MACHINE, HKCM_LANGUAGE_FORMAT);
}
/*--------------------------------------------------------------------------*/
BOOL setLanguagePreferences(void)
{
    char* pstLang = getlanguage();
    DWORD length = ((DWORD)strlen(pstLang) + 1) * 2;

    if (pstLang)
    {
        char* keyString = NULL;
        int lenkeyString = (int)(strlen(HKCU_LANGUAGE_FORMAT) + strlen(SCI_VERSION_STRING)) + 1;
        keyString = (char*)MALLOC(sizeof(char) * lenkeyString);
        if (keyString)
        {
            DWORD OpensKeyOptions = 0;
            HKEY hKey;
            DWORD result = 0;
            sprintf_s(keyString, lenkeyString, HKCU_LANGUAGE_FORMAT, SCI_VERSION_STRING);

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
            if (RegCreateKeyExA(HKEY_CURRENT_USER, keyString, 0, NULL, REG_OPTION_NON_VOLATILE, OpensKeyOptions, NULL, &hKey, &result) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                if (keyString)
                {
                    FREE(keyString);
                    keyString = NULL;
                }

                free(pstLang);
                return FALSE;
            }

            if (RegSetValueExA(hKey, LANGUAGE_ENTRY, 0, REG_SZ, (LPBYTE)pstLang, length) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                if (keyString)
                {
                    FREE(keyString);
                    keyString = NULL;
                }

                free(pstLang);
                return FALSE;
            }

            RegCloseKey(hKey);
            if (keyString)
            {
                FREE(keyString);
                keyString = NULL;
            }

            free(pstLang);
            return TRUE;
        }

        free(pstLang);
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
