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

/*
  Read the content of a dataset.
  Scilab prototype:
  - h5read(obj) (obj must be a dataset)
  - h5read(obj, location)
  - h5read(obj, location, start, count)
  - h5read(obj, location, start, count, stride)
  - h5read(obj, location, start, count, stride, block)
  - h5read(obj, start, count)
  - h5read(obj, start, count, stride)
  - h5read(obj, start, count, stride, block)
  - h5read(filename, location)
  - h5read(filename, location, start, count)
  - h5read(filename, location, start, count, stride)
  - h5read(filename, location, start, count, stride, block)
*/

/*--------------------------------------------------------------------------*/
int sci_h5read(char *fname, int* pvApiCtx)
{
    SciErr err;
    H5Object * hobj = 0;
    int * addr = 0;
    char * expandedPath = 0;
    char * str = 0;
    std::string _expandedPath;
    std::string location;
    double * start = 0;
    double * stride = 0;
    double * count = 0;
    double * block = 0;
    double ** dptrs[4] = {&start, &count, &stride, &block};
    int inc = 0;
    int row, col;
    unsigned int size = 0;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 1, 6);

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
        if (nbIn == 1)
        {
            Scierror(999, _("%s: Invalid number of input argument.\n"), fname);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
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

        if (_expandedPath.empty())
        {
            Scierror(999, _("%s: Invalid path: cannot be empty.\n"), fname);
            return 0;
        }
    }

    if (nbIn >= 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isStringType(pvApiCtx, addr))
        {
            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            location = std::string(str);
            freeAllocatedSingleString(str);
        }
        else
        {
            if (!_expandedPath.empty())
            {
                Scierror(999, _("%s: Second argument must be a dataset name.\n"), fname);
                return 0;
            }

            if (nbIn == 2)
            {
                Scierror(999, _("%s: Invalid number of input argument.\n"), fname);
                return 0;
            }

            location = std::string(".");

            if (isDoubleType(pvApiCtx, addr))
            {
                err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, dptrs[0]);
                if (row != 1 && col != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: Real row or column vector expected.\n"), fname, 2);
                    return 0;
                }

                if (size == 0)
                {
                    size = row > col ? row : col;
                }
                else if (size != (row > col ? row : col))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: Same size as the data expected.\n"), fname, 2);
                    return 0;
                }

                inc = 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector expected.\n"), fname, 2);
                return 0;
            }
        }
    }

    for (int i = 3; i <= nbIn; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }
        if (isDoubleType(pvApiCtx, addr))
        {
            err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, dptrs[i - 3 + inc]);
            if (row != 1 && col != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Real row or column vector expected.\n"), fname, i);
                return 0;
            }

            if (size == 0)
            {
                size = row > col ? row : col;
            }
            else if (size != (row > col ? row : col))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Same size as the data expected.\n"), fname, i);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector expected.\n"), fname, i);
            return 0;
        }
    }

    if (start && !count)
    {
        Scierror(999, _("%s: Argument 'count' is missing.\n"), fname);
        return 0;
    }

    try
    {
        if (hobj)
        {
            HDF5Scilab::readData(*hobj, location, size, start, stride, count, block, nbIn + 1, pvApiCtx);
        }
        else
        {
            HDF5Scilab::readData(_expandedPath, location, size, start, stride, count, block, nbIn + 1, pvApiCtx);
        }
    }
    catch (const std::exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
