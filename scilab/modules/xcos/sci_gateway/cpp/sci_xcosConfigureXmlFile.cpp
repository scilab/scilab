/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "Modelica.hxx"
#include "GiwsException.hxx"
#include "xcosUtilities.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos_modelica;

int sci_xcosConfigureXmlFile(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    char *init = NULL;
    char *relations = NULL;

    /* first file setup */
    if (readSingleString(pvApiCtx, 1, &init, fname))
    {
        return 0;
    }
    /* second file setup */
    if (readSingleString(pvApiCtx, 2, &relations, fname))
    {
        FREE(init);
        return 0;
    }

    /* Call the java implementation */
    try
    {
        Modelica::load(getScilabJavaVM(), init, relations);

        FREE(init);
        FREE(relations);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());

        FREE(init);
        FREE(relations);
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());

        FREE(init);
        FREE(relations);
        return 0;
    }

    PutLhsVar();
    return 0;
}
