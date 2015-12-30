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


#include "configvariable.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C"
{
#include "home.h"
#include "sci_malloc.h"
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
char* getHOME(void)
{
    return os_strdup(ConfigVariable::getHOME().c_str());
}
/*--------------------------------------------------------------------------*/
void setHOME(const char* _home)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_home);
    symbol::Context::getInstance()->put(symbol::Symbol("home"), pS);

    ConfigVariable::setHOME(_home);
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
            delete[] Home;
            return NULL;
        }
    }
    return Home;
}
/*--------------------------------------------------------------------------*/
void putenvHOME(const char* _home)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    /* to be sure that it's unix 8.3 format */
    /* c:/progra~1/scilab-5.0 */
    BOOL bConvertOK = FALSE;
    ShortPath = getshortpathname(_home, &bConvertOK);

    CopyOfDefaultPath = new char[strlen(_home) + 1];
    AntislashToSlash(ShortPath, CopyOfDefaultPath);

    setenvc("HOME", ShortPath);

    delete[] CopyOfDefaultPath;
    FREE(ShortPath);
}

/*--------------------------------------------------------------------------*/
void defineHOME()
{
    char* home = computeHOME();
    setHOME(home);
    putenvHOME(home);
    FREE(home);
}
