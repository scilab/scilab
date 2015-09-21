/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include <string.h>
#include "io_gw.hxx"
#include "string.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "systemc.h"
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_host(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "host", 1);
        return types::Function::Error;
    }

    types::InternalType* pIT = in[0];

    if (pIT->isString() == false || pIT->getAs<types::String>()->getSize() != 1)
    {
        Scierror(89, _("%s: Wrong size for input argument #%d: A string expected.\n"), "host", 1);
        return types::Function::Error;
    }

    wchar_t* pstCommand = pIT->getAs<types::String>()->get(0);

    int stat = 0;
    systemcW(pstCommand, &stat);

    out.push_back(new types::Double(stat));
    return types::Function::OK;
}
