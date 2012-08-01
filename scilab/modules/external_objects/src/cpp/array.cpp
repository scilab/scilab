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

int ScilabGateway::array(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int ret = 0;
    int * addr = 0;
    char * className = 0;
    int * args = 0;
    char * errmsg = 0;

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);

    if (Rhs < 2)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong number of arguments : more than %d expected."), 2);
    }

    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    env.getGatewayOptions().setIsNew(false);

    className = ScilabObjects::getSingleString(1, pvApiCtx);

    args = new int[Rhs - 1];

    for (int i = 0; i < Rhs - 1; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 2, &addr);
        if (err.iErr)
        {
            delete[] args;
            freeAllocatedSingleString(className);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        args[i] = ScilabObjects::isPositiveIntegerAtAddress(addr, pvApiCtx);

        if (args[i] == -1 || args[i] == 0)
        {
            delete[] args;
            freeAllocatedSingleString(className);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("A strictly positive integer is expected at position %d."), i + 2);
        }
    }

    try
    {
        ret = env.createarray(className, args, Rhs - 1);
    }
    catch (std::exception & e)
    {
        delete[] args;
        freeAllocatedSingleString(className);
        throw;
    }

    delete[] args;
    freeAllocatedSingleString(className);

    try
    {
        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, envId, pvApiCtx);
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
