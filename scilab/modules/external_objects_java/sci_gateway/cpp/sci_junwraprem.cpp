/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
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

#include "ScilabJavaEnvironment.hxx"
#include "ScilabGateway.hxx"

extern "C" {
#include "Scierror.h"
#include "gw_external_objects_java.h"
}

using namespace org_scilab_modules_external_objects_java;
using namespace org_modules_external_objects;

int sci_junwraprem(char * fname, void* pvApiCtx)
{
    try
    {
        const int envId = ScilabJavaEnvironment::start();
        return ScilabGateway::unwrapremove(fname, envId, pvApiCtx);
    }
    catch (std::exception & e)
    {
        Scierror(999, "%s: An error occured: %s", fname, e.what());
        return 0;
    }
}
