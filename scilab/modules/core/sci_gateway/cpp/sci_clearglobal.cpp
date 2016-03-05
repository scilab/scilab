/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

types::Function::ReturnValue sci_clearglobal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::iterator inIterator;
    int iWrongType = 1;

    if (in.size() == 0)
    {
        symbol::Context::getInstance()->removeGlobalAll();
    }
    else
    {
        // First check if all arguments are Single Strings.
        for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
        {
            if (!(*inIterator)->isString() || (*inIterator)->getAs<types::String>()->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "clearglobal", iWrongType);
                return types::Function::Error;
            }
            iWrongType++;
        }

        for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
        {
            symbol::Context::getInstance()->removeGlobal(symbol::Symbol((*inIterator)->getAs<types::String>()->get(0, 0)));
        }
    }
    return types::Function::OK;
}
