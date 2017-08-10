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
#include "scicurdir.h"
}

namespace org_modules_external_objects
{

int ScilabGateway::newInstance(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int idClass = 0;
    int * tmpvar = 0;
    int * args = 0;
    int ret = 0;
    char * className = 0;
    int error = 0;
    char * cwd = 0;
    int nbArgs = Rhs - 1;

    if (Rhs == 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong number of arguments : more than 1 argument expected"));
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

    if (isStringType(pvApiCtx, addr))
    {
        cwd = scigetcwd(&error);
        if (error && cwd)
        {
            FREE(cwd);
            cwd = 0;
        }

        className = ScilabObjects::getSingleString(1, pvApiCtx);
        try
        {
            idClass = env.loadclass(className, cwd, false, helper.getAllowReload());
        }
        catch (std::exception & /*e*/)
        {
            if (cwd)
            {
                FREE(cwd);
            }
            freeAllocatedSingleString(className);
            throw;
        }
        if (cwd)
        {
            FREE(cwd);
        }
        freeAllocatedSingleString(className);
    }
    else if (ScilabObjects::isExternalClass(addr, pvApiCtx))
    {
        idClass = ScilabObjects::getArgumentId(addr, 0, false, true, envId, pvApiCtx);
    }

    tmpvar = new int[Rhs];
    *tmpvar = 0;
    args = new int[Rhs - 1];

    for (int i = 0; i < Rhs - 1; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 2, &addr);
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
        ret = env.newinstance(idClass, args, nbArgs);
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

    try
    {
        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & /*e*/)
    {
        env.removeobject(ret);
        throw;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
