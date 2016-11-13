/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include "configvariable.hxx"

extern "C"
{
#include <stdlib.h>
#include "dynamic_link.h"
#include "addinter.h"
}

typedef void(*function)(char*);

int AddInterfaceToScilab(const char* _pstDynamicLibraryName, const char* _pstModuleName, const char** _pstEntryPointName, int _iEntryPointSize)
{
    int iLibID = -1; /* Id of library */
    int iErr = 0;

    /** Try to unlink the interface if it was previously linked **/
    ConfigVariable::EntryPointStr* pEP = ConfigVariable::getEntryPoint(_pstModuleName);
    if (pEP)
    {
        //entry point already linked, so remove it before add it
        ConfigVariable::removeDynamicLibrary(pEP->iLibIndex);
    }

    /* link then search  */
    /* Haven't been able to find the symbol. Try C symbol */
    iLibID =  scilabLink(iLibID, _pstDynamicLibraryName, &_pstModuleName, 1, FALSE, &iErr);
    if (iErr)
    {
        /* Trying with the fortran symbol */
        iLibID =  scilabLink(iLibID, _pstDynamicLibraryName, &_pstModuleName, 1, TRUE, &iErr);
        if (iErr)
        {
            return iErr;
        }
    }

    pEP = ConfigVariable::getEntryPoint(_pstModuleName);
    if (pEP == NULL)
    {
        //
        return -1;
    }

    for (int i = 0 ; i < _iEntryPointSize ; i++)
    {
        ((function)pEP->functionPtr)((char*)_pstEntryPointName[i]);
    }
    return 0;
}
