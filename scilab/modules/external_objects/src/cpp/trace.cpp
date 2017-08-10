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

extern "C"
{
#include "expandPathVariable.h"
#include "sci_malloc.h"
}

namespace org_modules_external_objects
{

int ScilabGateway::trace(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    char * filename = 0;
    bool disable = true;

    CheckInputArgument(pvApiCtx, 0, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    if (Rhs == 1)
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isEmptyMatrix(pvApiCtx, addr))
        {
            disable = false;
        }
    }

    if (disable)
    {
        env.disabletrace();
    }
    else
    {
        filename = ScilabObjects::getSingleString(1, pvApiCtx);
        char * expandedPath = expandPathVariable(const_cast<char *>(filename));
        if (expandedPath)
        {
            freeAllocatedSingleString(filename);
            try
            {
                env.enabletrace(expandedPath);
            }
            catch (std::exception & /*e*/)
            {
                FREE(expandedPath);
                throw;
            }
            FREE(expandedPath);
        }
        else
        {
            std::string str(filename);
            freeAllocatedSingleString(filename);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot open the given file %s."), str.c_str());
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
