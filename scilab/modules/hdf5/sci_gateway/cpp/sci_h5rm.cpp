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

#include <string>

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "sciprint.h"
#include "expandPathVariable.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*
  Remove an object.
  Scilab prototype:
  - h5rm(obj)
  - h5rm(obj, name)
  - h5rm(filename, name)
*/

/*--------------------------------------------------------------------------*/
int sci_h5rm(char *fname, int* pvApiCtx)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    char * str = 0;
    char ** strs = 0;
    int row, col;
    std::string file;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 1, 2);

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
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string or H5Object expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        file = std::string(str);
        freeAllocatedSingleString(str);

        if (nbIn == 1)
        {
            Scierror(999, gettext("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 0;
        }
    }

    if (nbIn == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &strs) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    try
    {
        if (hobj)
        {
            if (strs)
            {
                HDF5Scilab::deleteObject(*hobj, row * col, const_cast<const char **>(strs));
            }
            else
            {
                HDF5Scilab::deleteObject(*hobj, std::string(""));
                H5VariableScope::removeIdAndDelete(hobj->getScilabId());
            }
        }
        else
        {
            HDF5Scilab::deleteObject(file, row * col, const_cast<const char **>(strs));
        }
    }
    catch (const std::exception & e)
    {
        if (strs)
        {
            freeAllocatedMatrixOfString(row, col, strs);
        }

        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    if (strs)
    {
        freeAllocatedMatrixOfString(row, col, strs);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
