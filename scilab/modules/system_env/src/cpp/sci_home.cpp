/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "sci_home.h"

#include "configvariable.hxx"

#include "string.hxx"
#include "context.hxx"
#include "setenvvar.hxx"

extern "C"
{
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "os_wcsdup.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "machine.h"
#include "version.h"
#include "setenvc.h"
#include "getenvc.h"
}

/*--------------------------------------------------------------------------*/
wchar_t* getSCIHOMEW(void)
{
    return os_wcsdup(ConfigVariable::getSCIHOME().c_str());
}
/*--------------------------------------------------------------------------*/
char* getSCIHOME(void)
{
    return wide_string_to_UTF8(ConfigVariable::getSCIHOME().c_str());
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
    symbol::Context::getInstance()->put(L"SCIHOME", *pS);

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
#define BASEDIR L"Scilab"
    int ierr = 0;
    int buflen = PATH_MAX;
    int iflag = 0;

    char USERPATHSCILAB[PATH_MAX];
    char SCIHOMEPATH[PATH_MAX * 2];
    char* SHORTUSERHOMESYSTEM = NULL;

    char USERHOMESYSTEM[PATH_MAX];

    bool bConverted = false;

    getenvc(&ierr, "APPDATA", USERHOMESYSTEM, &buflen, &iflag);

    /* if APPDATA not found we try with USERPROFILE */
    if(ierr)
    {
        getenvc(&ierr, "USERPROFILE", USERHOMESYSTEM, &buflen, &iflag);
    }

    /* convert long path to short path format : remove some special characters */
    SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM, &bConverted);
    if(SHORTUSERHOMESYSTEM)
    {
        if(!isdir(SHORTUSERHOMESYSTEM))
        {
            /* last chance, we try to get default all users profile */
            getenvc(&ierr, "ALLUSERSPROFILE", USERHOMESYSTEM, &buflen, &iflag);
            if(ierr)
            {
                delete []SHORTUSERHOMESYSTEM;
                return NULL;
            }

            /* convert long path to short path format : remove some special characters */
            SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM, &bConverted);

            if((!SHORTUSERHOMESYSTEM) || !isdir(SHORTUSERHOMESYSTEM))
            {
                if(SHORTUSERHOMESYSTEM)
                {
                    delete []SHORTUSERHOMESYSTEM;
                }
                return NULL;
            }
        }
    }
    else
    {
        if(SHORTUSERHOMESYSTEM)
        {
            delete []SHORTUSERHOMESYSTEM;
        }
        return NULL;
    }

    /* checks that directory exists */
    strcpy(USERHOMESYSTEM, SHORTUSERHOMESYSTEM);
    if(SHORTUSERHOMESYSTEM)
    {
        delete []SHORTUSERHOMESYSTEM;
    }

    /* Set SCIHOME environment variable */
    sprintf(USERPATHSCILAB, "%s%s%s", USERHOMESYSTEM, DIR_SEPARATOR, BASEDIR);
    sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);

    /* creates directory if it does not exists */
    if(!isdir(SCIHOMEPATH))
    {
        if(!isdir(USERPATHSCILAB))
        {
            createdirectory(USERPATHSCILAB);
        }

        if(createdirectory(SCIHOMEPATH))
        {

            return strdup(SCIHOMEPATH);
        }
    }
    else
    {
        return strdup(SCIHOMEPATH);
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
    if(ierr)
    {
        return NULL;
    }

    /* Set SCIHOME environment variable */
    sprintf(USERPATHSCILAB, "%s%s%s", USERHOMESYSTEM, DIR_SEPARATOR, BASEDIR);
    sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);

    /* creates directory if it does not exists */
    if(!isdir(SCIHOMEPATH))
    {
        if(!isdir(USERPATHSCILAB))
        {
            createdirectory(USERPATHSCILAB);
        }

        if(createdirectory(SCIHOMEPATH))
        {
            return strdup(SCIHOMEPATH);
        }
    }
    else
    {
        return strdup(SCIHOMEPATH);
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

    if(SciHome)
    {
        getenvc(&ierr, "SCIHOME", SciHome, &lbuf, &iflag);

        if(ierr == 1)
        {
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

    CopyOfDefaultPath = new char[strlen(_sci_home) + 1];
    if (CopyOfDefaultPath)
    {
        /* to be sure that it's unix 8.3 format */
        /* c:/progra~1/scilab-5.0 */
        bool bConvertOK = false;
        ShortPath = getshortpathname(_sci_home, &bConvertOK);
        AntislashToSlash(ShortPath,CopyOfDefaultPath);
        setenvc("SCIHOME", ShortPath);
        if(CopyOfDefaultPath)
        {
            delete[] CopyOfDefaultPath;
            CopyOfDefaultPath = NULL;
        }

        if(ShortPath)
        {
            delete[] ShortPath;
            ShortPath = NULL;
        }
    }
    return;
}

/*--------------------------------------------------------------------------*/
void defineSCIHOME()
{
    wchar_t* sci_home = computeSCIHOMEW();
    setSCIHOMEW(sci_home);
    putenvSCIHOMEW(sci_home);
    FREE(sci_home);
}
