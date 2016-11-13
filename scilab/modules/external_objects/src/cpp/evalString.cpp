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

namespace org_modules_external_objects
{

int ScilabGateway::evalString(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int row;
    int col;
    char ** code = 0;
    ScilabStringStackAllocator * allocator = 0;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (!isStringType(pvApiCtx, addr))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: string expected."), 1);
    }

    err = getVarDimension(pvApiCtx, addr, &row, &col);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if ((row < 1 || col != 1) && (col < 1 || row != 1))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimension for argument #%d: A row or a column expected."), 1);
    }

    if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &code))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (Rhs == 2)
    {
        int val;

        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isBooleanType(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A boolean expected."), 2);
        }

        if (!isScalar(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A single boolean expected."), 2);
        }

        getScalarBoolean(pvApiCtx, addr, &val);

        if (val)
        {
            allocator = new ScilabStringStackAllocator(pvApiCtx, Rhs + 1);
        }
    }

    try
    {
        env.evalString(const_cast<const char **>(code), row * col, allocator);
    }
    catch (std::exception & /*e*/)
    {
        freeAllocatedMatrixOfString(row, col, code);
        throw;
    }

    if (allocator)
    {
        delete allocator;
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }

    PutLhsVar();

    return 0;
}
}
