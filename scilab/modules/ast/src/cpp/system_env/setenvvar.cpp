/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifdef _MSC_VER
#ifndef MAX_PATH_SHORT
#define MAX_PATH_SHORT 260
#endif
//Only for windows
#pragma warning(disable : 4996) //It's not beautifull but that works !
#endif

#include <iostream>

#ifdef _MSC_VER
#define NOMINMAX
#include "windows.h"
#define putenv _putenv

#else

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define  DIRMODE	0777

#endif

#include "context.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "setenvvar.h"
#include "sci_home.h"
#include "home.h"
#include "sci_path.h"
#include "sci_tmpdir.h"
#include "sci_malloc.h"
#include "getshortpathname.h"
}

static void SetScilabVariables(void);


/*--------------------------------------------------------------------------*/
/**
* Les variables d'environnements SCI, and some others
* sont définies directement dans scilab
* scilex peut donc etre executé seul
*/

/*--------------------------------------------------------------------------*/
/**
* Define SCI and some others Environments variables
*/
void SetScilabEnvironment(void)
{
#ifdef _MSC_VER
    //windows check
    SciEnvForWindows();
#endif

    SetScilabVariables();
}

static void SetScilabVariables(void)
{
    //create SCI
    defineSCI();
    //create SCIHOME
    defineSCIHOME();
    //create TMPDIR
    defineTMPDIR();
    //create home
    defineHOME();
}

#ifdef _MSC_VER
void SciEnvForWindows(void)
{
    char *SCIPathName = computeSCI();
    /* Correction Bug 1579 */
    if (!IsTheGoodShell())
    {
        if ( (!Set_Shell()) || (!IsTheGoodShell()))
        {
            std::cout << "Please modify ""ComSpec"" environment variable." << std::endl << "cmd.exe on W2K and more." << std::endl;
        }
    }

    SetScilabEnvironmentVariables(SCIPathName);
    if (SCIPathName)
    {
        FREE(SCIPathName);
        SCIPathName = NULL;
    }
}
#endif

#ifdef _MSC_VER
/*--------------------------------------------------------------------------*/
BOOL IsTheGoodShell(void)
{
    bool bOK = false;
    char shellCmd[PATH_MAX];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    strcpy(shellCmd, "");
    strcpy(fname, "");
    GetEnvironmentVariableA("ComSpec", shellCmd, PATH_MAX);
    os_splitpath(shellCmd, drive, dir, fname, ext);

    if (_stricmp(fname, "cmd") == 0)
    {
        bOK = true;
    }

    return bOK;
}

/*--------------------------------------------------------------------------*/
BOOL Set_Shell(void)
{
    bool bOK = false;
    char env[_MAX_DRIVE + _MAX_DIR + _MAX_FNAME + _MAX_EXT + 10];
    char *WINDIRPATH = NULL;

    WINDIRPATH = getenv ("SystemRoot");
    sprintf(env, "ComSpec=%s\\system32\\cmd.exe", WINDIRPATH);

    if (putenv (env))
    {
        bOK = false;
    }
    else
    {
        bOK = true;
    }

    return bOK;
}

static BOOL AddScilabBinDirectoryToPATHEnvironnementVariable(char *DefaultPath)
{
#define PATH_FORMAT "PATH=%s/bin;%s"

    BOOL bOK = FALSE;
    char *PATH = NULL;
    char *env = NULL;

    PATH = getenv("PATH");

    env = (char*) MALLOC(sizeof(char) * (strlen(PATH_FORMAT) + strlen(PATH) +
                                         strlen(DefaultPath) + 1));
    if (env)
    {
        sprintf(env, PATH_FORMAT, DefaultPath, PATH);
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
    return bOK;
}

void Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB(void)
{
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
}

void SetScilabEnvironmentVariables(char *DefaultSCIPATH)
{
    if (DefaultSCIPATH)
    {
        Set_SOME_ENVIRONMENTS_VARIABLES_FOR_SCILAB();
        AddScilabBinDirectoryToPATHEnvironnementVariable(DefaultSCIPATH);
    }
    else
    {
        /* Error */
        exit(1);
    }

}
#endif

/*--------------------------------------------------------------------------*/
BOOL AntislashToSlash(const char *pathwindows, char *pathunix)
{
    return convertSlash(pathwindows, pathunix, FALSE);
}
/*--------------------------------------------------------------------------*/
BOOL SlashToAntislash(const char *pathunix, char *pathwindows)
{
    return convertSlash(pathunix, pathwindows, TRUE);
}
/*--------------------------------------------------------------------------*/
BOOL convertSlash(const char *path_in, char *path_out, BOOL slashToAntislash)
{
    BOOL ret = FALSE;
    if ( (path_in) && (path_out) )
    {
        int i = 0;
        strcpy(path_out, path_in);
        for (i = 0; i < (int)strlen(path_out); i++)
        {
            if ( slashToAntislash )
            {
                if (path_in[i] == UNIX_SEPATATOR)
                {
                    path_out[i] = WINDOWS_SEPATATOR;
                    ret = TRUE;
                }
            }
            else
            {
                if (path_in[i] == WINDOWS_SEPATATOR)
                {
                    path_out[i] = UNIX_SEPATATOR;
                    ret =  TRUE;
                }
            }
        }
    }

    return ret;
}
