/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

/*-----------------------------------------------------------------------------------*/
#ifndef __DYNAMIC_LINK_H__
#define __DYNAMIC_LINK_H__

#include <wchar.h> /* wchar_t */
#include "dynlib_dynamic_link.h"
#include "dynamiclibrary.h"
#include "BOOL.h"

/**
* load a shared archive and call LoadDynLibrary
* the shared lib handler is stored in a Table
* return value is == -1 if the LoadDynLibrary failed
* @param loaded_file
*/
DYNAMIC_LINK_IMPEXP int Sci_dlopen(wchar_t* _pwstDynLibPath);

/**
* unload a shared library by calling FreeDynLibrary
* return value is == 0 if the FreeDynLibrary failed
* @param loaded_file
*/
DYNAMIC_LINK_IMPEXP int Sci_dlclose(DynLibHandle _hLib);

/**
* This routine load the entryname ename
* from shared lib ishared
* @return TRUE or FALSE
*/
DYNAMIC_LINK_IMPEXP int Sci_dlsym(wchar_t* _pwstEntryPointName, int _iLibID, BOOL _bFortran);

/**
* call link for scilab
* @param _iLibID                : Id of an existing shared lib otherwise -1
* @param _pwstLibraryName       : Dynamic library name
* @param _pwstEntryPointName    : List of functions name in dynamic library
* @param _iEntryPointSize       : Size of _pwstEntryPointName
* @param _bFortran              : Is a link on a fortran function
* @param _piErr                 : Error Id
* @return id                    : Dynamic Library ID
*/
DYNAMIC_LINK_IMPEXP int scilabLink(int _iLibID, wchar_t* _pwstLibraryName, wchar_t** _pwstEntryPointName, int _iEntryPointSize, BOOL _bFortran , int *_piErr);

#endif /* __DYNAMIC_LINK_H__ */
/*-----------------------------------------------------------------------------------*/
