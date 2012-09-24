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

            if (isDoubleType(pvApiCtx, addr))
            {
                err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, dptrs[0]);
                if (row != 1 && col != 1)
                {
                    Scierror(999, _("%s: Bad dimensions for input argument #%d: a row or a column expected.\n"), fname, 2);
                    return 0;
                }

                if (size == 0)
                {
                    size = row > col ? row : col;
                }
                else if (size != (row > col ? row : col))
                {
                    Scierror(999, _("%s: Bad dimensions for input argument #%d: the same size are expected.\n"), fname, 2);
                    return 0;
                }

                inc = 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A row of doubles expected.\n"), fname, 2);
                return 0;
            }
        }
    }


    for (unsigned int i = 3; i <= nbIn; i++)
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
                Scierror(999, _("%s: Bad dimensions for input argument #%d: a row or a column expected.\n"), fname, i);
                return 0;
            }

            if (size == 0)
            {
                size = row > col ? row : col;
            }
            else if (size != (row > col ? row : col))
            {
                Scierror(999, _("%s: Bad dimensions for input argument #%d: the same size are expected.\n"), fname, i);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A row of doubles expected.\n"), fname, i);
            return 0;
        }
    }

    try
    {
        if (hobj)
        {
            HDF5Scilab::readData(*hobj, _location, size, start, stride, count, block, nbIn + 1, pvApiCtx);
        }
        else
        {
            HDF5Scilab::readData(_expandedPath, _location, size, start, stride, count, block, nbIn + 1, pvApiCtx);
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
