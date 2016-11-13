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

#include <stdlib.h>

#include "configvariable.hxx"

#include "string.hxx"
#include "context.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "sci_path.h"
#include "os_string.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "setenvc.h"
#include "getenvc.h"
#include "setenvvar.h"
#include "getshortpathname.h"
}

char *getSCI(void)
{
    return os_strdup(ConfigVariable::getSCIPath().c_str());
}
/*--------------------------------------------------------------------------*/
void setSCI(const char* _sci_path)
{
    //
    char *ShortPath = NULL;
    char *pstSlash = new char[strlen(_sci_path) + 1];
    BOOL bConvertOK = FALSE;
    ShortPath = getshortpathname(_sci_path, &bConvertOK);
    AntislashToSlash(ShortPath, pstSlash);


    //SCI
    types::String *pSSCI = new types::String(pstSlash);
    symbol::Context::getInstance()->put(symbol::Symbol("SCI"), pSSCI);

    //WSCI
#ifdef _MSC_VER
    char *pstBackSlash = NULL;
    pstBackSlash = new char[strlen(_sci_path) + 1];
    SlashToAntislash(_sci_path, pstBackSlash);
    types::String *pSWSCI = new types::String(pstBackSlash);
    symbol::Context::getInstance()->put(symbol::Symbol("WSCI"), pSWSCI);
    delete[] pstBackSlash;
#else
    pstSlash = os_strdup(_sci_path);
#endif

    ConfigVariable::setSCIPath(pstSlash);

    if (pstSlash)
    {
        delete[] pstSlash;
    }
    if (ShortPath)
    {
        FREE(ShortPath);
    }
}
/*--------------------------------------------------------------------------*/
void putenvSCI(const char* _sci_path)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    /* to be sure that it's unix 8.3 format */
    /* c:/progra~1/scilab-5.0 */
    BOOL bConvertOK = FALSE;
    ShortPath = getshortpathname(_sci_path, &bConvertOK);

    CopyOfDefaultPath = new char[strlen(_sci_path) + 1];
    //GetShortPathName(_sci_path,ShortPath,PATH_MAX);
    AntislashToSlash(ShortPath, CopyOfDefaultPath);

    setenvc("SCI", ShortPath);

    delete[] CopyOfDefaultPath;
    FREE(ShortPath);
}
/*--------------------------------------------------------------------------*/
char* getenvSCI()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPath = new char[PATH_MAX];

    if (SciPath)
    {
        getenvc(&ierr, "SCI", SciPath, &lbuf, &iflag);

        if (ierr == 1)
        {
            delete[] SciPath;
            return NULL;
        }
    }

    return SciPath;
}
/*--------------------------------------------------------------------------*/
//windows : find main DLL and extract path
//linux and macos : scilab script fill SCI env variable
#ifdef _MSC_VER
char* computeSCI()
{
    char ScilabModuleName[MAX_PATH + 1];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    char SciPathName[_MAX_DRIVE + _MAX_DIR + 5];

    char *DirTmp = NULL;

    if (!GetModuleFileNameA((HINSTANCE)GetModuleHandleA("core"), ScilabModuleName, MAX_PATH))
    {
        return NULL;
    }

    os_splitpath(ScilabModuleName, drive, dir, fname, ext);

    if (dir[strlen(dir) - 1] == '\\')
    {
        dir[strlen(dir) - 1] = '\0';
    }

    DirTmp = strrchr(dir, '\\');
    if (strlen(dir) - strlen(DirTmp) > 0)
    {
        dir[strlen(dir) - strlen(DirTmp)] = '\0';
    }
    else
    {
        return NULL;
    }

    os_makepath(SciPathName, drive, dir, NULL, NULL);

    for (int i = 0 ; i < static_cast<int>(strlen(SciPathName)) ; i++)
    {
        if (SciPathName[i] == '\\')
        {
            SciPathName[i] = '/';
        }
    }
    SciPathName[strlen(SciPathName) - 1] = '\0';
    return os_strdup(SciPathName);
}
#else
char* computeSCI()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPathName = new char[PATH_MAX];

    if (SciPathName)
    {
        getenvc(&ierr, "SCI", SciPathName, &lbuf, &iflag);

        if (ierr == 1)
        {
            std::cerr << "SCI environment variable not defined." << std::endl;
            exit(1);
        }
    }

    return SciPathName;
}
#endif
/*--------------------------------------------------------------------------*/
void defineSCI()
{
    char* sci_path = computeSCI();
    setSCI(sci_path);
    putenvSCI(sci_path);
    FREE(sci_path);
}

