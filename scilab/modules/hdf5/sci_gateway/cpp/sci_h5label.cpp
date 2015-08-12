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

#include <string>

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "expandPathVariable.h"
}

#include "H5Group.hxx"
#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*
  Create a label on a dataset
  Scilab prototype:
  - h5label(obj, location, dims, names)
  - h5label(filename, location, dims, names)
*/

/*--------------------------------------------------------------------------*/
int sci_h5label(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    char * str = 0;
    char ** strs = 0;
    char * expandedPath = 0;
    std::string _expandedPath;
    std::string location;
    double * dims = 0;
    unsigned int * _dims = 0;
    unsigned int size;
    const int nbIn = nbInputArgument(pvApiCtx);
    int row, col;

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 4, 4);

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
            Scierror(999, _("%s: Invalid H5Object.\n"), fname);
            return 0;
        }
    }
    else
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string or a H5Object expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        expandedPath = expandPathVariable(str);
        _expandedPath = std::string(expandedPath);
        FREE(expandedPath);
        freeAllocatedSingleString(str);
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    location = std::string(str);
    freeAllocatedSingleString(str);

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, addr))
    {
        printError(&err, 0);
        Scierror(999, gettext("%s: Wrong type for input argument #%d: Real row vector expected.\n"), fname, 3);
        return 0;
    }

    err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &dims);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (row != 1 && col != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Real row or column vector expected.\n"), fname, 3);
        return 0;
    }

    size = (unsigned int)(row == 1 ? col : row);
    _dims = new unsigned int[size];

    for (unsigned int i = 0; i < size; i++)
    {
        _dims[i] = (unsigned int)dims[i] - 1;
    }

    err = getVarAddressFromPosition(pvApiCtx, 4, &addr);
    if (err.iErr)
    {
        delete[] _dims;
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        delete[] _dims;
        Scierror(999, gettext("%s: Wrong type for input argument #%d: Row array of strings expected.\n"), fname, 4);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &strs) != 0)
    {
        delete[] _dims;
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    if ((row != 1 && col != 1) || (row == 1 && col != size) || (col == 1 && row != size))
    {
        delete[] _dims;
        freeAllocatedMatrixOfString(row, col, strs);
        Scierror(999, _("%s: Wrong size for input argument #%d: A row or column vector expected.\n"), fname, 4);
        return 0;
    }

    try
    {
        if (hobj)
        {
            HDF5Scilab::label(*hobj, location, size, _dims, const_cast<const char **>(strs));
        }
        else
        {
            HDF5Scilab::label(_expandedPath, location, size, _dims, const_cast<const char **>(strs));
        }
        delete[] _dims;
        freeAllocatedMatrixOfString(row, col, strs);
    }
    catch (const std::exception & e)
    {
        delete[] _dims;
        freeAllocatedMatrixOfString(row, col, strs);
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
