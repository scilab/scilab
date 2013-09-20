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

#include "H5Exception.hxx"
#include "HDF5Scilab.hxx"
#include "H5Object.hxx"

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "localization.h"
}

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_percent_H5Object_p(char * fname, unsigned long fname_len)
{
    H5Object * obj = 0;
    int id;
    SciErr err;
    int * mlistaddr = 0;

    CheckInputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    try
    {
        id = HDF5Scilab::getH5ObjectId(mlistaddr, pvApiCtx);
        obj = H5VariableScope::getVariableFromId(id);
    }
    catch (std::exception & /*e*/)
    {
        Scierror(999, _("%s: Can not print H5Object.\n"), fname);
        return 0;
    }

    if (obj)
    {
        const std::string str = obj->toString();
        sciprint("%s\n", str.c_str());
    }
    else
    {
        Scierror(999, _("%s: Can not print H5Object: invalid object.\n"), fname);
    }

    return 0;
}
