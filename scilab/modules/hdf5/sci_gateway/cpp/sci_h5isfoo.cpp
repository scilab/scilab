/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
inline static int sci_h5isfoo(const HDF5Scilab::H5ObjectType type, char * fname, unsigned long fname_len)
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

int sci_h5isFile(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5FILE, fname, fname_len);
}
/*--------------------------------------------------------------------------*/

int sci_h5isGroup(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5GROUP, fname, fname_len);
}
/*--------------------------------------------------------------------------*/

int sci_h5isSet(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5DATASET, fname, fname_len);
}
/*--------------------------------------------------------------------------*/

int sci_h5isAttr(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5ATTRIBUTE, fname, fname_len);
}
/*--------------------------------------------------------------------------*/

int sci_h5isSpace(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5SPACE, fname, fname_len);
}
/*--------------------------------------------------------------------------*/

int sci_h5isType(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5TYPE, fname, fname_len);
}
/*--------------------------------------------------------------------------*/

int sci_h5isRef(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5REFERENCE, fname, fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_h5isList(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5LIST, fname, fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_h5isCompound(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5COMPOUND, fname, fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_h5isArray(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5ARRAY, fname, fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_h5isVlen(char * fname, unsigned long fname_len)
{
    return sci_h5isfoo(HDF5Scilab::H5VLEN, fname, fname_len);
}
/*--------------------------------------------------------------------------*/
