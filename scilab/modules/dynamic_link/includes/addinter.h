/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

#ifndef __ADDINTER_H__
#define  __ADDINTER_H__

#include <wchar.h>
#include "dynlib_dynamic_link.h"
#include "machine.h"

/**
* the first dynamic interface is at position 500+1
*/
#define DynInterfStart 500

#define NumberMaxFunctionsByGateway 1000

#define MAXDYNINTERF 50 /* default value compatibility scilab 4.x */
/* !!! WARNING !!! */
/* On Windows , AddInterfaceToScilab based on LoadLibrary C function */
/* you cannot load more than 80 dynamic libraries at the same time. */
/* Scilab will return a error (code Windows 1114) in this case.*/
/* A dynamic link library (DLL) initialization routine failed. */


/**
* Remove interface in scilab
* @param[in] id of interface
*/
DYNAMIC_LINK_IMPEXP void RemoveInterf(int id);

/**
* Add a interface in Scilab
* @param[in] filenamelib dynamic libraries filename
* @param[in] spname Name of interface routine entry point
* @param[in] fcts Name of new Scilab function implemented in the new interface
* @param[in] sizefcts size of fcts array
* @return a int id of interface (-1 failed)
*/
DYNAMIC_LINK_IMPEXP int AddInterfaceToScilab(wchar_t* _pwstDynamicLibraryName, wchar_t* _pwstModuleName, wchar_t** _pwstEntryPointName, int _iEntryPointSize);

#endif /*  __ADDINTER_H__ */
