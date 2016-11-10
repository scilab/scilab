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
#include "parser.hxx"

extern "C"
{
#include "sci_malloc.h"
#ifdef _MSC_VER
#include <process.h>
#else
#include <sys/types.h> /* getpid */
#include <unistd.h> /* getpid */
#endif
#include "sci_tmpdir.h"
#include "os_string.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "setenvc.h"
#include "getenvc.h"
#include "localization.h"
#include <errno.h>
#include "removedir.h"
#include "setenvvar.h"
#include "getshortpathname.h"
}

char* getTMPDIR(void)
{
    return os_strdup(ConfigVariable::getTMPDIR().c_str());
}
/*--------------------------------------------------------------------------*/
void setTMPDIR(const char* _sci_tmpdir)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_tmpdir);
    symbol::Context::getInstance()->put(symbol::Symbol("TMPDIR"), pS);

    //add SCI value ConfigVariable
    ConfigVariable::setTMPDIR(_sci_tmpdir);
}
/*--------------------------------------------------------------------------*/
void putenvTMPDIR(const char *_sci_tmpdir)
{
    char *ShortPath = NULL;
    char *CopyOfDefaultPath = NULL;

    /* to be sure that it's unix 8.3 format */
    /* c:/progra~1/scilab-5.0 */
    BOOL bConvertOK = FALSE;
    ShortPath = getshortpathname(_sci_tmpdir, &bConvertOK);
    //GetShortPathName(_sci_path,ShortPath,PATH_MAX);

    CopyOfDefaultPath = new char[strlen(_sci_tmpdir) + 1];
    AntislashToSlash(ShortPath, CopyOfDefaultPath);

    setenvc("TMPDIR", ShortPath);

    delete[] CopyOfDefaultPath;
    FREE(ShortPath);
}
/*--------------------------------------------------------------------------*/
char* getenvTMPDIR()
{
    int ierr, iflag = 0;
    int lbuf = PATH_MAX;
    char *SciPath = new char[PATH_MAX];

    if (SciPath)
    {
        getenvc(&ierr, "TMPDIR", SciPath, &lbuf, &iflag);

        if (ierr == 1)
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
    char tmpDirDefault[PATH_MAX];

    if (!GetTempPathA(PATH_MAX, tmpDirDefault))
    {
        MessageBoxA(NULL, _("Cannot find Windows temporary directory (1)."), _("Error"), MB_ICONERROR);
        exit(1);
    }
    else
    {
        char tmp_dir[PATH_MAX + FILENAME_MAX + 1];
        static char bufenv[PATH_MAX + 16];
        char *TmpDir = NULL;
        os_sprintf(tmp_dir, PATH_MAX + FILENAME_MAX + 1, "%sSCI_TMP_%d_", tmpDirDefault, _getpid());
        if (CreateDirectoryA(tmp_dir, NULL) == FALSE)
        {
            DWORD attribs = GetFileAttributesA(tmp_dir);
            if (attribs & FILE_ATTRIBUTE_DIRECTORY)
            {
                /* Repertoire existant */
            }
            else
            {
#ifdef _DEBUG
                {
                    char MsgErr[1024];
                    os_sprintf(MsgErr, 1024, _("Impossible to create : %s"), tmp_dir);
                    MessageBoxA(NULL, MsgErr, _("Error"), MB_ICONERROR);
                    exit(1);
                }
#else
                {
                    GetTempPathA(PATH_MAX, tmpDirDefault);
                    strcpy(tmp_dir, tmpDirDefault);
                    tmp_dir[strlen(tmp_dir) - 1] = '\0'; /* Remove last \ */
                }
#endif
            }
        }

        os_sprintf(bufenv, PATH_MAX + 16, "TMPDIR=%s", tmp_dir);
        _putenv(bufenv);

        return os_strdup(tmp_dir);
    }
#else
    char *tmpdir;

    char* env_dir = (char*)MALLOC(sizeof(char) * (PATH_MAX + 16));
    /* If the env variable TMPDIR is set, honor this preference */
    tmpdir = getenv("TMPDIR");
    if (tmpdir != NULL && strlen(tmpdir) < (PATH_MAX) && strstr(tmpdir, "SCI_TMP_") == NULL)
    {
        strcpy(env_dir, tmpdir);
    }
    else
    {
        strcpy(env_dir, "/tmp");
    }

    /* XXXXXX will be randomized by mkdtemp */
    char *env_dir_strdup = os_strdup(env_dir); /* Copy to avoid to have the same buffer as input and output for sprintf */
    sprintf(env_dir, "%s/SCI_TMP_%d_XXXXXX", env_dir_strdup, (int)getpid());
    free(env_dir_strdup);

    if (mkdtemp(env_dir) == NULL)
    {
        fprintf(stderr, _("Error: Could not create %s: %s\n"), env_dir, strerror(errno));
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
    char * tmpdir = getTMPDIR();
    Parser parser;
    parser.releaseTmpFile();
    removedir(tmpdir);
    FREE(tmpdir);
}

