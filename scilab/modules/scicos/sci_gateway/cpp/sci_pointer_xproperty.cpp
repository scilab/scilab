/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) INRIA - Allan CORNET
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

#include <string>
#include <algorithm>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "types.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "scicos.h" /* get_pointer_xproperty() and get_npointer_xproperty() */
#include "scicos-def.h"

#include "localization.h"
#include "Scierror.h"
    /*--------------------------------------------------------------------------*/
    // Variable defined in sci_scicosim.cpp
    extern COSIM_struct C2F(cosim);
    /*--------------------------------------------------------------------------*/
}

static double toDouble(const int a)
{
    return static_cast<double>(a);
}

static const std::string funname = "pointer_xproperty";

types::Function::ReturnValue sci_pointer_xproperty(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 0);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    const int isrun = C2F(cosim).isrun;
    if (!isrun)
    {
        Scierror(999, _("%s: scicosim is not running.\n"), funname.data());
        return types::Function::Error;
    }

    // Retrieve the current block's continuous state and copy it to the return
    const int* pointer_xproperty = get_pointer_xproperty();
    const int  npointer_xproperty = get_npointer_xproperty();

    double* data;
    types::Double* ret = new types::Double(npointer_xproperty, 1, &data);
#ifdef _MSC_VER
    std::transform(pointer_xproperty, pointer_xproperty + npointer_xproperty, stdext::checked_array_iterator<double*>(data, npointer_xproperty), toDouble);
#else
    std::transform(pointer_xproperty, pointer_xproperty + npointer_xproperty, data, toDouble);
#endif

    out.push_back(ret);
    return types::Function::OK;
}
