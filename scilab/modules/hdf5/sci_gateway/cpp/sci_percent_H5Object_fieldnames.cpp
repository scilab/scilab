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
#include "H5CompoundData.hxx"

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
int sci_percent_H5Object_fieldnames(char * fname, int* pvApiCtx)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    hobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
    if (!hobj)
    {
        Scierror(999, _("%s: Invalid H5Object.\n"), fname);
        return 0;
    }

    if (!hobj->isCompound())
    {
        Scierror(999, _("%s: Not an H5 Compound object.\n"), fname);
        return 0;
    }

    try
    {
        reinterpret_cast<H5CompoundData *>(hobj)->getFieldNames(nbIn + 1, pvApiCtx);
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
