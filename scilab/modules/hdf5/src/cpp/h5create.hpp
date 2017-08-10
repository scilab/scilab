/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
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
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

template <typename U>
int h5create(char * fname, int* pvApiCtx)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    char * name = 0;
    char * targetType = 0;
    std::string _name;
    std::string _targetType("");
    double * start = 0;
    double * stride = 0;
    double * count = 0;
    double * block = 0;
    double ** dptrs[4] = {&start, &count, &stride, &block};
    int inc = 0;
    int row, col;
    unsigned int size = 0;
    unsigned int rhsBegin;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
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
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: a H5Object expected.\n"), fname, 1);
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
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &name) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    _name = std::string(name);
    freeAllocatedSingleString(name);

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
		Scierror(999, _("%s: Wrong size for input argument #%d: A String expected.\n"), fname, 4);
	    }

	    if (getAllocatedSingleString(pvApiCtx, addr, &targetType) != 0)
	    {
		Scierror(999, _("%s: No more memory.\n"), fname);
		return 0;
	    }
	    _targetType = std::string(targetType);
	    freeAllocatedSingleString(targetType);
	    rhsBegin = 5;
        }
	else
	{
	    rhsBegin = 4;
	}
    }

    for (unsigned int i = rhsBegin; i <= nbIn; i++)
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

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    try
    {
	U & newobj = HDF5Scilab::createObjectFromStack<U>(*hobj, _name, _targetType, size, start, stride, count, block, pvApiCtx, addr, 3);
        newobj.createOnScilabStack(nbIn + 1, pvApiCtx);
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

/*--------------------------------------------------------------------------*/
