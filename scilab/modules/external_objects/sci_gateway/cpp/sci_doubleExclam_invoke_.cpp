/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
