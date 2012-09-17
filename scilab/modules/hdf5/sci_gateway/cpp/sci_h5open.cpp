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
#include "sciprint.h"
#include "expandPathVariable.h"
}

#include "H5File.hxx"

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5open(char *fname, unsigned long fname_len)
{
    H5File * h5file = 0;
    SciErr err;
    int * addr = 0;
    char * path = 0;
    const char * access = 0;
    const char * name = 0;
    char * args[3];

    CheckLhs(1, 1);
    CheckRhs(1, 3);

    for (int i = 1; i <= Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            for (int j = 0; j < i; j++)
            {
                freeAllocatedSingleString(args[j]);
            }
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            for (int j = 0; j < i; j++)
            {
                freeAllocatedSingleString(args[j]);
            }
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, args + i - 1) != 0)
        {
            for (int j = 0; j < i; j++)
            {
                freeAllocatedSingleString(args[j]);
            }
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    path = expandPathVariable(args[0]);

    switch (Rhs)
    {
        case 1:
            access = "a";
            name = "/";
            break;
        case 2:
            if (H5File::checkFileAccess(args[1]))
            {
                access = args[1];
                name = "/";
            }
            else
            {
                access = "a";
                name = args[1];
            }
            break;
        case 3:
            if (H5File::checkFileAccess(args[1]))
            {
                access = args[1];
                name = args[2];
            }
            else if (H5File::checkFileAccess(args[2]))
            {
                name = args[1];
                access = args[2];
            }
            else
            {
                name = args[1];
                access = "a";
            }
    }

    try
    {
        h5file = new H5File(path, name, access);
        for (int i = 0; i < Rhs; i++)
        {
            freeAllocatedSingleString(args[i]);
        }
        FREE(path);
    }
    catch (const H5Exception & e)
    {
        for (int i = 0; i < Rhs; i++)
        {
            freeAllocatedSingleString(args[i]);
        }
        FREE(path);
        Scierror(999, _("%s: %s\n"), fname, e.what());
        return 0;
    }

    try
    {
        h5file->createOnScilabStack(Rhs + 1, pvApiCtx);
    }
    catch (const H5Exception & e)
    {
        Scierror(999, _("%s: %s\n"), fname, e.what());
        delete h5file;
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
