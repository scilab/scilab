/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

int ScilabGateway::wrapInFloat(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int idObj;
    int type = 0;
    double * data = 0;
    int row, col;

    if (Rhs == 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong number of arguments : more than 1 argument expected."));
    }

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);
    const ScilabAbstractEnvironmentWrapper & wrapper = ScilabEnvironments::getEnvironment(envId).getWrapper();

    CheckOutputArgument(pvApiCtx, Rhs, Rhs);

    for (int i = 1; i < Rhs + 1; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        err = getVarType(pvApiCtx, addr, &type);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data type"));
        }

        if (type != sci_matrix || isVarComplex(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("%s: Wrong argument type at position %i: Double expected\n"), fname, i);
        }

        err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &data);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        idObj = ScilabObjects::wrapFloat(row, col, data, wrapper, false);

        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, idObj, envId, pvApiCtx);

        LhsVar(i) = Rhs + i;
    }

    PutLhsVar();

    return 0;
}
}
