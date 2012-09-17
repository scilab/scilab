/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "expandPathVariable.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5read(char *fname, unsigned long fname_len)
{
    SciErr err;
    H5Object * hobj = 0;
    int * addr = 0;
    char * path = 0;
    char * expandedPath = 0;
    char * location = 0;
    std::string _expandedPath;
    std::string _location;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        hobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!hobj)
        {
            Scierror(999, _("%s: Can not print H5Object: invalid object.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (Rhs != 2)
        {
            Scierror(999, _("%s: Invalid number of arguments: %d expected.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        expandedPath = expandPathVariable(path);
        _expandedPath = std::string(expandedPath);
        FREE(expandedPath);
        freeAllocatedSingleString(path);
    }

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &location) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        _location = std::string(location);
        freeAllocatedSingleString(location);
    }
    else
    {
        _location = std::string(".");
    }

    try
    {
        if (hobj)
        {
            HDF5Scilab::readData(*hobj, _location, Rhs + 1, pvApiCtx);
        }
        else
        {
            HDF5Scilab::readData(_expandedPath, _location, Rhs + 1, pvApiCtx);
        }
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
