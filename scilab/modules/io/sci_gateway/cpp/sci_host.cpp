/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
        Scierror(89, _("%s: Wrong size for input argument #%d: string expected.\n"), "host", 1);
        return types::Function::Error;
    }

    wchar_t* pstCommand = pIT->getAs<types::String>()->get(0);

    int stat = 0;
    systemcW(pstCommand, &stat);

    out.push_back(new types::Double(stat));
    return types::Function::OK;
}
