/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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


#include "configvariable.hxx"

#include "string.hxx"
#include "context.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "sci_home.h"
#include "os_string.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "machine.h"
#include "version.h"
#include "setenvc.h"
#include "getenvc.h"
#include "setenvvar.h"
#include "getshortpathname.h"
#include "sciprint.h"
#include "isdir.h"
#include "createdirectory.h"
#ifndef _MSC_VER
#include <sys/stat.h>
#else
#include <Shlwapi.h>
#endif
}

static void setSCIHOMEW(const wchar_t* _sci_home);
static void putenvSCIHOME(const char* _sci_home);
static char* computeSCIHOME(const char* path = "");
static wchar_t* computeSCIHOMEW(const wchar_t* path = L"");
static bool createDirectoryRecursivelyW(const std::wstring& path);
static bool createDirectoryRecursively(const std::string& path);


/*--------------------------------------------------------------------------*/
wchar_t* getSCIHOMEW(void)
{
    return os_wcsdup(ConfigVariable::getSCIHOME().c_str());
}
/*--------------------------------------------------------------------------*/
char* getSCIHOME(void)
{
    std::wstring tmpSCIHOME = ConfigVariable::getSCIHOME();
    if (tmpSCIHOME == L"")
    {
        tmpSCIHOME = L"empty_SCIHOME";
    }
    return wide_string_to_UTF8(tmpSCIHOME.c_str());
}
/*--------------------------------------------------------------------------*/
static void setSCIHOME(const char* _sci_home)
{
    wchar_t* pstTemp = to_wide_string(_sci_home);
    setSCIHOMEW(pstTemp);
    FREE(pstTemp);
}
/*--------------------------------------------------------------------------*/
static void setSCIHOMEW(const wchar_t* _sci_home)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_home);
    symbol::Context::getInstance()->put(symbol::Symbol(L"SCIHOME"), pS);

    std::wstring sci_home(_sci_home);
    ConfigVariable::setSCIHOME(sci_home);
}

/*--------------------------------------------------------------------------*/
static wchar_t* computeSCIHOMEW(const wchar_t* path)
{
    char* pstHome = wide_string_to_UTF8(path);
    char* pstTemp = computeSCIHOME(pstHome);
    wchar_t* pstReturn = to_wide_string(pstTemp);
    FREE(pstTemp);
    FREE(pstHome);
    return pstReturn;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char* computeSCIHOME(const char* path)
{
#define BASEDIR "Scilab"
    int ierr = 0;
    int buflen = PATH_MAX;
    int iflag = 0;

    char USERPATHSCILAB[PATH_MAX];
    char SCIHOMEPATH[PATH_MAX * 2];
    char* SHORTUSERHOMESYSTEM = NULL;

    char USERHOMESYSTEM[PATH_MAX];

    BOOL bConverted = FALSE;

    getenvc(&ierr, "APPDATA", USERHOMESYSTEM, &buflen, &iflag);

    /* if APPDATA not found we try with USERPROFILE */
    if (ierr)
    {
        getenvc(&ierr, "USERPROFILE", USERHOMESYSTEM, &buflen, &iflag);
    }

    /* convert long path to short path format : remove some special characters */
    SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM, &bConverted);
    if (SHORTUSERHOMESYSTEM)
    {
        if (!isdir(SHORTUSERHOMESYSTEM))
        {
            /* last chance, we try to get default all users profile */
            getenvc(&ierr, "ALLUSERSPROFILE", USERHOMESYSTEM, &buflen, &iflag);
            if (ierr)
            {
                delete []SHORTUSERHOMESYSTEM;
                return NULL;
            }

            /* convert long path to short path format : remove some special characters */
            SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM, &bConverted);

            if ((!SHORTUSERHOMESYSTEM) || !isdir(SHORTUSERHOMESYSTEM))
            {
                if (SHORTUSERHOMESYSTEM)
                {
                    delete []SHORTUSERHOMESYSTEM;
                }
                return NULL;
            }
        }
    }
    else
    {
        if (SHORTUSERHOMESYSTEM)
        {
            delete []SHORTUSERHOMESYSTEM;
        }
        return NULL;
    }

    /* checks that directory exists */
    os_strcpy(USERHOMESYSTEM, SHORTUSERHOMESYSTEM);
    if (SHORTUSERHOMESYSTEM)
    {
        delete []SHORTUSERHOMESYSTEM;
    }

    /* Set SCIHOME environment variable */
    os_sprintf(USERPATHSCILAB, "%s%s%s", USERHOMESYSTEM, DIR_SEPARATOR, BASEDIR);
    if (path && path[0] != '\0')
    {
        os_sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, path);
    }
    else
    {
        os_sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);
    }

    /* creates directory if it does not exist */
    if (!isdir(SCIHOMEPATH))
    {
        if (!isdir(USERPATHSCILAB))
        {
            createDirectoryRecursively(USERPATHSCILAB);
        }

        if (createDirectoryRecursively(SCIHOMEPATH))
        {

            return os_strdup(SCIHOMEPATH);
        }
    }
    else
    {
        return os_strdup(SCIHOMEPATH);
    }

    return NULL;
}
#else
static char* computeSCIHOME(const char* path)
{
#define BASEDIR ".Scilab"
    int ierr   = 0;
    int buflen = PATH_MAX;
    int iflag  = 0;
    char USERPATHSCILAB[PATH_MAX];
    char USERHOMESYSTEM[PATH_MAX];
    char SCIHOMEPATH[PATH_MAX * 2];
    char HOME[] = "HOME";

    getenvc(&ierr, HOME, USERHOMESYSTEM, &buflen, &iflag);
    if (ierr)
    {
        return NULL;
    }

    /* Set SCIHOME environment variable */
    sprintf(USERPATHSCILAB, "%s%s%s", USERHOMESYSTEM, DIR_SEPARATOR, BASEDIR);
    if (path && path[0] != '\0')
    {
        sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, path);
    }
    else
    {
        sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);
    }

    /* creates directory if it does not exist */
    if (!isdir(SCIHOMEPATH))
    {
        if (!isdir(USERPATHSCILAB))
        {
            createDirectoryRecursively(USERPATHSCILAB);
        }

        if (createDirectoryRecursively(SCIHOMEPATH))
        {
            return os_strdup(SCIHOMEPATH);
        }
    }
    else
    {
        return os_strdup(SCIHOMEPATH);
    }

    return NULL;
}
#endif

