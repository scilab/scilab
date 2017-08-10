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

#include <set>
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
inline static int sci_h5isfoo(const HDF5Scilab::H5ObjectType type, char * fname, int* pvApiCtx)
{
    H5Object * hobj = 0;
    SciErr err;
    int * addr = 0;
    bool ok = false;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 1, 1);

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
            goto finish;
        }
    }
    else
    {
        goto finish;
    }

    try
    {
        ok = HDF5Scilab::checkType(*hobj, type);
    }
    catch (const std::exception & /*e*/)
    {

    }

finish:

    if (createScalarBoolean(pvApiCtx, nbIn + 1, ok ? 1 : 0))
    {
        Scierror(999, _("%s: Can not create output argument.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/

int sci_h5isFile(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5FILE, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/

int sci_h5isGroup(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5GROUP, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/

int sci_h5isSet(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5DATASET, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/

int sci_h5isAttr(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5ATTRIBUTE, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/

int sci_h5isSpace(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5SPACE, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/

int sci_h5isType(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5TYPE, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/

int sci_h5isRef(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5REFERENCE, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/
int sci_h5isList(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5LIST, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/
int sci_h5isCompound(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5COMPOUND, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/
int sci_h5isArray(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5ARRAY, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/
int sci_h5isVlen(char * fname, int* pvApiCtx)
{
    return sci_h5isfoo(HDF5Scilab::H5VLEN, fname, pvApiCtx);
}
/*--------------------------------------------------------------------------*/
