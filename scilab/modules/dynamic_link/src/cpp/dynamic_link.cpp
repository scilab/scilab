/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA/ENPC
* Copyright (C) DIGITEO - 2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

static void Underscores(BOOL _bFortran, const char* _pstEntryPointName, char* _pstTrailingName);

typedef void (*function) ();


int scilabLink(int _iLibID, const char* _pstLibraryName, const char** _pstEntryPointName, int _iEntryPointSize, BOOL _bFortran, int *_piErr)
{
    int iLibID = -1;

    if (_iLibID == -1)
    {
        iLibID = Sci_dlopen(_pstLibraryName);
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
            if (isDll(_pstLibraryName))
            {
#ifdef _WIN64
                if (isX86Dll(_pstLibraryName))
                {
                    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprint(_("%s: can not to load a x86 dll in a x64 environment.\n" ), "link");
                    }
                    SetLastError(ERROR_DLL_INIT_FAILED);
                }
#else
                if (isX64Dll(_pstLibraryName))
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
                char* pstPathSearch = searchEnv(_pstLibraryName, "PATH");
                if (pstPathSearch == NULL)
                {
                    if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprint(_("%s: The file %s does not exist in PATH environment.\n" ), "link", _pstLibraryName);
                    }
                }
            }
#else
            if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
            {
                sciprint(_("Link failed for dynamic library '%s'.\n"), _pstLibraryName);
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
        *_piErr = Sci_dlsym(_pstEntryPointName[i], iLibID, _bFortran);
    }

    return iLibID;
}
/*---------------------------------------------------------------------------*/
/**
* Underscores : deals with the trailing _
* in entry names
*/
static void Underscores(BOOL _bFortran, const char* _pstEntryPointName, char* _pstTrailingName)
{
#ifdef WLU1
    *_pstTrailingName = '_';
    _pstTrailingName++;
#endif
    strcpy(_pstTrailingName, _pstEntryPointName);
#ifdef WTU
    if (_bFortran)
    {
        strcat(_pstTrailingName, "_");
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
int Sci_dlopen(const char* _pstDynLibPath)
{
    static DynLibHandle hLib = NULL;
    int i = 0;

#ifdef _MSC_VER
    {
        hLib = LoadDynLibrary(_pstDynLibPath);
    }
#else
    {
        char* pstDynLibPath = wide_string_to_UTF8(_pstDynLibPath);
        hLib = LoadDynLibrary (pstDynLibPath);
        FREE(pstDynLibPath);
    }
#endif

    if (hLib == NULL)
    {
        return -1 ; /* the shared archive was not loaded. */
    }

    ConfigVariable::DynamicLibraryStr* pDL = ConfigVariable::getNewDynamicLibraryStr();
    ConfigVariable::setLibraryName(pDL, _pstDynLibPath);
    pDL->hLib = hLib;


    return ConfigVariable::addDynamicLibrary(pDL);
}
/*---------------------------------------------------------------------------*/
int Sci_dlsym(const char* _pstEntryPointName, int _iLibID, BOOL _bFortran)
{
    DynLibHandle hDynLib = NULL;
    ConfigVariable::EntryPointStr* pEP = ConfigVariable::getNewEntryPointStr();
    //+3 : 1 for '\0', 1 for prefix _, 1 for suffix _
    char* pstEntryPointName = (char*)MALLOC(sizeof(char) * (strlen(_pstEntryPointName) + 3));
    memset(pstEntryPointName, 0x00, (strlen(_pstEntryPointName) + 3));

    Underscores(_bFortran, _pstEntryPointName, pstEntryPointName);


    if (_iLibID < 0 || ConfigVariable::isDynamicLibrary(_iLibID) == false)
    {
        //no valid library at this ID
        return -3;
    }

    /** entry was previously loaded **/
    if (ConfigVariable::getEntryPoint(_pstEntryPointName, _iLibID) != NULL)
    {
        sciprint(_("Entry name %s.\n"), _pstEntryPointName);
        return -4;
    }

    pEP->iLibIndex = _iLibID;
    hDynLib = (DynLibHandle)  ConfigVariable::getDynamicLibrary(_iLibID)->hLib;
    pEP->functionPtr = (function) GetDynLibFuncPtr(hDynLib, pstEntryPointName);
    FREE(pstEntryPointName);
    if (pEP->functionPtr == NULL)
    {
        if (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
        {
            sciprint(_("%s is not an entry point.\n"), _pstEntryPointName);
        }
        return -5;
    }


    if (0 /*debug mode*/)
    {
        sciprint(_("Linking %s.\n"), _pstEntryPointName);
    }

    ConfigVariable::setEntryPointName(pEP, _pstEntryPointName);
    ConfigVariable::addEntryPoint(pEP);
    return 0;
}
/*---------------------------------------------------------------------------*/
