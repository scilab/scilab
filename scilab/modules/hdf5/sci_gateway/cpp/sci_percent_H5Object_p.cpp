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
int sci_percent_H5Object_p(char * fname, int* pvApiCtx)
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
