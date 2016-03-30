/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
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

#include <sstream>
#include <cstdio>

#include "listundefined.hxx"
#include "localization.h"
#include "scilabWrite.hxx"

namespace types
{
ListUndefined::~ListUndefined()
{
}

void ListUndefined::whoAmI()
{
    std::cout << "types::ListUndefined";
}

ListUndefined* ListUndefined::clone(void)
{
    return new ListUndefined();
}

InternalType::ScilabType ListUndefined::getType(void)
{
    return ScilabListUndefinedOperation;
}

bool ListUndefined::toString(std::wostringstream& ostr)
{
    ostr << L"    Undefined" << std::endl;
    return true;
}
}
