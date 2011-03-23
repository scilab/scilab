/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008-2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <Windows.h>
#include "LanguagePreferences_Windows.h"
#include "strdup_windows.h"
#include "setgetlanguage.h"
#include "version.h"
#include "MALLOC.h"
#include "GetWindowsVersion.h"
/*--------------------------------------------------------------------------*/ 
#define HKCU_LANGUAGE_FORMAT "SOFTWARE\\Scilab\\%s\\Settings" /* r/w registry */
#define HKCM_LANGUAGE_FORMAT "SOFTWARE\\Scilab\\%s" /* only read registry */
#define LANGUAGE_ENTRY "LANGUAGE"
#define DEFAULT_LANGUAGE_VALUE "en_US"
/*--------------------------------------------------------------------------*/ 
static char *languageFromCommandLine = NULL;
/*--------------------------------------------------------------------------*/ 
static char *getLanguagePreferencesCurrentUser(void);
static char *getLanguagePreferencesAllUsers(void);
static char *readRegistryLanguage(HKEY hKeyRoot,char *keyString);
/*--------------------------------------------------------------------------*/ 
BOOL isValidLanguage(char *lang)
{
    if (lang)
    {
        if ( strcmp(lang, "") == 0) return TRUE;
        if ( strcmp(lang, "C") == 0) return TRUE;
        /* xx_XX */
        if ( ((int) strlen(lang) == 5) && (lang[2] == '_') ) return TRUE;
        else if ((strlen(lang) == 2) && (convertlanguagealias(lang)))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL setLanguageFromCommandLine(char *lang)
{
    if (lang)
    {
        if (languageFromCommandLine)
        {
            FREE(languageFromCommandLine);
            languageFromCommandLine = NULL;
        }

        languageFromCommandLine = strdup(convertlanguagealias(lang));

        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
char *getLanguagePreferences(void)
{
    char *LanguageUser = NULL;

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
        char *LanguageAllUsers = getLanguagePreferencesAllUsers();

        if (LanguageAllUsers == NULL)
        {
            return strdup("");
        }
        else
        {
            if (isValidLanguage(LanguageAllUsers)) return LanguageAllUsers;
            else return strdup("");
        }
    }
    else
    {
        if (isValidLanguage(LanguageUser)) return LanguageUser;
        else return strdup("");
    }
    return strdup("");
}
/*--------------------------------------------------------------------------*/ 
static char *readRegistryLanguage(HKEY hKeyRoot,char *keyStringFormat)
{
#define LENGTH_LANGUAGE_REGISTRY 64
    char LANGUAGE_REGISTRY[LENGTH_LANGUAGE_REGISTRY] = DEFAULT_LANGUAGE_VALUE;
    char *keyString = NULL;
    int lenkeyString = (int)(strlen(keyStringFormat)+strlen(SCI_VERSION_STRING)) + 1;

    keyString = (char*) MALLOC(sizeof(char) * lenkeyString);

    if (keyString)
    {

        DWORD OpensKeyOptions = 0;
        HKEY hKey;
        DWORD result = 0;
        int length = LENGTH_LANGUAGE_REGISTRY;
        wsprintf(keyString, keyStringFormat, SCI_VERSION_STRING);
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
        if ( RegOpenKeyEx(hKeyRoot, keyString, 0, OpensKeyOptions, &hKey) != ERROR_SUCCESS )
        {
            RegCloseKey(hKey);
            if (keyString) { FREE(keyString); keyString = NULL;}
            return NULL;
        }

        if ( RegQueryValueEx(hKey, LANGUAGE_ENTRY, 0, NULL ,(LPBYTE)LANGUAGE_REGISTRY,&length)  !=  ERROR_SUCCESS )
        {
            RegCloseKey(hKey);
            if (keyString) { FREE(keyString); keyString = NULL;}
            return NULL;
        }

        RegCloseKey(hKey);
        if (keyString) { FREE(keyString); keyString = NULL;}
    }
    return strdup(LANGUAGE_REGISTRY);
}
/*--------------------------------------------------------------------------*/ 
static char *getLanguagePreferencesCurrentUser(void)
{
    return readRegistryLanguage(HKEY_CURRENT_USER, HKCU_LANGUAGE_FORMAT);
}
/*--------------------------------------------------------------------------*/ 
static char *getLanguagePreferencesAllUsers(void)
{
    return readRegistryLanguage(HKEY_LOCAL_MACHINE, HKCM_LANGUAGE_FORMAT);
}
/*--------------------------------------------------------------------------*/ 
BOOL setLanguagePreferences(void)
{
    char *LANGUAGE = getlanguage();

    if (LANGUAGE)
    {
        char *keyString = NULL;
        int lenkeyString = (int)(strlen(HKCU_LANGUAGE_FORMAT)+strlen(SCI_VERSION_STRING)) + 1;
        keyString = (char*) MALLOC(sizeof(char) * lenkeyString);
        if (keyString)
        {
            DWORD OpensKeyOptions = 0;
            HKEY hKey;
            DWORD result = 0;
            wsprintf(keyString, HKCU_LANGUAGE_FORMAT, SCI_VERSION_STRING);

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
            if ( RegCreateKeyEx(HKEY_CURRENT_USER, keyString, 0, NULL, REG_OPTION_NON_VOLATILE, OpensKeyOptions, NULL, &hKey, &result) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                if (keyString) { FREE(keyString); keyString = NULL;}
                return FALSE;
            }

            if ( RegSetValueEx(hKey, LANGUAGE_ENTRY, 0, REG_SZ, (LPBYTE)LANGUAGE, (DWORD)(strlen(LANGUAGE)+1)) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                if (keyString) { FREE(keyString); keyString = NULL;}
                return FALSE;
            }

            RegCloseKey(hKey);
            if (keyString) { FREE(keyString); keyString = NULL;}
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
