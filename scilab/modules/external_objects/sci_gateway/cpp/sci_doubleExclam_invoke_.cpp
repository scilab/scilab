/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabGateway.hxx"

extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "gw_external_objects.h"
}

using namespace org_modules_external_objects;

int sci_doubleExclam_invoke_(char * fname, void* pvApiCtx)
{
    try
    {
        int* addr = NULL;
        double dbl = 0;
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
        if (sciErr.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (getScalarDouble(pvApiCtx, addr, &dbl))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        return ScilabGateway::doubleExclam_invoke(fname, static_cast<int>(dbl), pvApiCtx);
    }
    catch (std::exception & e)
    {
        Scierror(999, _("%s: An error occurred: %s"), "Method invocation", e.what());
        return 0;
    }
}
