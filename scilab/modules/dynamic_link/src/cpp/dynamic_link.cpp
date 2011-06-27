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
#include "dynamiclibrary.h"
#include "men_Sutils.h"
#include "MALLOC.h" /* MALLOC */
#include "sci_warning.h"
#include "sciprint.h"
#include "stack-c.h"
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
/*---------------------------------------------------------------------------*/
static void Underscores(BOOL _bFortran, wchar_t* _pwstEntryPointName, wchar_t* _pwstTrailingName);
static int SearchFandS(char *op, int ilib);
/*---------------------------------------------------------------------------*/
#define MAXNAME  256 
#define TMPL 256
#define debug C2F(iop).ddt==1
/*---------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* struct used by fortran (F2C) */
/* required to be defined in C */

typedef struct {
    char name[nlgh+1];
} CINTER_struct;

__declspec (dllexport) CINTER_struct C2F(cinter);

/* struct used by fortran (F2C) */
/* required to be defined in C */
typedef struct {
    int ibuf[lsiz];
} IBFU_struct;
__declspec (dllexport) CINTER_struct C2F(ibfu);

#endif
/*---------------------------------------------------------------------------*/
typedef char Name[MAXNAME];   /* could be changed to dynamic structure */

typedef void (*function) (wchar_t*);

typedef struct 
{ 
    function epoint;            /* the entry point */ 
    Name     name;              /* entry point name */
    int      Nshared;           /* number of the shared file */
} Epoints;

typedef struct 
{
    int ok;
    char tmp_file[TMPL];
    unsigned long  shl;
} Hd;

