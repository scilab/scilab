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
    return wide_string_to_UTF8(ConfigVariable::getTMPDIR().c_str());
}
/*--------------------------------------------------------------------------*/
wchar_t* getTMPDIRW(void)
{
    return os_wcsdup(ConfigVariable::getTMPDIR().c_str());
}

/*--------------------------------------------------------------------------*/
void setTMPDIRW(const wchar_t* _sci_tmpdir)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_tmpdir);
    symbol::Context::getInstance()->put(symbol::Symbol(L"TMPDIR"), pS);

    //add SCI value ConfigVariable
    std::wstring sci_tmpdir(_sci_tmpdir);
    ConfigVariable::setTMPDIR(sci_tmpdir);
}
/*--------------------------------------------------------------------------*/
void setTMPDIR(const char* _sci_tmpdir)
{
    wchar_t* pstTemp = to_wide_string(_sci_tmpdir);
    setTMPDIRW(pstTemp);
    FREE(pstTemp);
}
/*--------------------------------------------------------------------------*/
void putenvTMPDIRW(const wchar_t* _sci_tmpdir)
{
    char* pstTemp = wide_string_to_UTF8(_sci_tmpdir);
    putenvTMPDIR(pstTemp);
    FREE(pstTemp);
    return;
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
wchar_t* getenvTMPDIRW()
{
    char *SciTemp = getenvTMPDIR();
    wchar_t* pstTemp = to_wide_string(SciTemp);
    delete[] SciTemp;
    return pstTemp;
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
            delete[] SciPath;
            return NULL;
        }
    }

    return SciPath;
}
/*--------------------------------------------------------------------------*/
wchar_t* computeTMPDIRW(void)
{
    char* pstTemp = computeTMPDIR();
    wchar_t* pstReturn = to_wide_string(pstTemp);
    FREE(pstTemp);
    return pstReturn;
}
/*--------------------------------------------------------------------------*/
//windows : find main DLL and extract path
//linux and macos : scilab script fill SCI env variable
char* computeTMPDIR()
{
#ifdef _MSC_VER
    wchar_t wcTmpDirDefault[PATH_MAX];

    if (!GetTempPathW(PATH_MAX, wcTmpDirDefault))
    {
        MessageBoxA(NULL, _("Cannot find Windows temporary directory (1)."), _("Error"), MB_ICONERROR);
        exit(1);
    }
    else
    {
        wchar_t wctmp_dir[PATH_MAX + FILENAME_MAX + 1];
        static wchar_t bufenv[PATH_MAX + 16];
        char *TmpDir = NULL;
        os_swprintf(wctmp_dir, PATH_MAX + FILENAME_MAX + 1, L"%lsSCI_TMP_%d_", wcTmpDirDefault, _getpid());
        if (CreateDirectoryW(wctmp_dir, NULL) == FALSE)
        {
            DWORD attribs = GetFileAttributesW(wctmp_dir);
            if (attribs & FILE_ATTRIBUTE_DIRECTORY)
            {
                /* Repertoire existant */
            }
            else
            {
#ifdef _DEBUG
                {
                    char MsgErr[1024];
                    wsprintfA(MsgErr, _("Impossible to create : %s"), wctmp_dir);
                    MessageBoxA(NULL, MsgErr, _("Error"), MB_ICONERROR);
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

        os_swprintf(bufenv, PATH_MAX + 16, L"TMPDIR=%ls", wctmp_dir);
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
    wchar_t* sci_tmpdir = computeTMPDIRW();
    setTMPDIRW(sci_tmpdir);
    putenvTMPDIRW(sci_tmpdir);
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

