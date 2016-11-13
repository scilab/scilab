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

int ScilabGateway::addNamedVariable(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    char * varName = 0;
    int tmpvar[] = {0, 0};
    int idObj;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    varName = ScilabObjects::getSingleString(1, pvApiCtx);

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        freeAllocatedSingleString(varName);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    try
    {
        idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & /*e*/)
    {
        freeAllocatedSingleString(varName);
        throw;
    }

    try
    {
        env.addNamedVariable(idObj, varName);
    }
    catch (std::exception & /*e*/)
    {
        freeAllocatedSingleString(varName);
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        throw;
    }

    freeAllocatedSingleString(varName);
    ScilabObjects::removeTemporaryVars(envId, tmpvar);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