/*--------------------------------------------------------------------------*/
static char* getenvSCIHOME(void)
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciHome = new char[PATH_MAX];

    if (SciHome)
    {
        getenvc(&ierr, "SCIHOME", SciHome, &lbuf, &iflag);

        if (ierr == 1)
        {
            delete[] SciHome;
            return NULL;
        }
    }
    return SciHome;
}
/*--------------------------------------------------------------------------*/
static wchar_t* getenvSCIHOMEW(void)
{
    char *SciHome = getenvSCIHOME();
    wchar_t* pstTemp = to_wide_string(SciHome);
    delete[] SciHome;
    return pstTemp;
}
/*--------------------------------------------------------------------------*/
static void putenvSCIHOMEW(const wchar_t* _sci_home)
{
    char* pstTemp = wide_string_to_UTF8(_sci_home);
    putenvSCIHOME(pstTemp);
    FREE(pstTemp);
    return;
}

static void putenvSCIHOME(const char* _sci_home)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    /* to be sure that it's unix 8.3 format */
    /* c:/progra~1/scilab-5.0 */
    BOOL bConvertOK = FALSE;
    ShortPath = getshortpathname(_sci_home, &bConvertOK);

    CopyOfDefaultPath = new char[strlen(_sci_home) + 1];
    AntislashToSlash(ShortPath, CopyOfDefaultPath);

    setenvc("SCIHOME", ShortPath);

    delete[] CopyOfDefaultPath;
    FREE(ShortPath);
}
/*--------------------------------------------------------------------------*/
static bool createDirectoryRecursivelyW(const std::wstring& path)
{
    char* c = wide_string_to_UTF8(path.data());
    std::string s(c);
    FREE(c);
    return createDirectoryRecursively(s);
}

static bool createDirectoryRecursively(const std::string& path)
{
#ifdef _MSC_VER
    size_t pos = 0;
    do
    {
        pos = path.find_first_of("\\/", pos + 1);
        if (CreateDirectoryA(path.substr(0, pos).c_str(), NULL) == FALSE)
        {
            DWORD d = GetLastError();
            if (d == ERROR_PATH_NOT_FOUND)
            {
                return false;
            }
        }
    } while (pos != std::string::npos);

    return true;
#else
    char* file_path = os_strdup(path.data());

    char* p = NULL;
    for (p = strchr(file_path + 1, '/'); p; p = strchr(p + 1, '/'))
    {
        *p = '\0';
        if (mkdir(file_path, 0777) == -1)
        {
            if (errno != EEXIST)
            {
                *p = '/';
                FREE(file_path);
                return false;
            }
        }
        *p = '/';
    }

    //create final folder
    if (mkdir(path.data(), 0777) == -1)
    {
        if (errno != EEXIST)
        {
            if (p != NULL)
            { 
                *p = '/';
            }
            FREE(file_path);
            return false;
        }
    }

    FREE(file_path);
    return true;
#endif
}
/*--------------------------------------------------------------------------*/
static bool isAbsolutePath(const wchar_t* path)
{
#ifdef _MSC_VER
    if(PathIsRelative(path))
    {
        return false;
    }
    else
    {
        return true;
    }
#else
    return path[0] == L'/';
#endif
}

void defineSCIHOME()
{
    wchar_t* sci_home = getSCIHOMEW();
    if (wcscmp(sci_home, L"") == 0)
    {
        FREE(sci_home);
        sci_home = computeSCIHOMEW();
    }
    else
    {
        if (isAbsolutePath(sci_home))
        {
            if (createDirectoryRecursivelyW(sci_home) == false)
            {
                sciprint("Unable to create SCIHOME in `%ls`.\n", sci_home);
                sciprint("Back to normal behaviour.\n");
                FREE(sci_home);
                sci_home = computeSCIHOMEW();
            }
        }
        else
        {
            std::wstring w(sci_home);
            FREE(sci_home);
            sci_home = computeSCIHOMEW(w.data());
        }
    }

    setSCIHOMEW(sci_home);
    putenvSCIHOMEW(sci_home);
    FREE(sci_home);
}
