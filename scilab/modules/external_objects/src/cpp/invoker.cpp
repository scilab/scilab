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

int ScilabGateway::invoke(char * fname, const int envId, ScilabAbstractInvoker & invoker, void * pvApiCtx)
{
    SciErr err;
    int * tmpvar = 0;
    int * addr = 0;
    int * args = 0;
    int ret = 0;
    int nbArgs = Rhs;

    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper & helper = env.getOptionsHelper();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    tmpvar = new int[Rhs + 1];
    *tmpvar = 0;
    args = new int[Rhs];

    for (int i = 0; i < Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
        if (err.iErr)
        {
            delete[] args;
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
        try
        {
            args[i] = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & /*e*/)
        {
            delete[] args;
            delete[] tmpvar;
            throw;
        }

        if (args[i] == VOID_OBJECT)
        {
            nbArgs = 0;
        }
    }

    try
    {
        ret = invoker.invoke(args, nbArgs);
    }
    catch (std::exception & /*e*/)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        delete[] tmpvar;
        throw;
    }

    delete[] args;
    ScilabObjects::removeTemporaryVars(envId, tmpvar);
    delete[] tmpvar;

    if (ret == VOID_OBJECT)
    {
        PutLhsVar();

        return 0;
    }

    if (helper.getAutoUnwrap())
    {
        if (!ScilabObjects::unwrap(ret, Rhs + 1, envId, pvApiCtx))
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                env.removeobject(ret);
                throw;
            }
        }
        else
        {
            env.removeobject(ret);
        }
    }
    else
    {
        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & /*e*/)
        {
            env.removeobject(ret);
            throw;
        }
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
