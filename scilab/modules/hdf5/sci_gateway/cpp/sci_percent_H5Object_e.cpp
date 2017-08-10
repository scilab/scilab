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

#include "H5Exception.hxx"
#include "HDF5Scilab.hxx"
#include "H5Object.hxx"
#include "H5ReferenceData.hxx"

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
int sci_percent_H5Object_e(char * fname, int* pvApiCtx)
{
    H5Object * obj = 0;
    SciErr err;
    int * addr = 0;
    char * field = 0;
    std::string _field;
    int fieldtype;
    unsigned int * index = 0;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbIn < 2)
    {
        Scierror(999, gettext("%s: Wrong number of input arguments: More than %d expected.\n"), fname, 2);
    }

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = getVarType(pvApiCtx, addr, &fieldtype);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (fieldtype != sci_matrix && fieldtype != sci_strings)
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string or double expected.\n"), fname, 1);
        return 0;
    }

    if (fieldtype == sci_strings)
    {
        if (nbIn >= 3)
        {
            Scierror(999, gettext("%s: Only one field can be requested.\n"), fname);
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &field) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        _field = std::string(field);
        freeAllocatedSingleString(field);
    }
    else
    {
        double d;
        index = new unsigned int[nbIn - 1];

        for (int i = 1; i <= nbIn - 1; i++)
        {
            err = getVarAddressFromPosition(pvApiCtx, i, &addr);
            if (err.iErr)
            {
                delete[] index;
                printError(&err, 0);
                return 0;
            }

            if (getScalarDouble(pvApiCtx, addr, &d) != 0)
            {
                delete[] index;
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            index[i - 1] = (unsigned int)(d - 1);
        }
    }

    err = getVarAddressFromPosition(pvApiCtx, nbIn, &addr);
    if (err.iErr)
    {
        if (index)
        {
            delete[] index;
        }
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, nbIn);
        return 0;
    }

    if (HDF5Scilab::isH5Object(addr, pvApiCtx))
    {
        obj = HDF5Scilab::getH5Object(addr, pvApiCtx);
        if (!obj)
        {
            if (index)
            {
                delete[] index;
            }
            Scierror(999, _("%s: Invalid H5Object.\n"), fname);
            return 0;
        }
    }
    else
    {
        if (index)
        {
            delete[] index;
        }
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A H5Object expected.\n"), fname, nbIn);
        return 0;
    }

    try
    {
        if (fieldtype == sci_strings)
        {
            obj->getAccessibleAttribute(_field, nbIn + 1, pvApiCtx);
        }
        else
        {
            H5Object & robj = obj->getData(nbIn - 1, index);
            //robj.createOnScilabStack(nbIn + 1, pvApiCtx);
            robj.toScilab(pvApiCtx, nbIn + 1, 0, 0, H5Options::isReadFlip());
        }
        /*        else if (obj->isReference())
                {
                    H5ReferenceData * ref = reinterpret_cast<H5ReferenceData *>(obj);
                    H5Object & robj = ref->getReferencesObject(nbIn - 1, index);
                    robj.createOnScilabStack(nbIn + 1, pvApiCtx);
        	    }
                else
                {
                    if (index)
                    {
                        delete[] index;
                    }
                    Scierror(999, gettext("%s: Invalid field.\n"), fname);
                    return 0;
        	    }*/
    }
    catch (std::exception & e)
    {
        if (index)
        {
            delete[] index;
        }
        Scierror(999, _("%s: Error in retrieving field content:\n%s\n"), fname, e.what());
        return 0;
    }

    if (index)
    {
        delete[] index;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
