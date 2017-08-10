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
#include "sciprint.h"
#include "expandPathVariable.h"
}

#include "H5File.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5open(char *fname, int* pvApiCtx)
{
    H5File * h5file = 0;
    SciErr err;
    int * addr = 0;
    char * str = 0;
    char * expandedPath = 0;
    std::string path;
    std::string access = "a";
    std::string driver;
    int _backingStore;
    bool backingStore = true;
    double _size;
    hsize_t memberSize = (unsigned int) - 1;
    size_t blockSize = 64 * 1024;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 1, 5);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    expandedPath = expandPathVariable(str);
    path = std::string(expandedPath);
    FREE(expandedPath);
    freeAllocatedSingleString(str);

    if (nbIn >= 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        if (!H5File::checkFileAccess(str))
        {
            Scierror(999, _("%s: Invalid access mode: %s.\n"), fname, str);
            freeAllocatedSingleString(str);
            return 0;
        }

        access = std::string(str);
        freeAllocatedSingleString(str);

        if (nbIn >= 3)
        {
            err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
            if (err.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            driver = std::string(str);
            freeAllocatedSingleString(str);
            std::transform(driver.begin(), driver.end(), driver.begin(), tolower);

            if (((driver == "sec2" || driver == "stdio") && nbIn >= 4) || (driver == "family" && nbIn == 5))
            {
                Scierror(999, _("%s: Invalid number of input arguments.\n"), fname);
                return 0;
            }

            if (nbIn >= 4)
            {
                err = getVarAddressFromPosition(pvApiCtx, 4, &addr);
                if (err.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                    return 0;
                }

                if (driver == "core")
                {
                    if (!isBooleanType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
                    {
                        Scierror(999, gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 4);
                        return 0;
                    }

                    if (getScalarBoolean(pvApiCtx, addr, &_backingStore))
                    {
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }

                    backingStore = _backingStore != 0;
                }
                else // driver is necessarly 'family'
                {
                    if (!isDoubleType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
                    {
                        Scierror(999, gettext("%s: Wrong type for input argument #%d: A double expected.\n"), fname, 4);
                        return 0;
                    }

                    if (getScalarDouble(pvApiCtx, addr, &_size))
                    {
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }

                    memberSize = _size > 0 ? (hsize_t)_size : memberSize;
                }

                if (nbIn == 5)
                {
                    err = getVarAddressFromPosition(pvApiCtx, 5, &addr);
                    if (err.iErr)
                    {
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
                        return 0;
                    }

                    if (!isDoubleType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
                    {
                        Scierror(999, gettext("%s: Wrong type for input argument #%d: A double expected.\n"), fname, 5);
                        return 0;
                    }

                    if (getScalarDouble(pvApiCtx, addr, &_size))
                    {
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }

                    blockSize = _size > 0 ? (size_t)_size : blockSize;
                }
            }
        }
    }

    try
    {
        if (driver.empty() || driver == "sec2" || driver == "stdio")
        {
            h5file = new H5File(path, "/", access, driver);
        }
        else if (driver == "core")
        {
            h5file = new H5File(path, "/", access, backingStore, blockSize);
        }
        else
        {
            h5file = new H5File(path, "/", access, memberSize);
        }
    }
    catch (const std::exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    try
    {
        h5file->createOnScilabStack(nbIn + 1, pvApiCtx);
    }
    catch (const std::exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        delete h5file;
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
