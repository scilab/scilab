/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

bool ListOperation::toString(std::ostringstream& ostr)
{
    ostr << "FIXME : Implement ListOperation::toString" << std::endl;
    return true;
}
}
