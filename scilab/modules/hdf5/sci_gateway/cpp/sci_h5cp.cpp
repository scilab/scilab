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
#include "H5File.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5cp(char *fname, unsigned long fname_len)
{
    SciErr err;
    H5Object * sobj = 0;
    H5Object * dobj = 0;
    int * addr = 0;
    char * str = 0;
    char * expandedPath = 0;
    std::string sfile;
    std::string dfile;
    std::string sloc;
    std::string dloc("");

    CheckLhs(1, 1);
    CheckRhs(2, 4);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        sobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!sobj)
        {
            Scierror(999, _("%s: Invalid H5Object.\n"), fname);
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

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        expandedPath = expandPathVariable(str);
        freeAllocatedSingleString(str);
        sfile = std::string(expandedPath);
        FREE(expandedPath);
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        dobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!dobj)
        {
            Scierror(999, _("%s: Invalid H5Object.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        if (Rhs == 3)
        {
            expandedPath = expandPathVariable(str);
            dfile = std::string(expandedPath);
            FREE(expandedPath);
        }
        else
        {
            sloc = std::string(str);
        }
        freeAllocatedSingleString(str);
    }

    if (Rhs >= 3)
    {
        err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (HDF5Scilab::isH5Object(addr, pvApiCtx))
        {
            dobj = HDF5Scilab::getH5Object(addr, pvApiCtx);
            if (!dobj)
            {
                Scierror(999, _("%s: Invalid H5Object.\n"), fname);
                return 0;
            }
        }
        else
        {
            if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            if (Rhs == 4)
            {
                expandedPath = expandPathVariable(str);
                dfile = std::string(expandedPath);
                FREE(expandedPath);
            }
            else
            {
                dloc = std::string(str);
            }
            freeAllocatedSingleString(str);
        }

        if (Rhs == 4)
        {
            err = getVarAddressFromPosition(pvApiCtx, 4, &addr);
            if (err.iErr)
            {
                printError(&err, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                return 0;
            }

            if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            dloc = std::string(str);
            freeAllocatedSingleString(str);
        }
    }

    try
    {
        if (sobj)
        {
            if (dobj)
            {
                HDF5Scilab::copy(*sobj, *dobj, dloc);
            }
            else
            {
                HDF5Scilab::copy(*sobj, dfile, dloc);
            }
        }
        else
        {
            if (dobj)
            {
                HDF5Scilab::copy(sfile, sloc, *dobj, dloc);
            }
            else
            {
                HDF5Scilab::copy(sfile, sloc, dfile, dloc);
            }
        }
    }
    catch (const std::exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
