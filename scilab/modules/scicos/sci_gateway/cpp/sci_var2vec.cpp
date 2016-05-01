/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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

#include <vector>
#include <string>
#include <cwchar>
#include <cstring>

#include "gw_scicos.hxx"

#include "var2vec.hxx"

#include "types.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

static const std::string var2vecName = "var2vec";

types::Function::ReturnValue sci_var2vec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), var2vecName.c_str(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), var2vecName.c_str(), 1);
        return types::Function::Error;
    }

    // Declare the returned column vector
    std::vector<double> retData;
    if (!var2vec(in[0], retData))
    {
        return types::Function::Error;
    }
    // TODO: avoid retDdata copy by adding a move constructor from std::vector<double> to types::Double*
    types::Double* ret = new types::Double(static_cast<int>(retData.size()), 1);
    memcpy(ret->get(), &retData[0], retData.size() * sizeof(double));

    out.push_back(ret);
    return types::Function::OK;
}
