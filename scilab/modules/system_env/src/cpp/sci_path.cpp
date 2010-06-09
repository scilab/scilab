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

#include <stdlib.h>

#include "sci_path.h"

#include "configvariable.hxx"

#include "setenvvar.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C"
{
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "setenvc.h"
#include "getenvc.h"
}

char *getSCI(void)
{
    return strdup(ConfigVariable::getSCIPath().c_str());
}

/*--------------------------------------------------------------------------*/
wchar_t *getSCIW(void)
{
    return to_wide_string(const_cast<char*>(ConfigVariable::getSCIPath().c_str()));
}

/*--------------------------------------------------------------------------*/
void setSCI(const char* _sci_path)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_path);
    symbol::Context::getInstance()->put("SCI", *pS);

    //add SCI value ConfigVariable
    std::string sci_path(_sci_path);
    ConfigVariable::setSCIPath(sci_path);
}

/*--------------------------------------------------------------------------*/
void putenvSCI(const char *_sci_path)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    CopyOfDefaultPath = new char[strlen(_sci_path) + 1];
    if (CopyOfDefaultPath)
    {
        /* to be sure that it's unix 8.3 format */
        /* c:/progra~1/scilab-5.0 */
        bool bConvertOK = false;
        ShortPath = getshortpathname(_sci_path, &bConvertOK);
        //GetShortPathName(_sci_path,ShortPath,PATH_MAX);
        AntislashToSlash(ShortPath,CopyOfDefaultPath);
        setenvc("SCI", ShortPath);
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
char* getenvSCI()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPath = new char[PATH_MAX];

    if(SciPath)
    {
        getenvc(&ierr, "SCI", SciPath, &lbuf, &iflag);

        if(ierr == 1)
        {
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

    char *SciPathName=NULL;
    char *DirTmp=NULL;

    if(!GetModuleFileName((HINSTANCE)GetModuleHandle("libScilab"), ScilabModuleName, MAX_PATH))
    {
        return NULL;
    }

    _splitpath(ScilabModuleName, drive, dir, fname, ext);

    if(dir[strlen(dir) - 1] == '\\')
    {
        dir[strlen(dir) - 1] = '\0';
    }

    DirTmp = strrchr(dir, '\\');
    if(strlen(dir) - strlen(DirTmp) > 0)
    {
        dir[strlen(dir) - strlen(DirTmp)] = '\0';
    }
    else
    {
        return NULL;
    }

    SciPathName = new char[strlen(drive) + strlen(dir) + 5];
    if (SciPathName)
    {
        _makepath(SciPathName, drive, dir, NULL, NULL);

        for(int i = 0 ; i < static_cast<int>(strlen(SciPathName)) ; i++)
        {
            if(SciPathName[i] == '\\')
            {
                SciPathName[i] = '/';
            }
        }
        SciPathName[strlen(SciPathName) - 1] = '\0';
    }
    return SciPathName;
}
#else
char* computeSCI()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPathName = new char[PATH_MAX];

    if(SciPathName)
    {
        getenvc(&ierr, "SCI", SciPathName, &lbuf, &iflag);

        if(ierr == 1)
        {
            cerr << "SCI environment variable not defined." << endl;
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
    delete[] sci_path;
}