static Hd  hd[ENTRYMAX]; /* shared libs handler */
static int Nshared = 0;
static int NEpoints = 0; /* Number of Linked names */
static Epoints EP[ENTRYMAX];  /* entryPoints */
/*---------------------------------------------------------------------------*/
int scilabLink(int _iLibID, wchar_t* _pwstLibraryName, wchar_t** _pwstEntryPointName, int _iEntryPointSize, BOOL _bFortran ,int *_piErr)
{
    int iLibID = -1; 

    if(_iLibID == -1) 
    {
        iLibID = Sci_dlopen(_pwstLibraryName);
    } 
    else 
    {
        iLibID = _iLibID;
    }

    if(iLibID == -1) 
    {
        if( getWarningMode() ) 
        {
#ifdef _MSC_VER
            if(isDllW(_pwstLibraryName))
            {
#ifdef _WIN64
                if(isX86DllW(_pwstLibraryName))
                {
                    if(getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprint(_("%s: can not to load a x86 dll in a x64 environment.\n" ), "link");
                    }
                    SetLastError(ERROR_DLL_INIT_FAILED);
                }
#else
                if(isX64DllW(_pwstLibraryName))
                {
                    if(getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
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
                if(pwstPathSearch == NULL)
                {
                    if(getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
                    {
                        sciprintW(_W("%ls: The file %ls does not exist in PATH environment.\n" ), L"link", _pwstLibraryName);
                    }
                }
            }
#else
            if(getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
            {
                sciprintW(_W("Link failed for dynamic library '%ls'.\n"), _pwstLibraryName);
                sciprint(_("An error occurred: %s\n"), GetLastDynLibError());
            }
#endif
        }
        *_piErr = -1;
        return iLibID;
    }

    if( (_iLibID == -1) && (getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)) 
    {
        sciprint(_("Shared archive loaded.\n"));
        sciprint(_("Link done.\n"));
    }

    for(int i = 0 ; i < _iEntryPointSize ; i++)
    {
        *_piErr = Sci_dlsym(_pwstEntryPointName[i], iLibID, _bFortran);
    }

    return iLibID;
}
/*---------------------------------------------------------------------------*/
char **getNamesOfFunctionsInSharedLibraries(int *sizearray)
{
    char **NamesOfFunctions = NULL;
    *sizearray = 0;

    if( (NEpoints) && (NEpoints > 0) )
    {
        int i = 0;
        NamesOfFunctions = (char **) MALLOC((NEpoints)*sizeof(char *));
        if(NamesOfFunctions)
        {
            for ( i = NEpoints-1 ; i >= 0 ; i--) 
            {
                if(EP[i].name)
                {
                    char *EntryName = (char *)MALLOC(((int)strlen(EP[i].name)+1)*sizeof(char));

                    if(EntryName)
                    {
                        (*sizearray)++;
                        strcpy(EntryName , EP[i].name);
                        NamesOfFunctions[(*sizearray)-1] = EntryName;
                    }
                }
            }
        }
    }
    return NamesOfFunctions;
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
    if(_bFortran)
    {
        wcscat(_pwstTrailingName, L"_");
    }
#endif
    return;
}
/*---------------------------------------------------------------------------*/
BOOL c_link(char *routinename,int *ilib)
{
    void (*loc)();
    if( *ilib != -1 ) *ilib = SearchFandS(routinename,*ilib);
    else *ilib = SearchInDynLinks(routinename,&loc);

    if(*ilib == -1) return FALSE;
    return TRUE;
}
/*---------------------------------------------------------------------------*/
void C2F(iislink)(char *routinename, int *ilib)
{
    c_link(routinename,ilib);
}
/*---------------------------------------------------------------------------*/
void GetDynFunc(int ii, void (**realop) ())
{
    //if( EP[ii].Nshared != -1 ) *realop = EP[ii].epoint;
    //else *realop = (function) 0;
}
/*---------------------------------------------------------------------------*/
int SearchInDynLinks(char *op, void (**realop) ())
{
    //int i=0;
    //for ( i = NEpoints-1 ; i >=0 ; i--) 
    //{
    //    if( strcmp(op,EP[i].name) == 0) 
    //    {
    //        *realop = EP[i].epoint;
    //        return(EP[i].Nshared );
    //    }
    //}
    return(-1);
}
/*---------------------------------------------------------------------------*/
/**
* Search a (function,libid) in the table 
* Search from end to top 
*/
static int SearchFandS(char *op, int ilib)
{
    int i = 0;
    for ( i = NEpoints-1 ; i >=0 ; i--) 
    {
        if( strcmp(op,EP[i].name) == 0 && EP[i].Nshared == ilib)
        {
            return(i);
        }
    }
    return(-1);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void unlinkallsharedlib(void)
{
    int i=0;
    for ( i = 0 ; i < Nshared ; i++) 
    {
        unlinksharedlib(&i);
    }
}
/*---------------------------------------------------------------------------*/
void unlinksharedlib(int *i) 
{
    /* delete entry points in shared lib *i */
    Sci_Delsym(*i);
    /* delete entry points used in addinter in shared lib *i */
    RemoveInterf(*i);
}
/*---------------------------------------------------------------------------*/
int Sci_dlclose(unsigned long _hLib)
{
#ifdef _MSC_VER
        return FreeDynLibrary ((DynLibHandle) ULongToHandle(_hLib));
#else
        return FreeDynLibrary ((DynLibHandle) _hLib);
#endif
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

    if(hLib == NULL)
    {
        return -1 ; /* the shared archive was not loaded. */
    }

    /* Warning x64 windows */

    ConfigVariable::DynamicLibraryStr* pDL = ConfigVariable::getNewDynamicLibraryStr();
    ConfigVariable::setLibraryName(pDL, _pwstDynLibPath);
#ifdef _MSC_VER
    pDL->hLib =   PtrToUlong(hLib);
#else
    pDL->hLib = (unsigned long)hLib;
#endif

    
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

    
    if(_iLibID < 0 || ConfigVariable::isDynamicLibrary(_iLibID) == false)
    {//no valid library at this ID
        return -3;
    }

    /** entry was previously loaded **/
    if(ConfigVariable::getEntryPoint(_pwstEntryPointName, _iLibID) != NULL)
    {
        sciprintW(_W("Entry name %ls.\n"), _pwstEntryPointName);
        return -4;
    }

    /* Warning x64 windows */
    pEP->iLibIndex = _iLibID;
#ifdef _MSC_VER
    hDynLib = (DynLibHandle)  ULongToHandle(ConfigVariable::getDynamicLibrary(_iLibID)->hLib);
#else
    hDynLib = (DynLibHandle)  ConfigVariable::getDynamicLibrary(_iLibID)->hLib;
#endif
#ifdef _MCS_VER
    pEP->functionPtr = (function) GetDynLibFuncPtrW(hDynLib, pwstEntryPointName);
#else
    char* pstEntryPointName = wide_string_to_UTF8(pwstEntryPointName);
    pEP->functionPtr = (function) GetDynLibFuncPtr(hDynLib, pstEntryPointName);
    FREE(pstEntryPointName);
#endif    
    if(pEP->functionPtr == NULL)
    {
        if(getIlibVerboseLevel() != ILIB_VERBOSE_NO_OUTPUT)
        {
            sciprintW(_W("%ls is not an entry point.\n"), _pwstEntryPointName);
        }
        return -5;
    }

    /* we don't add the _ in the table */
    if(debug)
    {
        sciprintW(_W("Linking %ls.\n"), _pwstEntryPointName);
    }

    ConfigVariable::setEntryPointName(pEP, _pwstEntryPointName);
    ConfigVariable::addEntryPoint(pEP);
    FREE(pwstEntryPointName);
    return 0;  
}
/*---------------------------------------------------------------------------*/
void Sci_Delsym(int ishared) 
{
    int ish = Min(Max(0,ishared),ENTRYMAX-1);
    int i=0;
    for ( i = NEpoints-1 ; i >=0 ; i--) 
    {
        if( EP[i].Nshared == ish )
        {
            int j;
            for ( j = i ; j <= NEpoints - 2 ; j++ )
            {
                EP[j].epoint = EP[j+1].epoint;
                EP[j].Nshared = EP[j+1].Nshared;
                strcpy(EP[j].name,EP[j+1].name);
            }
            NEpoints--;
        }
    }
    if( hd[ish].ok != FALSE)
    {
        /* Warning x64 windows */
#ifdef _MSC_VER
        FreeDynLibrary ((DynLibHandle) ULongToHandle(hd[ish].shl));
#else
        FreeDynLibrary ((DynLibHandle) hd[ish].shl);
#endif
        hd[ish].ok = FALSE;
    }
}
/*---------------------------------------------------------------------------*/
