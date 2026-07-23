/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

extern "C"
{
#include "callCommand.h"
}

#include "overload.hxx"
#include "string.hxx"

#include <string>

int callCommandFromGateway(const char *command)
{
    // will act as execstr
    types::typed_list in = types::typed_list{new types::String(command)};
    types::typed_list out = types::typed_list();
    types::Callable::ReturnValue ret = Overload::call(L"execstr", in, 0, out, false);
    if (ret == types::Function::OK)
    {
        return 0;
    }
    return 1;
}
