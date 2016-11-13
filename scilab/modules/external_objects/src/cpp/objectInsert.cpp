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

int ScilabGateway::objectInsert(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int tmpvarB[2] = {0, 0};
    int * addr = 0;
    int * tab = 0;
    int idObjA = 0;
    int idObjB = 0;
    int envIdA = 0;
    int type = -1;
    char * fieldName = 0;

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    envIdA = ScilabObjects::getEnvironmentId(addr, pvApiCtx);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envIdA);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);
    const int inc = helper.getUseScilabIndex() ? -1 : 0;

    idObjA = ScilabObjects::getArgumentId(addr, 0, false, false, envIdA, pvApiCtx);

    err = getVarAddressFromPosition(pvApiCtx, Rhs - 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    idObjB = ScilabObjects::getArgumentId(addr, tmpvarB, false, false, envIdA, pvApiCtx);

    /*
     * If all the parameters are integer, we can guess that a matrix insertion is expected
     */
    tab = new int[Rhs - 2];

    for (int i = 1; i < Rhs - 1; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
            delete[] tab;
            tab = 0;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        tab[i - 1] = ScilabObjects::isPositiveIntegerAtAddress(addr, pvApiCtx);
        if (tab[i - 1] == -1)
        {
            delete[] tab;
            tab = 0;
            break;
        }
        tab[i - 1] += inc;
    }

    if (tab)
    {
        bool failed = false;

        try
        {
            env.setarrayelement(idObjA, tab, Rhs - 2, idObjB);
        }
        catch (std::exception & /*e*/)
        {
            delete[] tab;
            ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
            failed = true;
        }

        if (!failed)
        {
            delete[] tab;
            ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);

            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, idObjA, envIdA, pvApiCtx);

            LhsVar(1) = Rhs + 1;
            PutLhsVar();

            return 0;
        }
    }

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (isStringType(pvApiCtx, addr) && isScalar(pvApiCtx, addr) && getAllocatedSingleString(pvApiCtx, addr, &fieldName))
    {
        ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), 1);
    }

    if (fieldName)
    {
        try
        {
            type = env.getfieldtype(idObjA, fieldName);
        }
        catch (std::exception & /*e*/)
        {
            ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
            freeAllocatedSingleString(fieldName);
            throw;
        }
    }

    if (type == 1)
    {
        try
        {
            env.setfield(idObjA, fieldName, idObjB);
        }
        catch (std::exception & /*e*/)
        {
            ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
            freeAllocatedSingleString(fieldName);
            throw;
        }

        freeAllocatedSingleString(fieldName);
    }
    else
    {
        if (fieldName)
        {
            freeAllocatedSingleString(fieldName);
        }

        int * args = new int[Rhs - 1];
        int * tmpvar = new int[Rhs - 1];
        *tmpvar = 0;

        for (int i = 0; i < Rhs - 2; i++)
        {
            int * laddr = 0;

            err = getVarAddressFromPosition(pvApiCtx, i + 1, &laddr);
            if (err.iErr)
            {
                ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
                ScilabObjects::removeTemporaryVars(envIdA, tmpvar);
                delete[] tmpvar;
                delete[] args;
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            try
            {
                args[i] = ScilabObjects::getArgumentId(laddr, tmpvar, false, false, envIdA, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
                delete[] tmpvar;
                delete[] args;
                throw;
            }
        }

        args[Rhs - 2] = idObjB;

        try
        {
            env.insert(idObjA, args, Rhs - 1);
        }
        catch (std::exception & /*e*/)
        {
            ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);
            ScilabObjects::removeTemporaryVars(envIdA, tmpvar);
            delete[] tmpvar;
            delete[] args;
            throw;
        }

        ScilabObjects::removeTemporaryVars(envIdA, tmpvar);
        delete[] tmpvar;
        delete[] args;
    }

    ScilabObjects::removeTemporaryVars(envIdA, tmpvarB);

    ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, idObjA, envIdA, pvApiCtx);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
}
