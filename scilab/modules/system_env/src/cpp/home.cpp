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

#include "home.h"

#include "configvariable.hxx"
#include "string.hxx"
#include "context.hxx"
#include "setenvvar.hxx"

extern "C"
{
#include "os_strdup.h"
#include "os_wcsdup.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "machine.h"
#include "version.h"
#include "setenvc.h"
#include "getenvc.h"
}

/*--------------------------------------------------------------------------*/
wchar_t* getHOMEW(void)
{
    return os_wcsdup(ConfigVariable::getHOME().c_str());
}
/*--------------------------------------------------------------------------*/
char* getHOME(void)
{
    return wide_string_to_UTF8(ConfigVariable::getHOME().c_str());
}
/*--------------------------------------------------------------------------*/
void setHOME(const char* _home)
{
    wchar_t* pstTemp = to_wide_string(_home);
    setHOMEW(pstTemp);
    FREE(pstTemp);
}
/*--------------------------------------------------------------------------*/
void setHOMEW(const wchar_t* _home)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_home);
    symbol::Context::getInstance()->put(symbol::Symbol(L"home"), *pS);

    std::wstring home(_home);
    ConfigVariable::setHOME(home);
}

/*--------------------------------------------------------------------------*/
wchar_t* computeHOMEW(void)
{
    char* pstTemp = computeHOME();
    wchar_t* pstReturn = to_wide_string(pstTemp);
    if (pstTemp)
    {
        delete[] pstTemp;
    }

    return pstReturn;
}
/*--------------------------------------------------------------------------*/
char* computeHOME(void)
{
    char *pstHOME = getenvHOME();
    if (pstHOME == NULL)
    {
        int ierr, iflag = 0;
        int lbuf = PATH_MAX;
        char *pstUserProfile = new char[PATH_MAX];
        getenvc(&ierr, "USERPROFILE", pstUserProfile, &lbuf, &iflag);
        if (ierr != 1)
        {
            return pstUserProfile;
        }
        else
        {
            /* if USERPROFILE is not defined , we use default profile */
            getenvc(&ierr, "ALLUSERSPROFILE", pstUserProfile, &lbuf, &iflag);
            if (ierr != 1)
            {
                return pstUserProfile;
            }
            else
            {
                return NULL;
            }
        }
    }
    return pstHOME;
}
/*--------------------------------------------------------------------------*/
char* getenvHOME(void)
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *Home = new char[PATH_MAX];

    if (Home)
    {
        getenvc(&ierr, "HOME", Home, &lbuf, &iflag);

        if (ierr == 1)
        {
            return NULL;
        }
    }
    return Home;
}
/*--------------------------------------------------------------------------*/
wchar_t* getenvHOMEW(void)
{
    char *Home = getenvHOME();
    wchar_t* pstTemp = to_wide_string(Home);
    delete[] Home;
    return pstTemp;
}
/*--------------------------------------------------------------------------*/
void putenvHOMEW(const wchar_t* _home)
{
    char* pstTemp = wide_string_to_UTF8(_home);
    putenvHOME(pstTemp);
    FREE(pstTemp);
    return;
}

void putenvHOME(const char* _home)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    CopyOfDefaultPath = new char[strlen(_home) + 1];
    if (CopyOfDefaultPath)
    {
        /* to be sure that it's unix 8.3 format */
        /* c:/progra~1/scilab-5.0 */
        bool bConvertOK = false;
        ShortPath = getshortpathname(_home, &bConvertOK);
        AntislashToSlash(ShortPath, CopyOfDefaultPath);
        setenvc("HOME", ShortPath);
        if (CopyOfDefaultPath)
        {
            delete[] CopyOfDefaultPath;
            CopyOfDefaultPath = NULL;
        }

        if (ShortPath)
        {
            delete[] ShortPath;
            ShortPath = NULL;
        }
    }
    return;
}

/*--------------------------------------------------------------------------*/
void defineHOME()
{
    wchar_t* home = computeHOMEW();
    setHOMEW(home);
    putenvHOMEW(home);
    FREE(home);
}
