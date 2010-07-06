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

#include "sci_tmpdir.h"

#include "configvariable.hxx"

#include "setenvvar.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C"
{
#ifdef _MSC_VER
#include <process.h>
#include "strdup_windows.h"
#endif
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "setenvc.h"
#include "getenvc.h"
#include "localization.h"
#include <errno.h>
#include "removedir.h"
}

char *getTMPDIR(void)
{
    return strdup(ConfigVariable::getSCIPath().c_str());
}

/*--------------------------------------------------------------------------*/
wchar_t *getTMPDIRW(void)
{
    return to_wide_string(const_cast<char*>(ConfigVariable::getTMPDIR().c_str()));
}

/*--------------------------------------------------------------------------*/
void setTMPDIR(const char* _sci_tmpdir)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_tmpdir);
    symbol::Context::getInstance()->put("TMPDIR", *pS);

    //add SCI value ConfigVariable
    std::string sci_tmpdir(_sci_tmpdir);
    ConfigVariable::setTMPDIR(sci_tmpdir);
}

/*--------------------------------------------------------------------------*/
void putenvTMPDIR(const char *_sci_tmpdir)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    CopyOfDefaultPath = new char[strlen(_sci_tmpdir) + 1];
    if (CopyOfDefaultPath)
    {
        /* to be sure that it's unix 8.3 format */
        /* c:/progra~1/scilab-5.0 */
        bool bConvertOK = false;
        ShortPath = getshortpathname(_sci_tmpdir, &bConvertOK);
        //GetShortPathName(_sci_path,ShortPath,PATH_MAX);
        AntislashToSlash(ShortPath,CopyOfDefaultPath);
        setenvc("TMPDIR", ShortPath);
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
char* getenvTMPDIR()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPath = new char[PATH_MAX];

    if(SciPath)
    {
        getenvc(&ierr, "TMPDIR", SciPath, &lbuf, &iflag);

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
char* computeTMPDIR()
{
#ifdef _MSC_VER
    wchar_t wcTmpDirDefault[PATH_MAX];

    if(!GetTempPathW(PATH_MAX,wcTmpDirDefault))
    {
        MessageBox(NULL, _("Cannot find Windows temporary directory (1)."), _("Error"), MB_ICONERROR);
        exit(1);
    }
    else
    {
        wchar_t wctmp_dir[PATH_MAX+FILENAME_MAX + 1];
        static wchar_t bufenv[PATH_MAX + 16];
        char *TmpDir = NULL;
        swprintf(wctmp_dir, PATH_MAX+FILENAME_MAX + 1, L"%sSCI_TMP_%d_", wcTmpDirDefault, _getpid());
        if( CreateDirectoryW(wctmp_dir, NULL) == FALSE)
        {
            DWORD attribs = GetFileAttributesW(wctmp_dir);
            if(attribs & FILE_ATTRIBUTE_DIRECTORY)
            {
                /* Repertoire existant */
            }
            else
            {
#ifdef _DEBUG
                {
                    char MsgErr[1024];
                    wsprintf(MsgErr, _("Impossible to create : %s"), wctmp_dir);
                    MessageBox(NULL, MsgErr, _("Error"), MB_ICONERROR);
                    exit(1);
                }
#else
                {
                    GetTempPathW(PATH_MAX, wcTmpDirDefault);
                    wcscpy(wctmp_dir, wcTmpDirDefault);
                    wctmp_dir[wcslen(wctmp_dir) - 1] = '\0'; /* Remove last \ */
                }
#endif
            }
        }

        swprintf(bufenv, PATH_MAX + 16, L"TMPDIR=%s", wctmp_dir);
        _wputenv(bufenv);

        TmpDir = wide_string_to_UTF8(wctmp_dir);
        if (TmpDir)
        {
            return TmpDir;
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
#else
    char *tmpdir;

    char* env_dir = (char*)MALLOC(sizeof(char) * (PATH_MAX + 16));
    /* If the env variable TMPDIR is set, honor this preference */
    tmpdir = getenv("TMPDIR");
    if(tmpdir != NULL && strlen(tmpdir) < (PATH_MAX))
    {
        strcpy(env_dir, tmpdir);
    }
    else
    {
        strcpy(env_dir, "/tmp");
    }

    /* XXXXXX will be randomized by mkdtemp */
    sprintf(env_dir, "%s/SCI_TMP_%d_XXXXXX", env_dir, (int) getpid());

    if(mkdtemp(env_dir) < 0)
    {
        fprintf(stderr,_("Error: Could not create %s: %s\n"), env_dir, strerror(errno));
    }

    setenvc("TMPDIR", env_dir);
    return env_dir;
#endif
}
/*--------------------------------------------------------------------------*/
void defineTMPDIR()
{
    char* sci_tmpdir = computeTMPDIR();
    setTMPDIR(sci_tmpdir);
    putenvTMPDIR(sci_tmpdir);
    FREE(sci_tmpdir);
}

void clearTMPDIR()
{
    removedir(getTMPDIR());
}