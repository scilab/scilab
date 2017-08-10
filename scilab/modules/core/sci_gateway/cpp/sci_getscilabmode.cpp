/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
