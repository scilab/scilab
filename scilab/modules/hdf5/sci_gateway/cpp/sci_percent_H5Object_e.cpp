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
int sci_percent_H5Object_e(char * fname, unsigned long fname_len)
{
    H5Object * obj = 0;
    int id;
    SciErr err;
    int * fieldaddr = 0;
    int * mlistaddr = 0;
    char * field = 0;
    int fieldtype;
    double index;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &fieldaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = getVarType(pvApiCtx, fieldaddr, &fieldtype);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (fieldtype != sci_matrix && fieldtype != sci_strings)
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A string or a double expected.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (fieldtype == sci_strings)
    {
        if (getAllocatedSingleString(pvApiCtx, fieldaddr, &field) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (getScalarDouble(pvApiCtx, fieldaddr, &index) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    try
    {
        id = HDF5Scilab::getH5ObjectId(mlistaddr, pvApiCtx);
        obj = H5VariableScope::getVariableFromId(id);

        if (fieldtype == sci_strings)
        {
            obj->getAccessibleAttribute(std::string(field), Rhs + 1, pvApiCtx);
        }
        else
        {
            obj->getAccessibleAttribute(index - 1, Rhs + 1, pvApiCtx);
        }
    }
    catch (std::exception & e)
    {
        if (fieldtype == sci_strings)
        {
            freeAllocatedSingleString(field);
        }
        Scierror(999, _("%s: Error in retrieving field content:\n%s\n"), fname, e.what());

        return 0;
    }

    if (fieldtype == sci_strings)
    {
        freeAllocatedSingleString(field);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
