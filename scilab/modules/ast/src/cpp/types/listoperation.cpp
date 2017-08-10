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

#include "listoperation.hxx"
#include "localization.h"
#include "scilabWrite.hxx"

namespace types
{
ListOperation::~ListOperation()
{
}

void ListOperation::whoAmI()
{
    std::cout << "types::ListDelete";
}

ListOperation* ListOperation::clone(void)
{
    return new ListOperation();
}

bool ListOperation::toString(std::wostringstream& ostr)
{
    ostr << L"FIXME : Implement ListOperation::toString" << std::endl;
    return true;
}
}
