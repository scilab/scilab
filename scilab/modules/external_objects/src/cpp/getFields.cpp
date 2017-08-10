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

int ScilabGateway::getFields(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int tmpvar[2] = {0, 0};
    int idObj = 0;
    ScilabStringStackAllocator * allocator = 0;

    CheckInputArgument(pvApiCtx, 1, 1);

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

    if (!ScilabObjects::isExternalObjOrClass(addr, pvApiCtx))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An External Object expected."), 1);
    }

    idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);

    allocator = new ScilabStringStackAllocator(pvApiCtx, Rhs + 1);

    try
    {
        env.getaccessiblefields(idObj, *allocator);
    }
    catch (std::exception & /*e*/)
    {
        delete allocator;
        throw;
    }
    delete allocator;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
