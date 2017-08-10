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

int ScilabGateway::objectPrint(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int tmpvar[2] = {0, 0};
    std::string rep;
    int * addr = 0;
    int idObj = 0;
    int eId = 0;

    CheckInputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(eId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, eId, pvApiCtx);

    try
    {
        rep = env.getrepresentation(idObj);
    }
    catch (std::exception & /*e*/)
    {
        ScilabObjects::removeTemporaryVars(eId, tmpvar);
        throw;
    }

    ScilabStream ss;
    ss << rep << std::endl;

    PutLhsVar();

    return 0;
}
}
