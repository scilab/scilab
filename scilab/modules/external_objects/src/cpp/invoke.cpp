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

int ScilabGateway::invoke(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * tmpvar = 0;
    int * addr = 0;
    int * args = 0;
    int idObj = 0;
    int * ret = 0;
    char * methName = 0;
    int nbArgs = Rhs - 2;
    std::vector<int> torem;

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
        err = getVarAddressFromPosition(pvApiCtx, i + 3, &addr);
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
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
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

        createEmptyMatrix(pvApiCtx, Rhs + 1);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }

    if (helper.getAutoUnwrap())
    {
        torem.reserve(*ret);
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
                    if (!torem.empty())
                    {
                        env.removeobject(&(torem[0]), static_cast<int>(torem.size()));
                    }
                    env.removeobject(ret + 1, *ret);
                    delete[] ret;
                    throw;
                }
            }
            else
            {
                torem.push_back(ret[i]);
            }

            LhsVar(i) = Rhs + i;
        }

        if (!torem.empty())
        {
            env.removeobject(&(torem[0]), static_cast<int>(torem.size()));
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
                env.removeobject(ret + 1, *ret);
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
