/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "getversion.h"
#include "version.h"
#include "scilabmode.h"
#include "scilabDefaults.h"
#include "getenvc.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "with_module.h"
#include "loadversion.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "../../../io/includes/getenvc.h"
/*--------------------------------------------------------------------------*/
#define TCLSCI_MODULE_NAME "tclsci"
#define TCLTK_OPTION_STRING "tk"
#define MODELICAC_OPTION_STRING "modelicac"
#define X86_STRING "x86"
#define X64_STRING "x64"
#define ICC_STRING "ICC"
#define VC_STRING "VC++"
#define GCC_STRING "GCC"
#define PGI_STRING "PGI"
#define SUN_STRING "SUN"
#define UNKNOW_STRING "UKN"
#define RELEASE_STRING "release"
#define DEBUG_STRING "debug"
#define DEFAULT_VERSION_ARRAY_SIZE 4
#define SCILAB_STRING "scilab"
/*--------------------------------------------------------------------------*/
int *getScilabVersion(int *sizeArrayReturned)
{
    int *returnedArray = (int *)MALLOC(sizeof(int) * DEFAULT_VERSION_ARRAY_SIZE);
    *sizeArrayReturned = 0;
    if (returnedArray)
    {
        returnedArray[0] = (int)SCI_VERSION_MAJOR;
        returnedArray[1] = (int)SCI_VERSION_MINOR;
        returnedArray[2] = (int)SCI_VERSION_MAINTENANCE;
        returnedArray[3] = (int)SCI_VERSION_TIMESTAMP;

        *sizeArrayReturned = DEFAULT_VERSION_ARRAY_SIZE;
    }
    return returnedArray;
}
/*--------------------------------------------------------------------------*/
int getScilabVersionMajor()
{
    return (int)SCI_VERSION_MAJOR;
}
/*--------------------------------------------------------------------------*/
int getScilabVersionMinor()
{
    return (int)SCI_VERSION_MINOR;
}
/*--------------------------------------------------------------------------*/
int getScilabVersionMaintenance()
{
    return (int)SCI_VERSION_MAINTENANCE;
}
/*--------------------------------------------------------------------------*/
int getScilabVersionTimestamp()
{
    return (int)SCI_VERSION_TIMESTAMP;
}
/*--------------------------------------------------------------------------*/
char *getScilabVersionAsString(void)
{
    return strdup(SCI_VERSION_STRING);
}
/*--------------------------------------------------------------------------*/
int* getModuleVersion(char *modulename, int *sizeArrayReturned)
{
    int *returnedArray = NULL;

    if (modulename)
    {
        if (strcmp(modulename, SCILAB_STRING) == 0)
        {
            returnedArray = getScilabVersion(sizeArrayReturned);
            return returnedArray;
        }
    }

    if (with_module(modulename))
    {
#define LineMax 1024
        char versionstring[LineMax];
        int version_module_major = 0;
        int version_module_minor = 0;
        int version_module_maintenance = 0;
        int version_module_revision = 0;

        if (getversionmodule(modulename, &version_module_major, &version_module_minor,
                             &version_module_maintenance, versionstring,
                             &version_module_revision))
        {
            returnedArray = (int *)MALLOC(sizeof(int) * DEFAULT_VERSION_ARRAY_SIZE);
            if (returnedArray)
            {
                returnedArray[0] = version_module_major;
                returnedArray[1] = version_module_minor;
                returnedArray[2] = version_module_maintenance;
                returnedArray[3] = version_module_revision;
                *sizeArrayReturned = DEFAULT_VERSION_ARRAY_SIZE;
            }
            else
            {
                *sizeArrayReturned = 0;
            }
        }
    }
    else
    {
        *sizeArrayReturned = 0;
    }
    return returnedArray;
}
/*--------------------------------------------------------------------------*/
char *getModuleVersionInfoAsString(char *modulename)
{
    char *infoString = NULL;

    if (modulename)
    {
        if (strcmp(modulename, SCILAB_STRING) == 0)
        {
            infoString = getScilabVersionAsString();
            return infoString;
        }
    }

    if (with_module(modulename))
    {
#define LineMax 1024
        char versionstring[LineMax];
        int version_module_major = 0;
        int version_module_minor = 0;
        int version_module_maintenance = 0;
        int version_module_revision = 0;
        if (getversionmodule(modulename, &version_module_major, &version_module_minor,
                             &version_module_maintenance, versionstring,
                             &version_module_revision))
        {
            infoString = strdup(versionstring);
        }
    }
    return infoString;
}
/*--------------------------------------------------------------------------*/
char **getScilabVersionOptions(int *sizeArrayReturned)
{
    char **options = NULL;
    int nbOptions = 2;
    *sizeArrayReturned = 0;

    options = (char**)MALLOC(sizeof(char*) * nbOptions);
    if (options)
    {
        options[0] = getCompilerUsedToBuildScilab();
        options[1] = getCompilerArchitecture();

        if (with_tk())
        {
            options = REALLOC(options, sizeof(char*) * (nbOptions + 1));
            if (options)
            {
                options[nbOptions] = strdup(TCLTK_OPTION_STRING);
                nbOptions++;
            }
            else
            {
                freeArrayOfString(options, nbOptions);
                return NULL;
            }
        }

        if (with_modelica_compiler())
        {
            options = REALLOC(options, sizeof(char*) * (nbOptions + 1));
            if (options)
            {
                options[nbOptions] = strdup(MODELICAC_OPTION_STRING);
                nbOptions++;
            }
            else
            {
                freeArrayOfString(options, nbOptions);
                return NULL;
            }
        }

        options = REALLOC(options, sizeof(char*) * (nbOptions + 1));
        if (options)
        {
            options[nbOptions] = getReleaseMode();
            nbOptions++;
        }
        else
        {
            freeArrayOfString(options, nbOptions);
            return NULL;
        }

        options = REALLOC(options, sizeof(char*) * (nbOptions + 1));
        if (options)
        {
            options[nbOptions] = getReleaseDate();
            nbOptions++;
        }
        else
        {
            freeArrayOfString(options, nbOptions);
            return NULL;
        }

        options = REALLOC(options, sizeof(char*) * (nbOptions + 1));
        if (options)
        {
            options[nbOptions] = getReleaseTime();
            nbOptions++;
        }
        else
        {
            freeArrayOfString(options, nbOptions);
            return NULL;
        }

        *sizeArrayReturned = nbOptions;
    }
    return options;
}
/*--------------------------------------------------------------------------*/
char *getReleaseMode(void)
{
#ifdef NDEBUG
    return strdup(RELEASE_STRING);
#else
    return strdup(DEBUG_STRING);
#endif
}
/*--------------------------------------------------------------------------*/
char *getReleaseDate(void)
{
    return strdup(__DATE__);
}
/*--------------------------------------------------------------------------*/
char *getReleaseTime(void)
{
    return strdup(__TIME__);
}
/*--------------------------------------------------------------------------*/
char *getCompilerUsedToBuildScilab(void)
{
#ifdef __INTEL_COMPILER
    return strdup(ICC_STRING);
#else
#ifdef _MSC_VER
    return strdup(VC_STRING);
#else
#ifdef __GNUC__
    return strdup(GCC_STRING);
#else
#ifdef __PGI
    return strdup(PGI_STRING);
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    return strdup(SUN_STRING);
#else
    return strdup(UNKNOW_STRING);
#endif
#endif
#endif
#endif
#endif
}
/*--------------------------------------------------------------------------*/
char *getCompilerArchitecture(void)
{
#ifdef _MSC_VER
#ifdef _WIN64
    return strdup(X64_STRING);
#else
    return strdup(X86_STRING);
#endif
#else
#ifdef _LP64
    return strdup(X64_STRING);
#else
    return strdup(X86_STRING);
#endif
#endif
}
/*--------------------------------------------------------------------------*/
BOOL with_modelica_compiler(void)
{
    char *fullpathModelicac = searchEnv(ModelicacName, "PATH");
    if (fullpathModelicac)
    {
        FREE(fullpathModelicac);
        fullpathModelicac = NULL;
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL with_tk(void)
{
    return with_module(TCLSCI_MODULE_NAME) && (getScilabMode() != SCILAB_NWNI);
}
/*--------------------------------------------------------------------------*/
