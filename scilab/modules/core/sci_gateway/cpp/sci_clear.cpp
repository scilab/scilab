/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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

    if (in.size() == 0)
    {
        //clear();
        symbol::Context::getInstance()->removeAll();
        return Function::OK;
    }

    // First check if all arguments are Single Strings.
    for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
    {
        if ((*inIterator)->isString() == false)
        {
            Scierror(207, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "clear", iWrongType);
            return Function::Error;
        }

        types::String* pStr = (*inIterator)->getAs<types::String>();
        for (int i = 0; i < pStr->getSize(); i++)
        {
            symbol::Context::getInstance()->remove(symbol::Symbol(pStr->get(i)));
        }
    }

    return Function::OK;
}
