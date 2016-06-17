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
}

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
void setSCIHOME(const char* _sci_home)
{
    wchar_t* pstTemp = to_wide_string(_sci_home);
    setSCIHOMEW(pstTemp);
    FREE(pstTemp);
}
/*--------------------------------------------------------------------------*/
void setSCIHOMEW(const wchar_t* _sci_home)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_home);
    symbol::Context::getInstance()->put(symbol::Symbol(L"SCIHOME"), pS);

    std::wstring sci_home(_sci_home);
    ConfigVariable::setSCIHOME(sci_home);
}

/*--------------------------------------------------------------------------*/
wchar_t* computeSCIHOMEW(void)
{
    char* pstTemp = computeSCIHOME();
    wchar_t* pstReturn = to_wide_string(pstTemp);
    FREE(pstTemp);
    return pstReturn;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
char* computeSCIHOME(void)
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
    os_sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);

    /* creates directory if it does not exists */
    if (!isdir(SCIHOMEPATH))
    {
        if (!isdir(USERPATHSCILAB))
        {
            createdirectory(USERPATHSCILAB);
        }

        if (createdirectory(SCIHOMEPATH))
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
char* computeSCIHOME(void)
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
    sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);

    /* creates directory if it does not exists */
    if (!isdir(SCIHOMEPATH))
    {
        if (!isdir(USERPATHSCILAB))
        {
            createdirectory(USERPATHSCILAB);
        }

        if (createdirectory(SCIHOMEPATH))
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
char* getenvSCIHOME(void)
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
wchar_t* getenvSCIHOMEW(void)
{
    char *SciHome = getenvSCIHOME();
    wchar_t* pstTemp = to_wide_string(SciHome);
    delete[] SciHome;
    return pstTemp;
}
/*--------------------------------------------------------------------------*/
void putenvSCIHOMEW(const wchar_t* _sci_home)
{
    char* pstTemp = wide_string_to_UTF8(_sci_home);
    putenvSCIHOME(pstTemp);
    FREE(pstTemp);
    return;
}

void putenvSCIHOME(const char* _sci_home)
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
void defineSCIHOME()
{
    wchar_t* sci_home = computeSCIHOMEW();
    setSCIHOMEW(sci_home);
    putenvSCIHOMEW(sci_home);
    FREE(sci_home);
}
