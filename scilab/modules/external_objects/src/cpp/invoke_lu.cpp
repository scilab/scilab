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

int ScilabGateway::invoke_lu(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int typ = 0;
    int * addr = 0;
    int * listaddr = 0;
    int len = 0;
    int * tmpvar = 0;
    int idObj = 0;
    int * args = 0;
    int * child = 0;
    char * methName = 0;
    int * eId;
    int row, col;
    int * ret = 0;
    int nbArgs = 0;
    std::vector<int> torem;

    CheckInputArgument(pvApiCtx, 4, 4);

    err = getVarAddressFromPosition(pvApiCtx, 4, &listaddr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, listaddr, &typ);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (typ != sci_list)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A List expected."), 4);
    }

    err = getListItemNumber(pvApiCtx, listaddr, &len);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    // Get the environment id
    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, addr, &typ);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (typ != sci_ints)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 2);
    }
    else
    {
        int prec;

        err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (prec != SCI_INT32)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 2);
        }

        err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, &eId);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(*eId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    // Get the object id
    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    err = getVarType(pvApiCtx, addr, &typ);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (typ != sci_ints)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 1);
    }
    else
    {
        int prec;
        int * id;

        err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (prec != SCI_INT32)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: An Integer32 expected."), 1);
        }

        err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, &id);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        idObj = *id;
    }

    if (idObj == 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot invoke on null object."), __FILE__, __LINE__);
    }

    tmpvar = new int[len + 1];
    *tmpvar = 0;

    args = new int[len];
    nbArgs = len;

    for (int i = 0; i < len; i++)
    {
        err = getListItemAddress(pvApiCtx, listaddr, i + 1, &child);
        if (err.iErr)
        {
            delete[] args;
            ScilabObjects::removeTemporaryVars(*eId, tmpvar);
            delete[] tmpvar;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        try
        {
            args[i] = ScilabObjects::getArgumentId(child, tmpvar, false, false, *eId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & /*e*/)
        {
            delete[] args;
            delete[] tmpvar;
            throw;
        }

        if (args[i] == VOID_OBJECT)
        {
            nbArgs = 0;
        }
    }

    try
    {
        methName = ScilabObjects::getSingleString(3, pvApiCtx);
    }
    catch (ScilabAbstractEnvironmentException & /*e*/)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(*eId, tmpvar);
        delete[] tmpvar;
        throw;
    }

    try
    {
        ret = env.invoke(idObj, methName, args, nbArgs);
    }
    catch (std::exception & /*e*/)
    {
        delete[] args;
        ScilabObjects::removeTemporaryVars(*eId, tmpvar);
        delete[] tmpvar;
        freeAllocatedSingleString(methName);
        throw;
    }

    delete[] args;
    ScilabObjects::removeTemporaryVars(*eId, tmpvar);
    delete[] tmpvar;
    freeAllocatedSingleString(methName);

    if (!ret || *ret <= 0 || (*ret == 1 && ret[1] == VOID_OBJECT))
    {
        if (ret)
        {
            delete[] ret;
        }

        PutLhsVar();

        return 0;
    }

    torem.reserve(*ret);

    for (int i = 1; i <= *ret; i++)
    {
        if (!ScilabObjects::unwrap(ret[i], Rhs + i, *eId, pvApiCtx))
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, ret[i], *eId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                if (!torem.empty())
                {
                    env.removeobject(&(torem[0]), static_cast<int>(torem.size()));
                }
                env.removeobject(ret + 1, *ret);
                delete[] ret;
                throw;
            }
        }
        else
        {
            torem.push_back(ret[i]);
        }

        LhsVar(i) = Rhs + i;
    }

    if (!torem.empty())
    {
        env.removeobject(&(torem[0]), static_cast<int>(torem.size()));
    }

    delete[] ret;

    PutLhsVar();

    return 0;
}
}
