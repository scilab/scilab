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

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*
  Create a new dataset (equivalent to h5write).
  Scilab prototype:
  - h5write(obj, name, data)
  - h5write(obj, name, data, start, count)
  - h5write(obj, name, data, start, count, stride)
  - h5write(obj, name, data, start, count, stride, block)
  - h5write(obj, name, data, targetType)
  - h5write(obj, name, data, targetType, start, count)
  - h5write(obj, name, data, targetType, start, count, stride)
  - h5write(obj, name, data, targetType, start, count, stride, block)
  - h5write(obj, name, data, start, count)
  - h5write(obj, name, data, start, count, stride)
  - h5write(obj, name, data, start, count, stride, block)
  - h5write(obj, name, data, targetType)
  - h5write(obj, name, data, targetType, start, count)
  - h5write(obj, name, data, targetType, start, count, stride)
  - h5write(obj, name, data, targetType, start, count, stride, block)
  - h5write(obj, name, data, targetType, start, count, stride, block, maxdims)
  - h5write(filename, name, data)
  - h5write(filename, name, data, start, count)
  - h5write(filename, name, data, start, count, stride)
  - h5write(filename, name, data, start, count, stride, block)
  - h5write(filename, name, data, targetType)
  - h5write(filename, name, data, targetType, start, count)
  - h5write(filename, name, data, targetType, start, count, stride)
  - h5write(filename, name, data, targetType, start, count, stride, block, maxdims)
*/

int sci_h5write(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    char * str = 0;
    std::string name;
    std::string targetType;
    std::string file;
    double * start = 0;
    double * stride = 0;
    double * count = 0;
    double * block = 0;
    double ** dptrs[4] = {&start, &count, &stride, &block};
    hsize_t * _start = 0;
    hsize_t * _stride = 0;
    hsize_t * _count = 0;
    hsize_t * _block = 0;
    hsize_t ** hptrs[4] = {&_start, &_count, &_stride, &_block};
    int inc = 0;
    int row, col;
    unsigned int size = 0;
    unsigned int rhsBegin = 0;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);

    // passer a 9 pr autoriser le chunking et surtout l'ajouter a la creation du dataset.
    CheckInputArgument(pvApiCtx, 3, 8);

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

        file = std::string(str);
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

    name = std::string(str);
    freeAllocatedSingleString(str);

    if (nbIn >= 4)
    {
        err = getVarAddressFromPosition(pvApiCtx, 4, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (isStringType(pvApiCtx, addr))
        {
            if (!checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 4);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            targetType = std::string(str);
            freeAllocatedSingleString(str);
            rhsBegin = 5;
        }
        else
        {
            rhsBegin = 4;
        }

        for (int i = rhsBegin; i <= nbIn; i++)
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
                err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, dptrs[i - rhsBegin]);
                if (row > 1 && col > 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: Real row or column vector expected.\n"), fname, i);
                    return 0;
                }

                if (row != 0 && col != 0)
                {
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
                    *(dptrs[i - rhsBegin]) = 0;
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

        for (unsigned int i = 0; i < 4; i++)
        {
            *hptrs[i] = HDF5Scilab::flipAndConvert(size, *dptrs[i]);
        }
    }

    try
    {
        if (hobj)
        {
            HDF5Scilab::createObjectFromStack<H5Dataset>(*hobj, "", name, true, pvApiCtx, 3, 0, 0, _start, _stride, _count, _block, targetType, size, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            HDF5Scilab::createObjectFromStack<H5Dataset>(file, "/", name, true, pvApiCtx, 3, 0, 0, _start, _stride, _count, _block, targetType, size, 0, 0, 0, 0, 0, 0);
        }

        for (unsigned int i = 0; i < 4; i++)
        {
            if (hptrs[i])
            {
                delete[] *hptrs[i];
            }
        }
    }
    catch (const H5Exception & e)
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            if (hptrs[i])
            {
                delete[] *hptrs[i];
            }
        }
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
