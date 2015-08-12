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

/**
 * The function !!_invoke_l is called by !_invoke_ which is called on object.method
 */

namespace org_modules_external_objects
{

int ScilabGateway::doubleExclam_invoke(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    int * tmpvar = 0;
    int * args = 0;
    int eId = OptionsHelper::getEnvId();
    int ret = 0;
    int * sret = 0;
    int len = Rhs;
    int nbArgs = len;
    std::vector<int> torem;

    CheckOutputArgument(pvApiCtx, 1, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(eId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabObjects::initialization(env, pvApiCtx);

    if (!OptionsHelper::getCopyOccurred()) // if the function is called outside a method context, then return null
    {
        //TODO: get null for environment
        ScilabObjects::unwrap(0, Rhs + 1, eId, pvApiCtx);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }

    OptionsHelper::setCopyOccurred(false);

    if (!options.getMethodName())
    {
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }

    if (Rhs == 1)
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (ScilabObjects::isExternalVoid(addr, pvApiCtx))
        {
            len = 0;
        }
    }

    tmpvar = new int[len + 1];
    *tmpvar = 0;
    args = new int[len];

    for (int i = 0; i < len; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
        if (err.iErr)
        {
            ScilabObjects::removeTemporaryVars(eId, tmpvar);
            delete[] tmpvar;
            delete[] args;
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        try
        {
            args[i] = ScilabObjects::getArgumentId(addr, tmpvar, false, false, eId, pvApiCtx);
        }
        catch (ScilabAbstractEnvironmentException & e)
        {
            delete[] tmpvar;
            delete[] args;
            throw;
        }
    }

    if (options.getIsNew())
    {
        try
        {
            ret = env.newinstance(options.getObjId(), args, len);
        }
        catch (std::exception & e)
        {
            options.setIsNew(false);
            ScilabObjects::removeTemporaryVars(eId, tmpvar);
            delete[] tmpvar;
            delete[] args;
            throw;
        }

        options.setIsNew(false);
        ScilabObjects::removeTemporaryVars(eId, tmpvar);
        delete[] tmpvar;
        delete[] args;
        options.setMethodName(0);

        if (ret == VOID_OBJECT)
        {
            PutLhsVar();

            return 0;
        }

        try
        {
            ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + 1, ret, eId, pvApiCtx);
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
    else
    {
        try
        {
            sret = env.invoke(options.getObjId(), options.getMethodName(), args, len);
        }
        catch (std::exception & e)
        {
            ScilabObjects::removeTemporaryVars(eId, tmpvar);
            delete[] tmpvar;
            delete[] args;
            options.setMethodName(0);
            throw;
        }
    }

    ScilabObjects::removeTemporaryVars(eId, tmpvar);
    delete[] tmpvar;
    delete[] args;
    options.setMethodName(0);

    if (!sret || *sret <= 0 || (*sret == 1 && sret[1] == VOID_OBJECT))
    {
        if (sret)
        {
            delete[] sret;
        }

        PutLhsVar();

        return 0;
    }

    if (helper.getAutoUnwrap())
    {
        torem.reserve(*sret);
        for (int i = 1; i <= *sret; i++)
        {
            if (!ScilabObjects::unwrap(sret[i], Rhs + i, eId, pvApiCtx))
            {
                try
                {
                    ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, sret[i], eId, pvApiCtx);
                }
                catch (ScilabAbstractEnvironmentException & e)
                {
                    if (!torem.empty())
                    {
                        env.removeobject(&(torem[0]), torem.size());
                    }
                    env.removeobject(sret + 1, *sret);
                    delete[] sret;
                    throw;
                }
            }
            else
            {
                torem.push_back(sret[i]);
            }

            LhsVar(i) = Rhs + i;
        }

        if (!torem.empty())
        {
            env.removeobject(&(torem[0]), torem.size());
        }
    }
    else
    {
        for (int i = 1; i <= *sret; i++)
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(EXTERNAL_OBJECT, Rhs + i, sret[i], eId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & e)
            {
                env.removeobject(sret + 1, *sret);
                delete[] sret;
                throw;
            }

            LhsVar(i) = Rhs + i;
        }
    }

    delete[] sret;

    PutLhsVar();

    return 0;
}
}
