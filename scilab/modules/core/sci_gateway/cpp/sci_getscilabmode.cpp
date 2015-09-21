/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "funcmanager.hxx"
#include "context.hxx"
#include "core_gw.hxx"
#include "string.hxx"

extern "C"
{
#include "configvariable_interface.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getscilabmode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int n1 = 0, m1 = 0;
    char *output = NULL ;

    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "getscilabmode", 0);
        return types::Function::Error;
    }

    const char* pst = getScilabModeString();
    types::String* pS = new types::String(pst);
    out.push_back(pS);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
