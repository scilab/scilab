/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA/ENPC
* Copyright (C) DIGITEO - 2011 - Allan CORNET
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

/*---------------------------------------------------------------------------*/

#include "configvariable.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_link.h"
#include "men_Sutils.h"
#include "sci_malloc.h" /* MALLOC */
#include "configvariable_interface.h"
#include "sciprint.h"
#include "addinter.h"
#include "localization.h"
#include "Scierror.h"
#include "FileExist.h"
#include "ilib_verbose.h"

#ifdef _MSC_VER
#include "getenvc.h"
#include "dllinfo.h"
#endif

#include "getshortpathname.h"
#include "BOOL.h"
#include "charEncoding.h"
}

static void Underscores(BOOL _bFortran, wchar_t* _pwstEntryPointName, wchar_t* _pwstTrailingName);

typedef void (*function) ();


int scilabLink(int _iLibID, wchar_t* _pwstLibraryName, wchar_t** _pwstEntryPointName, int _iEntryPointSize, BOOL _bFortran , int *_piErr)
{
    int iLibID = -1;

    if (_iLibID == -1)
    {
        iLibID = Sci_dlopen(_pwstLibraryName);
    }
    else
    {
        iLibID = _iLibID;
    }

    if (iLibID == -1)
    {
        if ( getWarningMode() )
        {
#ifdef _MSC_VER
            if (isDllW(_pwstLibraryName))
            {
#ifdef _WIN64
                if (isX86DllW(_pwstLibraryName))
                {
                    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprint(_("%s: can not to load a x86 dll in a x64 environment.\n" ), "link");
                    }
                    SetLastError(ERROR_DLL_INIT_FAILED);
                }
#else
                if (isX64DllW(_pwstLibraryName))
                {
                    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprint(_("%s: can not load a x64 dll in a x86 environment.\n" ), "link");
                    }
                    SetLastError(ERROR_DLL_INIT_FAILED);
                }
#endif
            }
            else
            {
                wchar_t* pwstPathSearch = searchEnvW(_pwstLibraryName, L"PATH");
                if (pwstPathSearch == NULL)
                {
                    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprint(_("%ls: The file %ls does not exist in PATH environment.\n" ), L"link", _pwstLibraryName);
                    }
                }
            }
#else
            if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
            {
                sciprint(_("Link failed for dynamic library '%ls'.\n"), _pwstLibraryName);
                sciprint(_("An error occurred: %s\n"), GetLastDynLibError());
            }
#endif
        }
        *_piErr = -1;
        return iLibID;
    }

    if ( (_iLibID == -1) && (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT))
    {
        sciprint(_("Shared archive loaded.\n"));
        sciprint(_("Link done.\n"));
    }

    for (int i = 0 ; i < _iEntryPointSize ; i++)
    {
        *_piErr = Sci_dlsym(_pwstEntryPointName[i], iLibID, _bFortran);
    }

    return iLibID;
}
/*---------------------------------------------------------------------------*/
/**
* Underscores : deals with the trailing _
* in entry names
*/
static void Underscores(BOOL _bFortran, wchar_t* _pwstEntryPointName, wchar_t* _pwstTrailingName)
{
#ifdef WLU1
    *_pwstTrailingName = L'_';
    _pwstTrailingName++;
#endif
    wcscpy(_pwstTrailingName, _pwstEntryPointName);
#ifdef WTU
    if (_bFortran)
    {
        wcscat(_pwstTrailingName, L"_");
    }
#endif
    return;
}
/*---------------------------------------------------------------------------*/
int Sci_dlclose(DynLibHandle _hLib)
{
    return FreeDynLibrary(_hLib);
}
/*---------------------------------------------------------------------------*/
int Sci_dlopen(wchar_t* _pwstDynLibPath)
{
    static DynLibHandle hLib = NULL;
    int i = 0;

#ifdef _MSC_VER
    {
        hLib = LoadDynLibraryW(_pwstDynLibPath);
    }
#else
    {
        char* pstDynLibPath = wide_string_to_UTF8(_pwstDynLibPath);
        hLib = LoadDynLibrary (pstDynLibPath);
        FREE(pstDynLibPath);
    }
#endif

    if (hLib == NULL)
    {
        return -1 ; /* the shared archive was not loaded. */
    }

    ConfigVariable::DynamicLibraryStr* pDL = ConfigVariable::getNewDynamicLibraryStr();
    ConfigVariable::setLibraryName(pDL, _pwstDynLibPath);
    pDL->hLib = hLib;


    return ConfigVariable::addDynamicLibrary(pDL);
}
/*---------------------------------------------------------------------------*/
int Sci_dlsym(wchar_t* _pwstEntryPointName, int _iLibID, BOOL _bFortran)
{
    DynLibHandle hDynLib = NULL;
    ConfigVariable::EntryPointStr* pEP = ConfigVariable::getNewEntryPointStr();
    //+3 : 1 for '\0', 1 for prefix _, 1 for suffix _
    wchar_t* pwstEntryPointName = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(_pwstEntryPointName) + 3));
    memset(pwstEntryPointName, 0x00, (wcslen(_pwstEntryPointName) + 3));

    Underscores(_bFortran, _pwstEntryPointName, pwstEntryPointName);


    if (_iLibID < 0 || ConfigVariable::isDynamicLibrary(_iLibID) == false)
    {
        //no valid library at this ID
        FREE(pwstEntryPointName);
        FREE(pEP);
        return -3;
    }

    /** entry was previously loaded **/
    if (ConfigVariable::getEntryPoint(_pwstEntryPointName, _iLibID) != NULL)
    {
        sciprint(_("Entry name %ls.\n"), _pwstEntryPointName);
        FREE(pwstEntryPointName);
        FREE(pEP);
        return -4;
    }

    pEP->iLibIndex = _iLibID;
    hDynLib = (DynLibHandle)  ConfigVariable::getDynamicLibrary(_iLibID)->hLib;
#ifdef _MCS_VER
    pEP->functionPtr = (function) GetDynLibFuncPtrW(hDynLib, pwstEntryPointName);
#else
    char* pstEntryPointName = wide_string_to_UTF8(pwstEntryPointName);
    pEP->functionPtr = (function) GetDynLibFuncPtr(hDynLib, pstEntryPointName);
    FREE(pstEntryPointName);
#endif
    if (pEP->functionPtr == NULL)
    {
        if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
        {
            sciprint(_("%ls is not an entry point.\n"), _pwstEntryPointName);
        }
        FREE(pwstEntryPointName);
        FREE(pEP);
        return -5;
    }


    if (0 /*debug mode*/)
    {
        sciprint(_("Linking %ls.\n"), _pwstEntryPointName);
    }

    ConfigVariable::setEntryPointName(pEP, _pwstEntryPointName);
    ConfigVariable::addEntryPoint(pEP);
    FREE(pwstEntryPointName);
    return 0;
}
/*---------------------------------------------------------------------------*/
