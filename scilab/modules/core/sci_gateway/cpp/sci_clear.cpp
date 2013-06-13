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

#include "core_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_clear(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::iterator inIterator;
    int iWrongType = 1;

    // First check if all arguments are Single Strings.
    for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
    {
        if (!(*inIterator)->isString())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "clear", iWrongType);
            return Function::Error;
        }
        if ((*inIterator)->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Single string expected.\n"), "clear", iWrongType);
            return Function::Error;
        }
        iWrongType++;
    }

    for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
    {
        symbol::Context::getInstance()->remove(symbol::Symbol((*inIterator)->getAs<types::String>()->get(0, 0)));
    }

    return Function::OK;
}
