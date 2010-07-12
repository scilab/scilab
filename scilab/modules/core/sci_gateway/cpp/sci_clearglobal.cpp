/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"

Function::ReturnValue sci_clearglobal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::iterator inIterator;
    int iWrongType = 1;

    if(in.size() == 0)
    {
        symbol::Context::getInstance()->removeGlobalAll();
    }
    else
    {
        // First check if all arguments are Single Strings.
        for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
        {
            if(!(*inIterator)->isString() || (*inIterator)->getAsString()->size_get() != 1)
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"), "clearglobal", iWrongType);
                return Function::Error;
            }
            iWrongType++;
        }

        for (inIterator = in.begin() ; inIterator != in.end() ; iWrongType++, inIterator++)
        {
            symbol::Context::getInstance()->removeGlobal((*inIterator)->getAsString()->string_get(0,0));
        }
    }
    return Function::OK;
}
