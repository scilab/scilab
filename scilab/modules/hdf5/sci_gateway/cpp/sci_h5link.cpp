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

#include <string>

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "HDF5Scilab.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5link(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    H5Object * target = 0;
    SciErr err;
    int * addr = 0;
    char * name = 0;
    char * targetName = 0;
    char * targetFile = 0;
    std::string _name;
    std::string _targetName;
    std::string _targetFile;
    int _hard = 0;
    bool hard = false;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 3, 4);

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
        Scierror(999, _("%s: Wrong type for input argument #%d: A H5Object expected.\n"), fname, 1);
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

    err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        target = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!target)
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

        if (getAllocatedSingleString(pvApiCtx, addr, &targetName) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        _targetName = std::string(targetName);
        freeAllocatedSingleString(targetName);
    }

    if (nbIn == 4)
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
        else if (isStringType(pvApiCtx, addr) && !_targetName.empty())
        {
            if (getAllocatedSingleString(pvApiCtx, addr, &targetFile) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            _targetFile = std::string(targetFile);
            freeAllocatedSingleString(targetFile);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string or boolean expected.\n"), fname);
            return 0;
        }
    }

    try
    {
        if (target)
        {
            HDF5Scilab::createLink(*hobj, _name, *target, hard);
        }
        else
        {
            if (_targetFile.empty())
            {
                HDF5Scilab::createLink(*hobj, _name, _targetName, hard);
            }
            else
            {
                HDF5Scilab::createLink(*hobj, _name, _targetName, _targetFile);
            }
        }
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
