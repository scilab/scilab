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
extern "C"
{
#include "os_string.h"
}
namespace org_modules_external_objects
{

int ScilabGateway::getEnvironmentName(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int eId = 0;
    std::string envName;
    char * str;

    CheckInputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (!ScilabObjects::isExternalObjOrClass(addr, pvApiCtx))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An External Object expected."), 1);
    }

    eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(eId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    envName = env.getEnvironmentName();
    str = os_strdup(envName.c_str());

    err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, (const char * const *)&str);
    free(str);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
