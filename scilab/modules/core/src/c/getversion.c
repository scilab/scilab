/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include <string.h>
#include "getversion.h"
#include "version.h"
#include "configvariable_interface.h"
#include "scilabDefaults.h"
#include "getenvc.h"
#include "os_string.h"
#include "with_module.h"
#include "loadversion.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "../../../io/includes/getenvc.h"
#include "charEncoding.h"

/*--------------------------------------------------------------------------*/
/*wide char*/
#define TCLSCI_MODULE_NAME L"tclsci"
#define TCLTK_OPTION_STRING L"tk"
#define MODELICAC_OPTION_STRING L"modelicac"
#define X86_STRING L"x86"
#define X64_STRING L"x64"
#define ICC_STRING L"ICC"
#define VC_STRING L"VC++"
#define GCC_STRING L"GCC"
#define PGI_STRING L"PGI"
#define SUN_STRING L"SUN"
#define UNKNOW_STRING L"UKN"
#define RELEASE_STRING L"release"
#define DEBUG_STRING L"debug"
#define DEFAULT_VERSION_ARRAY_SIZE 4
#define SCILAB_STRING L"scilab"
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
char* getScilabVersionAsString(void)
{
    return os_strdup(SCI_VERSION);
}
/*--------------------------------------------------------------------------*/
wchar_t *getScilabVersionAsWideString(void)
{
    return os_wcsdup(SCI_VERSION_WIDE_STRING);
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
int* getModuleVersion(wchar_t* _pwstModule, int *sizeArrayReturned)
{
    int *returnedArray = NULL;

    if (_pwstModule)
    {
        if (wcscmp(_pwstModule, SCILAB_STRING) == 0)
        {
            returnedArray = getScilabVersion(sizeArrayReturned);
            return returnedArray;
        }
    }

    if (with_module(_pwstModule))
    {
#define LineMax 1024
        wchar_t versionstring[LineMax];
        int version_module_major = 0;
        int version_module_minor = 0;
        int version_module_maintenance = 0;
        int version_module_revision = 0;

        if (getversionmodule(_pwstModule, &version_module_major, &version_module_minor,
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
wchar_t* getModuleVersionInfoAsString(wchar_t* _pwstModule)
{
    wchar_t* infoString = NULL;

    if (_pwstModule)
    {
        if (wcscmp(_pwstModule, SCILAB_STRING) == 0)
        {
            infoString = getScilabVersionAsWideString();
            return infoString;
        }
    }

    if (with_module(_pwstModule))
    {
#define LineMax 1024
        wchar_t versionstring[LineMax];
        int version_module_major = 0;
        int version_module_minor = 0;
        int version_module_maintenance = 0;
        int version_module_revision = 0;
        if (getversionmodule(_pwstModule, &version_module_major, &version_module_minor,
                             &version_module_maintenance, versionstring,
                             &version_module_revision))
        {
            infoString = os_wcsdup(versionstring);
        }
    }
    return infoString;
}
/*--------------------------------------------------------------------------*/
wchar_t** getScilabVersionOptions(int *sizeArrayReturned)
{
    wchar_t** options = NULL;
    int nbOptions = 2;
    *sizeArrayReturned = 0;

    options = (wchar_t**)MALLOC(sizeof(wchar_t*) * nbOptions);
    if (options)
    {
        options[0] = getCompilerUsedToBuildScilab();
        options[1] = getCompilerArchitecture();

        if (with_tk())
        {
            options = (wchar_t**)REALLOC(options, sizeof(wchar_t*) * (nbOptions + 1));
            if (options)
            {
                options[nbOptions] = os_wcsdup(TCLTK_OPTION_STRING);
                nbOptions++;
            }
            else
            {
                freeArrayOfWideString(options, nbOptions);
                return NULL;
            }
        }

        if (with_modelica_compiler())
        {
            options = (wchar_t**)REALLOC(options, sizeof(wchar_t*) * (nbOptions + 1));
            if (options)
            {
                options[nbOptions] = os_wcsdup(MODELICAC_OPTION_STRING);
                nbOptions++;
            }
            else
            {
                freeArrayOfWideString(options, nbOptions);
                return NULL;
            }
        }

        options = (wchar_t**)REALLOC(options, sizeof(wchar_t*) * (nbOptions + 1));
        if (options)
        {
            options[nbOptions] = getReleaseMode();
            nbOptions++;
        }
        else
        {
            freeArrayOfWideString(options, nbOptions);
            return NULL;
        }

        options = (wchar_t**)REALLOC(options, sizeof(wchar_t*) * (nbOptions + 1));
        if (options)
        {
            options[nbOptions] = getReleaseDate();
            nbOptions++;
        }
        else
        {
            freeArrayOfWideString(options, nbOptions);
            return NULL;
        }

        options = (wchar_t**)REALLOC(options, sizeof(wchar_t*) * (nbOptions + 1));
        if (options)
        {
            options[nbOptions] = getReleaseTime();
            nbOptions++;
        }
        else
        {
            freeArrayOfWideString(options, nbOptions);
            return NULL;
        }

        *sizeArrayReturned = nbOptions;
    }
    return options;
}
/*--------------------------------------------------------------------------*/
wchar_t* getReleaseMode(void)
{
#ifdef NDEBUG
    return os_wcsdup(RELEASE_STRING);
#else
    return os_wcsdup(DEBUG_STRING);
#endif
}
/*--------------------------------------------------------------------------*/
wchar_t* getReleaseDate(void)
{
    return to_wide_string(__DATE__);
}
/*--------------------------------------------------------------------------*/
wchar_t *getReleaseTime(void)
{
    return to_wide_string(__TIME__);
}
/*--------------------------------------------------------------------------*/
wchar_t* getCompilerUsedToBuildScilab(void)
{
#ifdef __INTEL_COMPILER
    return os_wcsdup(ICC_STRING);
#else
#ifdef _MSC_VER
    return os_wcsdup(VC_STRING);
#else
#ifdef __GNUC__
    return os_wcsdup(GCC_STRING);
#else
#ifdef __PGI
    return os_wcsdup(PGI_STRING);
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    return os_wcsdup(SUN_STRING);
#else
    return os_wcsdup(UNKNOW_STRING);
#endif
#endif
#endif
#endif
#endif
}
/*--------------------------------------------------------------------------*/
wchar_t* getCompilerArchitecture(void)
{
#ifdef _MSC_VER
#ifdef _WIN64
    return os_wcsdup(X64_STRING);
#else
    return os_wcsdup(X86_STRING);
#endif
#else
#ifdef _LP64
    return os_wcsdup(X64_STRING);
#else
    return os_wcsdup(X86_STRING);
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
