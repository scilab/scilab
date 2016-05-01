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

int ScilabGateway::addToClasspath(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int rows;
    int cols;
    char ** className = 0;

    CheckInputArgumentAtLeast(pvApiCtx, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    for (int i = 1; i <= Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isStringType(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for argument #%d: string expected."), 1);
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &rows, &cols, &className))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        for (int j = 0; j < rows * cols; j++)
        {
            char * expandedPath = expandPathVariable(const_cast<char *>(className[j]));
            if (expandedPath)
            {
                try
                {
                    env.addtoclasspath(expandedPath);
                }
                catch (std::exception & /*e*/)
                {
                    FREE(expandedPath);
                    freeAllocatedMatrixOfString(rows, cols, className);
                    throw;
                }
                FREE(expandedPath);
            }
            else
            {
                std::string str(className[j]);
                freeAllocatedMatrixOfString(rows, cols, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot open the given file %s."), str.c_str());
            }
        }

        freeAllocatedMatrixOfString(rows, cols, className);
        className = 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
