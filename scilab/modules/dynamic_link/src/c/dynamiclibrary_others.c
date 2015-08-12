/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "dynamiclibrary_others.h"
#include <stdlib.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
#ifndef NULL
#define NULL 0
#endif
/*---------------------------------------------------------------------------*/
BOOL FreeDynLibrary(DynLibHandle hInstance)
{
    if (hInstance)
    {

        if (dlclose( hInstance) == 0)
        {
            return TRUE;
        }
        else
        {
            fprintf(stderr, "Could not free library %s\n", dlerror());
        }
    }
#ifndef NDEBUG
    else
    {
        fprintf(stderr, "FreeDynLibrary: Cannot close a not-opened library.\n");
        fflush(NULL);
    }
#endif

    return FALSE;
}
/*---------------------------------------------------------------------------*/
DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, char *funcName)
{
    if (hInstance)
    {
        return dlsym(hInstance, funcName);
    }
    return NULL;
}
/*---------------------------------------------------------------------------*/
