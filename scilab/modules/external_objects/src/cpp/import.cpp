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

extern "C" {
#include "scicurdir.h"
}

namespace org_modules_external_objects
{

int ScilabGateway::import(char * fname, const int envId, void * pvApiCtx)
{
    SciErr err;
    int rows, cols;
    char ** className = 0;
    std::string * name = 0;
    int named = 1;
    int * addr = 0;
    int ret = 0;
    int nbArgs = Rhs;
    int error = 0;
    char * cwd = 0;

    CheckInputArgumentAtLeast(pvApiCtx, 1);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    OptionsHelper & helper = env.getOptionsHelper();
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (isBooleanType(pvApiCtx, addr))
    {
        nbArgs = Rhs - 1;
        if (getScalarBoolean(pvApiCtx, addr, &named))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

    if (nbArgs == 1)
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isStringType(pvApiCtx, addr))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), 1);
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &rows, &cols, &className))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }
    else
    {
        className = (char**)MALLOC(sizeof(char *) * nbArgs);
        for (int i = 0; i < nbArgs; i++)
        {
            err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
            if (err.iErr)
            {
                freeAllocatedMatrixOfString(1, i, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            if (!isStringType(pvApiCtx, addr))
            {
                freeAllocatedMatrixOfString(1, i, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), i + 1);
            }

            if (getAllocatedSingleString(pvApiCtx, addr, &(className[i])))
            {
                freeAllocatedMatrixOfString(1, i, className);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }
        }

        rows = 1;
        cols = nbArgs;
    }

    if (named)
    {
        name = new std::string[rows * cols];

        for (int i = 0; i < rows * cols; i++)
        {
            name[i] = std::string(className[i]);
            if (helper.getUseLastName())
            {
                std::size_t pos = name[i].find_last_of('.');
                if (pos != std::string::npos)
                {
                    if (pos == name[i].size() - 1)
                    {
                        freeAllocatedMatrixOfString(rows, cols, className);
                        delete[] name;
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("The class name cannot end with a dot."));
                    }
                    name[i] = name[i].substr(pos + 1);
                }
            }
            else
            {
                std::size_t pos = name[i].find_first_of('.');
                if (pos != std::string::npos)
                {
                    if (pos == 0)
                    {
                        freeAllocatedMatrixOfString(rows, cols, className);
                        delete[] name;
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("The class name cannot start with a dot."));
                    }
                    name[i] = name[i].substr(0, pos);
                }
            }

            if (isNamedVarExist(pvApiCtx, name[i].c_str()))
            {
                addr = 0;
                err = getVarAddressFromName(pvApiCtx, name[i].c_str(), &addr);
                if (err.iErr || addr == 0 || !ScilabObjects::isValidExternal(addr, pvApiCtx) || ScilabObjects::getEnvironmentId(addr, pvApiCtx) != envId)
                {
                    freeAllocatedMatrixOfString(rows, cols, className);
                    delete[] name;
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("A variable with this name is already existing"));
                }
            }
        }
    }

    if (!named && rows * cols != Lhs)
    {
        freeAllocatedMatrixOfString(rows, cols, className);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Bad number of output arguments"), __FILE__, __LINE__);
    }

    const int type = helper.getNewAllowed() ? EXTERNAL_CLASS : EXTERNAL_OBJECT;

    cwd = scigetcwd(&error);
    if (error)
    {
        FREE(cwd);
        cwd = 0;
    }

    for (int i = 0; i < rows * cols; i++)
    {
        try
        {
            ret = env.loadclass(className[i], cwd, named != 0, helper.getAllowReload());
        }
        catch (std::exception & /*e*/)
        {
            FREE(cwd);
            freeAllocatedMatrixOfString(rows, cols, className);
            if (named)
            {
                delete[] name;
            }
            throw;
        }

        if (named)
        {
            try
            {
                ScilabObjects::createNamedEnvironmentObject(type, name[i].c_str(), ret, envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                FREE(cwd);
                freeAllocatedMatrixOfString(rows, cols, className);
                delete[] name;
                throw;
            }
        }
        else
        {
            try
            {
                ScilabObjects::createEnvironmentObjectAtPos(type, Rhs + i + 1, ret, envId, pvApiCtx);
            }
            catch (ScilabAbstractEnvironmentException & /*e*/)
            {
                FREE(cwd);
                freeAllocatedMatrixOfString(rows, cols, className);
                env.removeobject(ret);
                throw;
            }
            LhsVar(i + 1) = Rhs + i + 1;
        }
    }

    FREE(cwd);

    freeAllocatedMatrixOfString(rows, cols, className);
    if (named)
    {
        delete[] name;
        LhsVar(1) = 0;
    }

    PutLhsVar();

    return 0;
}
}
