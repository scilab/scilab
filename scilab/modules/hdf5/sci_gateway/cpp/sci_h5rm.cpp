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
int sci_h5rm(char *fname, unsigned long fname_len)
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
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
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
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
