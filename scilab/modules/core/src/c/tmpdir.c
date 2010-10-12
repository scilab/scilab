/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#include <process.h>
#endif

#if defined(__STDC__) || defined(_MSC_VER)
#include <stdlib.h>
#ifndef _MSC_VER
#include <sys/types.h>
#include <unistd.h>
#endif
#else 
extern  char  *getenv();
#endif

#include <errno.h>

#ifdef _MSC_VER
#include <process.h>
#include "strdup_windows.h"
#endif
#include "MALLOC.h" /* MALLOC */
#include "tmpdir.h"
#include "localization.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "PATH_MAX.h"
#include "removedir.h"
#include "createdirectory.h"
/*--------------------------------------------------------------------------*/
static char tmp_dir[PATH_MAX+FILENAME_MAX+1];
static int alreadyCreated = 0;
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
void createScilabTMPDIR(void)
{
    wchar_t wcTmpDirDefault[PATH_MAX];
    if (!GetTempPathW(PATH_MAX,wcTmpDirDefault))
    {
        MessageBox(NULL,_("Cannot find Windows temporary directory (1)."),_("Error"),MB_ICONERROR);
        exit(1);
    }
    else
    {
        wchar_t wctmp_dir[PATH_MAX+FILENAME_MAX+1];
        static wchar_t bufenv[PATH_MAX + 16];
        char *TmpDir = NULL;
        swprintf(wctmp_dir,PATH_MAX+FILENAME_MAX+1,L"%sSCI_TMP_%d_",wcTmpDirDefault,(int) _getpid());
        if ( CreateDirectoryW(wctmp_dir,NULL) == FALSE)
        {
            DWORD attribs = GetFileAttributesW (wctmp_dir);
            if (attribs & FILE_ATTRIBUTE_DIRECTORY)
            {
                /* Repertoire existant */
            }
            else
            {
                #ifdef _DEBUG
                {
                    char MsgErr[1024];
                    wsprintf(MsgErr,_("Impossible to create : %s"),tmp_dir);
                    MessageBox(NULL,MsgErr,_("Error"),MB_ICONERROR);
                    exit(1);
                }
                #else
                {
                    GetTempPathW(PATH_MAX,wcTmpDirDefault);
                    wcscpy(wctmp_dir,wcTmpDirDefault);
                    wctmp_dir[wcslen(wctmp_dir)-1]='\0'; /* Remove last \ */
                }
                #endif
            }
        }

        swprintf(bufenv,PATH_MAX + 16,L"TMPDIR=%s",wctmp_dir);
        _wputenv(bufenv);

        TmpDir = wide_string_to_UTF8(wctmp_dir);
        if (TmpDir)
        {
            strcpy(tmp_dir, TmpDir);
            FREE(TmpDir);
        }
        else
        {
            strcpy(tmp_dir, "");
        }
    }
}
#else

void createScilabTMPDIR(void)
{
    char *tmpdir;

    if ( alreadyCreated == 0 )
    {
        static char bufenv[PATH_MAX + 16];
        alreadyCreated++;
        /* If the env variable TMPDIR is set, honor this preference */
        if ((tmpdir = getenv("TMPDIR")) != NULL &&
            strlen(tmpdir) < (PATH_MAX) && strstr(tmpdir, "SCI_TMP_") == NULL)
        {
            /* TMPDIR does not contains SCI_TMP. Using TMPDIR */
            strcpy(tmp_dir, tmpdir);
        }
        else
        {
            /* Two cases where we can be here:
             *  - When TMPDIR is not set (all cases)
             *  - If SCI_TMP_* is in the TMPDIR variable, switch to the default
            */
            strcpy(tmp_dir, "/tmp");
        }

        /* XXXXXX will be randomized by mkdtemp */
        char *tmp_dir_strdup = strdup(tmp_dir); /* Copy to avoid to have the same buffer as input and output for sprintf */
        sprintf(tmp_dir, "%s/SCI_TMP_%d_XXXXXX", tmp_dir_strdup, (int) getpid());
        free(tmp_dir_strdup);

        if(mkdtemp(tmp_dir) == NULL)
        {
                fprintf(stderr,_("Error: Could not create %s: %s\n"), tmp_dir, strerror(errno));
        }

        sprintf(bufenv, "TMPDIR=%s", tmp_dir);
        putenv(bufenv);
    }
}
#endif
/*--------------------------------------------------------------------------*/
/**
* creates a tmp dir for a scilab session
* and fixes the TMPDIR env variable
*/
void C2F(settmpdir)(void)
{
    createScilabTMPDIR();
}
/*--------------------------------------------------------------------------*/
char *getTMPDIR(void)
{
    return strdup(tmp_dir);
}
/*--------------------------------------------------------------------------*/
/**
* remove TMPDIR directory
*/
void C2F(tmpdirc)(void)
{
    removedir(tmp_dir);
}
/*--------------------------------------------------------------------------*/
wchar_t *getTMPDIRW(void)
{
    return to_wide_string(tmp_dir);
}
/*--------------------------------------------------------------------------*/
