/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - 2013 - Scilab Enterprises - Calixte DENIZET
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
#include "os_string.h"
}
namespace org_modules_external_objects
{

int ScilabGateway::compile(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int row = 0, col = 0;
    int * addr = 0;
    char * className = 0;
    char ** code = 0;
    int ret = 0;
    int iType = 0;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    if (Rhs == 1)
    {
        className = os_strdup("");
    }
    else
    {
        className = ScilabObjects::getSingleString(1, pvApiCtx);
    }

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
    if (err.iErr)
    {
        freeAllocatedSingleString(className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, addr, &iType);
    if (err.iErr)
    {
        freeAllocatedSingleString(className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (iType != sci_strings)
    {
        freeAllocatedSingleString(className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), 2);
    }

    err = getVarDimension(pvApiCtx, addr, &row, &col);
    if (err.iErr)
    {
        freeAllocatedSingleString(className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if ((row < 1 || col != 1) && (col < 1 || row != 1))
    {
        freeAllocatedSingleString(className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimension for argument #%d: A row or a column expected."), 2);
    }

    if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &code))
    {
        freeAllocatedSingleString(className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    try
    {
        ret = env.compilecode(className, code, row != 1 ? row : col);
    }
    catch (std::exception & /*e*/)
    {
        freeAllocatedSingleString(className);
        freeAllocatedMatrixOfString(row, col, code);
        throw;
    }

    freeAllocatedSingleString(className);
    freeAllocatedMatrixOfString(row, col, code);

    if (ret == -1)
    {
        LhsVar(1) = 0;
    }
    else
    {
        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_CLASS, Rhs + 1, ret, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & /*e*/)
        {
            env.removeobject(ret);
            throw;
        }

        LhsVar(1) = Rhs + 1;
    }

    PutLhsVar();

    return 0;
}
}
