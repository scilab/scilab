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

extern "C" {
#include "scicurdir.h"
#include "os_string.h"
}

#include "function.hxx"
#include "string.hxx"
#include "context.hxx"
#include "ScilabGateway.hxx"

namespace org_modules_external_objects
{

//no need to delete, just a pointer on existing varaible
static types::Callable* pCall = nullptr;

int ScilabGateway::deff(char * fname, const int envId, void * pvApiCtx)
{
    static int ONE = 1;
    static int TWO = 2;
    static int THREE = 3;

    SciErr err;
    char ** names[] = {0, 0, 0};
    int ret = 0;
    std::ostringstream os;
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

    if (pCall == nullptr)
    {
        symbol::Context* ctx = symbol::Context::getInstance();
        types::InternalType* pIT = ctx->get(symbol::Symbol(L"#_deff_wrapper"));
        if (pIT && pIT->isCallable())
        {
            pCall = pIT->getAs<types::Callable>();
        }
        else
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
    }

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
    catch (std::exception & /*e*/)
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
        //call #_deff_wrapper
        types::typed_list in, out;
        types::optional_list opt;

        //name
        in.push_back(new types::String(names[2][i]));


        //protopype
        os.str("");
        os << "y=" << names[2][i] << "(varargin)" << std::flush;
        in.push_back(new types::String(os.str().c_str()));

        //body
        os.str("");
        os << "y=invoke_lu(int32(" << ret << "),int32(" << envId << "),\"" << names[1][i] << "\",varargin)" << std::flush;
        in.push_back(new types::String(os.str().c_str()));

        if (pCall->call(in, opt, 0, out) != types::Function::OK)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot create the data"));
        }
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
