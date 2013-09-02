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

extern "C" {
#include "scicurdir.h"
}

namespace org_modules_external_objects
{

int ScilabGateway::deff(char * fname, const int envId, void * pvApiCtx)
{
    static int ONE = 1;
    static int TWO = 2;
    static int THREE = 3;

    SciErr err;
    char ** names[] = {0, 0, 0};
    int ret = 0;
    std::ostringstream os;
    char * str;
    int * addr[] = {0, 0, 0};
    int rows[] = {0, 0, 0};
    int cols[] = {0, 0, 0};
    int error = 0;
    char * cwd = 0;

    CheckInputArgument(pvApiCtx, 3, 3);

    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper & helper = env.getOptionsHelper();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    for (int i = 0; i < 3; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &(addr[i]));
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (!isStringType(pvApiCtx, addr[i]))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Wrong type for input argument #%d: A String expected."), 1);
        }

        err = getVarDimension(pvApiCtx, addr[i], &(rows[i]), &(cols[i]));
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

    if (rows[0] != 1 || cols[0] != 1)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimensions for input argument #%d: A single string expected."), 1);
    }

    if (rows[1] != rows[2] || cols[1] != cols[2])
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimensions: arguments #2 and #3 must have the same."));
    }

    for (int i = 0; i < 3; i++)
    {
        if (getAllocatedMatrixOfString(pvApiCtx, addr[i], &(rows[i]), &(cols[i]), &(names[i])))
        {
            for (int j = 0; j < i; j++)
            {
                freeAllocatedMatrixOfString(rows[j], cols[j], names[j]);
            }
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

    cwd = scigetcwd(&error);
    if (error)
    {
        FREE(cwd);
        cwd = 0;
    }

    try
    {
        ret = env.loadclass(names[0][0], cwd, false, helper.getAllowReload());
    }
    catch (std::exception & e)
    {
        FREE(cwd);
        for (int j = 0; j < 3; j++)
        {
            freeAllocatedMatrixOfString(rows[j], cols[j], names[j]);
        }
        throw;
    }
    FREE(cwd);

    for (int i = 0; i < rows[1] * cols[1]; i++)
    {
        err = createMatrixOfString(pvApiCtx, ONE, 1, 1, (const char * const *) & (names[2][i]));
        if (err.iErr)
        {
            for (int j = 0; j < 3; j++)
            {
                freeAllocatedMatrixOfString(rows[j], cols[j], names[j]);
            }
            env.removeobject(ret);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot create the data"));
        }

        os.str("");
        os << "y=" << names[2][i] << "(varargin)" << std::flush;
        str = strdup(os.str().c_str());

        err = createMatrixOfString(pvApiCtx, TWO, 1, 1, (const char * const *)&str);
        free(str);
        if (err.iErr)
        {
            for (int j = 0; j < 3; j++)
            {
                freeAllocatedMatrixOfString(rows[j], cols[j], names[j]);
            }
            env.removeobject(ret);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot create the data"));
        }

        os.str("");
        os << "y=invoke_lu(int32(" << ret << "),int32(" << envId << "),\"" << names[1][i] << "\",varargin)" << std::flush;
        str = strdup(os.str().c_str());

        err = createMatrixOfString(pvApiCtx, THREE, 1, 1, (const char * const *)&str);
        free(str);
        if (err.iErr)
        {
            for (int j = 0; j < 3; j++)
            {
                freeAllocatedMatrixOfString(rows[j], cols[j], names[j]);
            }
            env.removeobject(ret);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot create the data"));
        }

        SciString(&ONE, const_cast<char *>("!_deff_wrapper"), &ONE, &THREE);
    }

    for (int i = 0; i < 3; i++)
    {
        freeAllocatedMatrixOfString(rows[0], cols[0], names[i]);
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
}
