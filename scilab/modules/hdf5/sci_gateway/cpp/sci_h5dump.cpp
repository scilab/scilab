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

#include <set>
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
#include "H5File.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5dump(char *fname, unsigned long fname_len)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    char * path = 0;
    char * expandedPath = 0;
    char * name = 0;
    std::map<haddr_t, std::string> visited;
    bool mustDelete = true;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 1, 2);

    if (nbIn == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &name) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

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

        if (nbIn == 2)
        {
            try
            {
                hobj = &H5Object::getObject(*hobj, name);
            }
            catch (const std::exception & e)
            {
                Scierror(999, _("%s: %s\n"), fname, e.what());
                return 0;
            }
        }
        else
        {
            mustDelete = false;
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
        freeAllocatedSingleString(path);
        try
        {
            if (nbIn == 2)
            {
                hobj = new H5File((const char *)expandedPath, (const char *)name, "r");
            }
            else
            {
                hobj = new H5File((const char *)expandedPath, "/", "r");
            }
            FREE(expandedPath);
        }
        catch (const std::exception & e)
        {
            Scierror(999, _("%s: %s\n"), fname, e.what());
            FREE(expandedPath);
            return 0;
        }
    }

    try
    {
        HDF5Scilab::scilabPrint(hobj->dump(visited));
        if (mustDelete)
        {
            delete hobj;
        }
    }
    catch (const std::exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        if (mustDelete)
        {
            delete hobj;
        }
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
