/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
extern "C"
{
#include "SetScilabEnvironmentVariables.h"
#include "PATH_MAX.h"
#include "sci_path.h"
#include "sci_malloc.h"
#include "getScilabDirectory.h"
#include "scilabDefaults.h"
#include "ConvertSlash.h"
#include "charEncoding.h"
#include "getshortpathname.h"
#include "stristr.h"
#include "strsubst.h"
#include "os_string.h"
}
/*--------------------------------------------------------------------------*/
static BOOL IsTheGoodShell(void);
static BOOL Set_Shell(void);
static BOOL Set_SCI_PATH(char *DefaultPath);
static BOOL Set_HOME_PATH(char *DefaultPath);
static BOOL AddScilabBinDirectoryToPATHEnvironnementVariable(char *DefaultPath);
static BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void);
/*--------------------------------------------------------------------------*/
/**
* Set some environment variablesSCI, and some others
*/
void SciEnvForWindows(void)
{
    char *SCIPathName = getScilabDirectory(TRUE);

    /* Correction Bug 1579 */
    if (!IsTheGoodShell())
    {
        if ( (!Set_Shell()) || (!IsTheGoodShell()))
        {
            MessageBox(NULL,
                       "Please modify ""ComSpec"" environment variable.\ncmd.exe on W2K and more.",
                       "Warning", MB_ICONWARNING | MB_OK);
        }
    }

    SetScilabEnvironmentVariables(SCIPathName);
    if (SCIPathName)
    {
        FREE(SCIPathName);
        SCIPathName = NULL;
    }
}
/*--------------------------------------------------------------------------*/
/* set env variables (used when calling scilab from * other programs) */
void SetScilabEnvironmentVariables(char *DefaultSCIPATH)
{
    if (DefaultSCIPATH)
    {
        Set_SCI_PATH(DefaultSCIPATH);
        Set_HOME_PATH(DefaultSCIPATH);
        Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB();
        AddScilabBinDirectoryToPATHEnvironnementVariable(DefaultSCIPATH);
    }
    else
    {
        /* Error */
        exit(1);
    }

}
/*--------------------------------------------------------------------------*/
BOOL Set_SCI_PATH(char *DefaultPath)
{
    BOOL bOK = FALSE;
    char *ShortPath = NULL;

    /* to be sure that it's unix 8.3 format */
    /* c:/progra~1/scilab-5.0 */
    ShortPath = getshortpathname(DefaultPath, &bOK);
    if (ShortPath)
    {
        char env[PATH_MAX + 1 + 10];
        AntislashToSlash(ShortPath, ShortPath);

        sprintf (env, "SCI=%s", ShortPath);
        setSCI(ShortPath);

        if (ShortPath)
        {
            FREE(ShortPath);
            ShortPath = NULL;
        }

        if (_putenv (env))
        {
            bOK = FALSE;
        }
        else
        {
            bOK = TRUE;
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL Set_HOME_PATH(char *DefaultPath)
{
    wchar_t *wHOME = _wgetenv(L"HOME");
    if (wHOME == NULL)
    {
        wchar_t *wUserProfile = _wgetenv(L"USERPROFILE");
        if (wUserProfile)
        {
            return SetEnvironmentVariableW(L"HOME", wUserProfile);
        }
        else
        {
            /* if USERPROFILE is not defined , we use default profile */
            wchar_t *wAllUsersProfile = _wgetenv(L"ALLUSERSPROFILE");
            if (wAllUsersProfile)
            {
                return SetEnvironmentVariableW(L"HOME", wUserProfile);
            }
            else
            {
                BOOL bRes = FALSE;
                wchar_t *wDefault = to_wide_string(DefaultPath);
                if (wDefault)
                {
                    bRes = SetEnvironmentVariableW(L"HOME", wDefault);
                    FREE(wDefault);
                    wDefault = NULL;
                }
                return bRes;
            }
        }
    }
    return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void)
{
    BOOL bOK = TRUE;

#ifdef _MSC_VER
    _putenv ("COMPILER=VC++");
#endif

    /* WIN32 variable Environment */
#ifdef _WIN32
    _putenv ("WIN32=OK");
#endif

    /* WIN64 variable Environment */
#ifdef _WIN64
    _putenv ("WIN64=OK");
#endif

    if ( GetSystemMetrics(SM_REMOTESESSION) )
    {
        _putenv ("SCILAB_MSTS_SESSION=OK");
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL IsTheGoodShell(void)
{
    char shellCmd[PATH_MAX];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    strcpy(shellCmd, "");
    strcpy(fname, "");
    GetEnvironmentVariable("ComSpec", shellCmd, PATH_MAX);
    os_splitpath(shellCmd, drive, dir, fname, ext);

    if (_stricmp(fname, "cmd") == 0)
    {
        return TRUE;
    }

    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL Set_Shell(void)
{
    BOOL bOK = FALSE;
    char env[_MAX_DRIVE + _MAX_DIR + _MAX_FNAME + _MAX_EXT + 10];
    char *WINDIRPATH = NULL;

    WINDIRPATH = getenv ("SystemRoot");
    sprintf(env, "ComSpec=%s\\system32\\cmd.exe", WINDIRPATH);

    if (_putenv (env))
    {
        bOK = FALSE;
    }
    else
    {
        bOK = TRUE;
    }

    if (WINDIRPATH)
    {
        FREE(WINDIRPATH);
        WINDIRPATH = NULL;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
static BOOL AddScilabBinDirectoryToPATHEnvironnementVariable(char *DefaultPath)
{
#define SCILAB_BIN_PATH "%s/bin"
#define NEW_PATH "PATH=%s;%s"

    BOOL bOK = FALSE;
    char *PATH = NULL;
    char *env = NULL;
    char scilabBinPath[MAX_PATH];
    char *scilabBinPathConverted;

    PATH = getenv("PATH");

    env = (char*) MALLOC(sizeof(char) * (strlen(NEW_PATH) + strlen(PATH) +
                                         strlen(DefaultPath) + 1));
    if (env)
    {
        sprintf(scilabBinPath, SCILAB_BIN_PATH, DefaultPath);

        scilabBinPathConverted = (char*) MALLOC(MAX_PATH * sizeof(char));
#ifdef _MSC_VER
        scilabBinPathConverted = strsub(scilabBinPath, "/", "\\");
#else
        scilabBinPathConverted = strdup(scilabBinPath);
#endif
        if (stristr(PATH, scilabBinPathConverted) == 0)
        {
            sprintf(env, NEW_PATH, scilabBinPathConverted, PATH);
            if (_putenv (env))
            {
                bOK = FALSE;
            }
            else
            {
                bOK = TRUE;
            }
            FREE(env);
            env = NULL;
        }

        FREE(scilabBinPathConverted);
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
