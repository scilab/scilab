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

int ScilabGateway::operation(char * fname, const int envId, const OperatorsType type, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int tmpvar[] = {0, 0};
    int idObjA;
    int idObjB = -1;
    int eId = 0;
    int ret;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    try
    {
        eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & /*e*/)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
        eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);
    }

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(eId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    getVarAddressFromPosition(pvApiCtx, 1, &addr);
    idObjA = ScilabObjects::getArgumentId(addr, tmpvar, false, false, eId, pvApiCtx);

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        ScilabObjects::removeTemporaryVars(envId, tmpvar);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (Rhs == 2)
    {
        try
        {
            idObjB = ScilabObjects::getArgumentId(addr, tmpvar, false, false, eId, pvApiCtx);
        }
        catch (std::exception & /*e*/)
        {
            throw;
        }
    }

    try
    {
        ret = env.operation(idObjA, idObjB, type);
    }
    catch (std::exception & /*e*/)
    {
        ScilabObjects::removeTemporaryVars(eId, tmpvar);
        throw;
    }

    ScilabObjects::removeTemporaryVars(eId, tmpvar);

    if (helper.getAutoUnwrap())
    {
        if (!ScilabObjects::unwrap(ret, Rhs + 1, eId, pvApiCtx))
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, eId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                env.removeobject(ret);
                throw;
            }
        }
        else
        {
            env.removeobject(ret);
        }
    }
    else
    {
        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, eId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & /*e*/)
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
