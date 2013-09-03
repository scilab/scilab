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

int ScilabGateway::unwrapremove(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int row = 0, col = 0;
    int * id = 0;

    if (Rhs == 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong number of arguments : more than 1 argument expected"));
    }

    CheckOutputArgument(pvApiCtx, Rhs, Rhs);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    for (int i = 1; i <= Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!ScilabObjects::isExternalObj(addr, pvApiCtx))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An External Object expected."), i);

        }

        err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!ScilabObjects::unwrap(*id, Rhs + i, envId, pvApiCtx))
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, *id, envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & e) { }
        }

        LhsVar(i) = Rhs + i;

        env.removeobject(*id);
    }

    PutLhsVar();

    return 0;
}
}
