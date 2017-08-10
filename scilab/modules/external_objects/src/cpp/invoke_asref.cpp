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

int ScilabGateway::invoke_asref(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * tmpvar = 0;
    int * addr = 0;
    int * args = 0;
    int idObj = 0;
    int * ret = 0;
    char * methName = 0;
    int nbArgs = Rhs - 2;

    if (Rhs < 2)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong number of arguments : more than 2 arguments expected"));
    }

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper & helper = env.getOptionsHelper();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    tmpvar = new int[Rhs - 1];
    *tmpvar = 0;

    try
    {
        idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & /*e*/)
    {
        delete[] tmpvar;
        throw;
    }

    if (idObj == 0)
    {
        delete[] tmpvar;
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot invoke on null object"));
    }

    args = new int[Rhs - 2];

    for (int i = 0; i < Rhs - 2; i++)
    {
        char * varName = 0;

        try
        {
            varName =  ScilabObjects::getSingleString(i + 3, pvApiCtx);
            err = getVarAddressFromName(pvApiCtx, varName, &addr);
            if (err.iErr)
            {
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            args[i] = ScilabObjects::getArgumentId(addr, tmpvar, true, false, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & /*e*/)
        {
            if (varName)
            {
                freeAllocatedSingleString(varName);
            }

            delete[] args;
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            delete[] tmpvar;
            throw;
        }

        if (args[i] == VOID_OBJECT)
        {
            nbArgs = 0;
        }

        if (varName)
        {
            freeAllocatedSingleString(varName);
        }
    }

    try
    {
        methName = ScilabObjects::getSingleString(2, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & /*e*/)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        delete[] tmpvar;
        throw;
    }

    try
    {
        ret = env.invoke(idObj, methName, args, nbArgs);
    }
    catch (std::exception & /*e*/)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        delete[] tmpvar;
        freeAllocatedSingleString(methName);
        throw;
    }

    delete[] args;
    ScilabObjects::removeTemporaryVars(envId, tmpvar);
    delete[] tmpvar;
    freeAllocatedSingleString(methName);

    if (!ret || *ret <= 0 || (*ret == 1 && ret[1] == VOID_OBJECT))
    {
        if (ret)
        {
            delete[] ret;
        }

        PutLhsVar();

        return 0;
    }

    if (helper.getAutoUnwrap())
    {
        for (int i = 1; i <= *ret; i++)
        {
            if (!ScilabObjects::unwrap(ret[i], Rhs + i, envId, pvApiCtx))
            {
                try
                {
                    ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, ret[i], envId, pvApiCtx);
                }
                catch (ScilabAbstractEnvironmentException & /*e*/)
                {
                    for (int j = 1; j <= *ret; j++)
                    {
                        env.removeobject(ret[j]);
                    }
                    delete[] ret;
                    throw;
                }
            }
            else
            {
                env.removeobject(ret[i]);
            }

            LhsVar(i) = Rhs + i;
        }
    }
    else
    {
        for (int i = 1; i <= *ret; i++)
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, ret[i], envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                for (int j = 1; j <= *ret; j++)
                {
                    env.removeobject(ret[j]);
                }
                delete[] ret;
                throw;
            }

            LhsVar(i) = Rhs + i;
        }
    }

    delete[] ret;

    PutLhsVar();

    return 0;
}
}
