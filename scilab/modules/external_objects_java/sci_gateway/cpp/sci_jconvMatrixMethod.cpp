/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabJavaEnvironment.hxx"
#include "ScilabGateway.hxx"

extern "C" {
#include "warningmode.h"
#include "Scierror.h"
#include "gw_external_objects_java.h"
}

using namespace org_scilab_modules_external_objects_java;
using namespace org_modules_external_objects;

int sci_jconvMatrixMethod(char * fname, unsigned long fname_len)
{
    int envId;
    char * type = 0;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);
    if (getWarningMode())
    {
        sciprint(_("%s: Function %s is obsolete.\n"), _("Warning"), fname);
        sciprint(_("%s: Please see documentation for more details.\n"), _("Warning"));
        sciprint(_("%s: This function will be permanently removed in Scilab %s\n\n"), _("Warning"), "6.0.0");
    }

    envId = ScilabJavaEnvironment::start();
    JavaOptionsSetter setter = ScilabJavaEnvironment::getInstance()->getOptionsHelper().getSetter(JavaOptionsSetter::METHODOFCONV);
    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    OptionsHelper::setCopyOccurred(false);
    ScilabObjects::initialization(env, pvApiCtx);
    options.setIsNew(false);

    if (Rhs == 0)
    {
        const char * order = setter.get() ? "rc" : "cr";
        createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &order);

        LhsVar(1) = 1;
        PutLhsVar();

        return 0;
    }

    try
    {
        type = ScilabObjects::getSingleString(Rhs, pvApiCtx);
    }
    catch (const std::exception & e)
    {
        Scierror(999, "%s: String \"rc\" or \"cr\" expected.", fname);
        return 0;
    }

    if (!strcmp(type, "rc"))
    {
        setter.set(true);
    }
    else if (!strcmp(type, "cr"))
    {
        setter.set(false);
    }
    else
    {
        delete type;
        Scierror(999, "%s: Invalid string: \"rc\" or \"cr\" expected.", fname);
        return 0;
    }

    delete type;

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
