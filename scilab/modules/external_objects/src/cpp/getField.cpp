/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabGateway.hxx"

namespace org_modules_external_objects
{

int ScilabGateway::getField(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int tmpvar[2] = {0, 0};
    int * addr = 0;
    int idObj = 0;
    int ret = 0;
    int mustUnwrap = 0;
    char * fieldName = 0;

    CheckInputArgument(pvApiCtx, 2, 3);
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

    idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);

    try
    {
        fieldName = ScilabObjects::getSingleString(2, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        throw;
    }

    if (Rhs == 3)
    {
        err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
        if (err.iErr)
        {
            freeAllocatedSingleString(fieldName);
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (getScalarBoolean(pvApiCtx, addr, &mustUnwrap))
        {
            freeAllocatedSingleString(fieldName);
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }
    else
    {
        mustUnwrap = static_cast<int>(env.getOptionsHelper().getAutoUnwrap());
    }

    try
    {
        ret = env.getfield(idObj, fieldName);
    }
    catch (std::exception & e)
    {
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        freeAllocatedSingleString(fieldName);
        throw;
    }

    ScilabObjects::removeTemporaryVars(envId, tmpvar);
    freeAllocatedSingleString(fieldName);

    if (mustUnwrap && ScilabObjects::unwrap(ret, Rhs + 1, envId, pvApiCtx))
    {
        env.removeobject(ret);
    }
    else
    {
        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, envId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & e)
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

