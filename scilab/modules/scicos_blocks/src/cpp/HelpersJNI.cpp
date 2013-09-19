/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include "Helpers.hxx"

#include "AfficheBlock.hxx"
#include "GiwsException.hxx"

extern "C" {
#include "getScilabJavaVM.h"
#include "scicos.h"
}

namespace org_scilab_modules_xcos_block
{

int AfficheBlock_setValue(char const* uid, char const* const* const* value, int valueSize, int valueSizeCol)
{

    try
    {
        AfficheBlock::setValue(getScilabJavaVM(), uid, value, valueSize, valueSizeCol);
    }
    catch (const GiwsException::JniException & exception)
    {
        /*
         * put a simulation error message.
         */
        Coserror(exception.whatStr().c_str());
        return -1;
    }
    return 0;
}
}
