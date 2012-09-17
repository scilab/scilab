/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "H5Exception.hxx"
#include "HDF5Scilab.hxx"
#include "H5Object.hxx"

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "localization.h"
}

using namespace org_modules_hdf5;

/*--------------------------------------------------------------------------*/
int sci_h5close(char *fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int * addr = 0;

    if (Rhs == 0)
    {
        H5Object::cleanAll();
    }
    else
    {
        for (unsigned int i = 1; i <= Rhs; i++)
        {
            err = getVarAddressFromPosition(pvApiCtx, i, &addr);
            if (err.iErr)
            {
                printError(&err, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 0;
            }

            try
            {
                id = HDF5Scilab::getH5ObjectId(addr, pvApiCtx);
                H5VariableScope::removeIdAndDelete(id);
            }
            catch (std::exception & e)
            {
                Scierror(999, _("%s: Cannot remove H5Object.\n"), fname);
                return 0;
            }
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
