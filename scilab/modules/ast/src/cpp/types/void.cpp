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

#include "void.hxx"
#include "localization.h"
#include "scilabWrite.hxx"

namespace types
{
Void::~Void()
{
}

void Void::whoAmI()
{
    std::cout << "types::Void";
}

Void* Void::clone(void)
{
    return new Void();
}

bool Void::toString(std::ostringstream& ostr)
{
    ostr << "" << std::endl;
    return true;
}
}
