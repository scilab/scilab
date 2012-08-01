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

namespace org_modules_external_objects
{

int ScilabGateway::cast(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int tmpvar[2] = {0, 0};
    int * addr = 0;
    int idObj = 0;
    int row = 0, col = 0;
    int * id = 0;
    char * objName = 0;
    char * errmsg = 0;
    int ret = 0;

    CheckInputArgument(pvApiCtx, 2, 2);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccured(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, envId, pvApiCtx);

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (ScilabObjects::isExternalClass(addr, pvApiCtx))
    {
        if (ScilabObjects::getEnvironmentId(addr, pvApiCtx) != envId)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Incompatible External Objects"));
        }

        err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        try
        {
            ret = env.castwithid(idObj, *id);
        }
        catch (std::exception & e)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            throw;
        }
    }
    else
    {
        try
        {
            objName = ScilabObjects::getSingleString(2, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & e)
        {
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            throw;
        }

        try
        {
            ret = env.cast(idObj, objName);
        }
        catch (std::exception & e)
        {
            freeAllocatedSingleString(objName);
            ScilabObjects::removeTemporaryVars(envId, tmpvar);
            throw;
        }
        freeAllocatedSingleString(objName);
    }

    ScilabObjects::removeTemporaryVars(envId, tmpvar);

    try
    {
        ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, envId, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & e)
    {
        env.removeobject(ret);
        throw;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
