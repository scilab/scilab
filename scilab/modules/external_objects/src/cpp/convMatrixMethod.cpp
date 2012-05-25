/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabGateway.hxx"
#include <cstring>

namespace org_modules_external_objects
{

int ScilabGateway::convMatrixMethod(char * fname, const int envId, void * pvApiCtx)
{
    char * method = 0;
    int rc = 0;
    int cr = 0;

    CheckInputArgument(pvApiCtx, 0, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    if (Rhs == 0)
    {
        SciErr err;
        const char * meth = "cr";
        if (helper.getMethodOfConv())
        {
            meth = "rc";
        }

        err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, const_cast<const char * const *>(&meth));
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }

    method = ScilabObjects::getSingleString(1, pvApiCtx);

    rc = std::strcmp(method, "rc");
    cr = std::strcmp(method, "cr");

    freeAllocatedSingleString(method);

    if (rc && cr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("The argument must be \'rc\' or \'cr\'."));
    }

    helper.setMethodOfConv(cr != 0);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
