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

int ScilabGateway::objectExtract(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    char * fieldName = 0;
    int tmpvar[2] = {0, 0};
    int * addr = 0;
    int * tab = 0;
    int idObj = 0;
    int eId = 0;
    int ret = 0;
    int type = -1;

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    eId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(eId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);
    const int inc = helper.getUseScilabIndex() ? -1 : 0;

    idObj = ScilabObjects::getArgumentId(addr, tmpvar, false, false, eId, pvApiCtx);

    /*
     * If all the parameters are integer, we can guess that a matrix extraction is expected
     */
    tab = new int[Rhs - 1];

    for (int i = 1; i < Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            ScilabObjects::removeTemporaryVars(eId, tmpvar);
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

    ScilabObjects::removeTemporaryVars(eId, tmpvar);

    if (tab)
    {
        try
        {
            ret = env.getarrayelement(idObj, tab, Rhs - 1);
        }
        catch (std::exception & /*e*/)
        {
        }

        delete[] tab;
    }

    if (!ret)
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (isStringType(pvApiCtx, addr) && isScalar(pvApiCtx, addr) && getAllocatedSingleString(pvApiCtx, addr, &fieldName))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), 1);
        }

        if (fieldName)
        {
            try
            {
                type = env.getfieldtype(idObj, fieldName);
            }
            catch (std::exception & /*e*/)
            {
                freeAllocatedSingleString(fieldName);
                throw;
            }

            if (type == -1)
            {
                freeAllocatedSingleString(fieldName);
            }
        }

        if (type == -1)
        {
            int * args = new int[Rhs - 1];
            int * tmpvar = new int[Rhs];
            *tmpvar = 0;

            for (int i = 0; i < Rhs - 1; i++)
            {
                int * laddr = 0;

                err = getVarAddressFromPosition(pvApiCtx, i + 1, &laddr);
                if (err.iErr)
                {
                    ScilabObjects::removeTemporaryVars(eId, tmpvar);
                    delete[] tmpvar;
                    delete[] args;
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                try
                {
                    args[i] = ScilabObjects::getArgumentId(laddr, tmpvar, false, false, eId, pvApiCtx);
                }
                catch (ScilabAbstractEnvironmentException & /*e*/)
                {
                    delete[] tmpvar;
                    delete[] args;
                    throw;
                }
            }

            try
            {
                ret = env.extract(idObj, args, Rhs - 1);
            }
            catch (std::exception & /*e*/)
            {
                ScilabObjects::removeTemporaryVars(eId, tmpvar);
                delete[] tmpvar;
                delete[] args;
                throw;
            }

            ScilabObjects::removeTemporaryVars(eId, tmpvar);
            delete[] tmpvar;
            delete[] args;
        }
        else
        {
            if (type == 0)
            {
                // setMethodName will free fieldName
                options.setMethodName(fieldName);
                options.setObjId(idObj);
                OptionsHelper::setEnvId(eId);
                ScilabObjects::copyInvocationMacroToStack(Rhs + 1, eId, options.getIsNew(), pvApiCtx);

                LhsVar(1) = Rhs + 1;
                PutLhsVar();

                return 0;
            }
            else if (type == 1)
            {
                try
                {
                    ret = env.getfield(idObj, fieldName);
                }
                catch (std::exception & /*e*/)
                {
                    freeAllocatedSingleString(fieldName);
                    throw;
                }

                freeAllocatedSingleString(fieldName);
            }
        }
    }

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
