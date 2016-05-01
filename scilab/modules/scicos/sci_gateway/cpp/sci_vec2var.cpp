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

#include "vec2var.hxx"

#include "types.hxx"
#include "function.hxx"
#include "internal.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

static const std::string vec2varName = "vec2var";

types::Function::ReturnValue sci_vec2var(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }

    if (!in[0]->isDouble())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real row vector expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }
    types::Double* input = in[0]->getAs<types::Double>();

    if (input->getCols() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), vec2varName.c_str(), 1);
        return types::Function::Error;
    }
    if (input->getRows() < 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), vec2varName.c_str(), 1, 2, 1);
        return types::Function::Error;
    }

    std::vector<double> inputVec (input->get(), input->get() + input->getRows());
    types::InternalType* res;
    if (!vec2var(inputVec, res))
    {
        return types::Function::Error;
    }

    out.push_back(res);
    return types::Function::OK;
}
