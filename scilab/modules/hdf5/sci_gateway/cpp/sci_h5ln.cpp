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
#include "expandPathVariable.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*
  Create a link
  Scilab prototype:
  - h5ln(obj, linkname, destobj)
  - h5ln(obj, linkname, destobj, hard)
  - h5ln(obj, linkname, destname)
  - h5ln(obj, linkname, destname, hard)
  - h5ln(obj, linkname, destfile, destname)
  - h5ln(file, location, linkname, destname)
  - h5ln(file, location, linkname, destname, hard)
  - h5ln(file, location, linkname, destfile, destname)

*/

/*--------------------------------------------------------------------------*/
int sci_h5ln(char *fname, int* pvApiCtx)
{
    H5Object * hobj = 0;
    H5Object * targetObj = 0;
    SciErr err;
    int * addr = 0;
    char * str = 0;
    char * expandedPath = 0;
    std::string linkName;
    std::string destName;
    std::string destFile;
    std::string file;
    std::string location;
    int _hard = 0;
    bool hard = false;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 3, 5);

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
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        expandedPath = expandPathVariable(str);
        file = std::string(expandedPath);
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
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    if (hobj)
    {
        linkName = std::string(str);
    }
    else
    {
        location = std::string(str);
    }
    freeAllocatedSingleString(str);

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        targetObj = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!targetObj)
        {
            Scierror(999, _("%s: Can not use H5Object: invalid object.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        if (hobj)
        {
            destName = std::string(str);
        }
        else
        {
            linkName = std::string(str);
        }
        freeAllocatedSingleString(str);
    }

    if (nbIn >= 4)
    {
        err = getVarAddressFromPosition(pvApiCtx, 4, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (!checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (isBooleanType(pvApiCtx, addr))
        {
            if (getScalarBoolean(pvApiCtx, addr, &_hard) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            hard = _hard != 0;
        }
        else if (isStringType(pvApiCtx, addr))
        {
            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            if (hobj)
            {
                destFile = destName;
            }
            destName = std::string(str);
            freeAllocatedSingleString(str);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string or boolean expected.\n"), fname);
            return 0;
        }

        if (nbIn == 5)
        {
            err = getVarAddressFromPosition(pvApiCtx, 5, &addr);
            if (err.iErr)
            {
                printError(&err, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
                return 0;
            }

            if (!checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 5);
                return 0;
            }

            if (isBooleanType(pvApiCtx, addr))
            {
                if (getScalarBoolean(pvApiCtx, addr, &_hard) != 0)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                hard = _hard != 0;
            }
            else if (isStringType(pvApiCtx, addr))
            {
                if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                destFile = destName;
                destName = std::string(str);
                freeAllocatedSingleString(str);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string or boolean expected.\n"), fname);
                return 0;
            }
        }
    }

    try
    {
        if (hobj)
        {
            if (targetObj)
            {
                HDF5Scilab::createLink(*hobj, linkName, *targetObj, hard);
            }
            else
            {
                if (destFile.empty())
                {
                    HDF5Scilab::createLink(*hobj, linkName, destName, hard);
                }
                else
                {
                    HDF5Scilab::createLink(*hobj, linkName, destFile, destName);
                }
            }
        }
        else
        {
            if (destFile.empty())
            {
                HDF5Scilab::createLink(file, location, linkName, destName, hard);
            }
            else
            {
                HDF5Scilab::createLink(file, location, linkName, destFile, destName);
            }
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
