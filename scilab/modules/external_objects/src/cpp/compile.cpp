/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabGateway.hxx"

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

    CheckInputArgument(pvApiCtx, 2, 2);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    className = ScilabObjects::getSingleString(1, pvApiCtx);

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
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
    catch (std::exception & e)
    {
        freeAllocatedSingleString(className);
        freeAllocatedMatrixOfString(row, col, code);
        throw;
    }

    freeAllocatedSingleString(className);
    freeAllocatedMatrixOfString(row, col, code);

    try
    {
        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_CLASS, Rhs + 1, ret, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        env.removeobject(ret);
        throw;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
