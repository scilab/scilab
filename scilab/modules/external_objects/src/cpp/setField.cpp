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

int ScilabGateway::setField(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int tmpvar[3] = {0, 0, 0};
    int * addr = 0;
    int idObj = 0;
    char * fieldName = 0;
    int arg = 0;

    CheckInputArgument(pvApiCtx, 3, 3);

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

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        freeAllocatedSingleString(fieldName);
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    try
    {
        arg = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        freeAllocatedSingleString(fieldName);
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        throw;
    }

    try
    {
        env.setfield(idObj, fieldName, arg);
    }
    catch (std::exception & e)
    {
        freeAllocatedSingleString(fieldName);
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        throw;
    }

    freeAllocatedSingleString(fieldName);
    ScilabObjects::removeTemporaryVars(envId, tmpvar);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
