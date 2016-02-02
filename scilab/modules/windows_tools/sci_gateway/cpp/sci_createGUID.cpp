/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright(C) 2015 - Scilab Enterprises - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "createGUID.h"
}
/*--------------------------------------------------------------------------*/
const std::string fname = "createGUID";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_createGUID(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int rhs = static_cast<int>(in.size());
    if (rhs != 0 && rhs != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname.data(), 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (rhs == 0)
    {
        //return 1 GUID
        out.push_back(new types::String(createGUID()));
        return types::Function::OK;
    }

    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    types::Double* dbl = in[0]->getAs<types::Double>();
    if (dbl->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    int count = static_cast<int>(dbl->get()[0]);
    if (count <= 0)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    types::String* guid = new types::String(count, 1);
    wchar_t** wcs = guid->get();

    for (int i = 0; i < count; ++i)
    {
        wcs[i] = createGUID();
    }

    out.push_back(guid);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
