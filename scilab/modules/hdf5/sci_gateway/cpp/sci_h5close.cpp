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

#include "mlist.hxx"
#include "int.hxx"

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

/*
  Close an open object.
  Scilab prototype:
  - h5close()
  - h5close(obj)
*/

/*--------------------------------------------------------------------------*/
int sci_h5close(char *fname, int* pvApiCtx)
{
    int id;
    SciErr err;
    int * addr = 0;
    const int invalid = -1;
    const int nbIn = nbInputArgument(pvApiCtx);

    if (nbIn == 0)
    {
        H5Object::cleanAll();
    }
    else
    {
        for (int i = 1; i <= nbIn; i++)
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
                if (HDF5Scilab::isH5Object(addr, pvApiCtx))
                {
                    id = HDF5Scilab::getH5ObjectId(addr, pvApiCtx);
                    H5VariableScope::removeIdAndDelete(id);

                    /**
                     * Very very crappy workaround to invalidate h5objects...
                     * In this module, args are passed as reference (cf hdf5.start)
                     * so modify mlist contents will have effect on Scilab side.
                     * Why 28 ?? because it is the good value where the _id is located
                     */

                    types::MList* m = (types::MList*)addr;
                    m->set(L"_id", new types::Int32(invalid));
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A H5Object expected.\n"), fname, i);
                    return 0;
                }
            }
            catch (std::exception & /*e*/)
            {
                Scierror(999, _("%s: Cannot remove H5Object.\n"), fname);
                return 0;
            }
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
