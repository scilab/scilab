/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Vincent COUVERT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "getScilabJavaVM.h"
#include "getConsoleIdentifier.h"
}

#include "CallGraphicController.hxx"

using namespace org_scilab_modules_graphic_objects;

int getConsoleIdentifier()
{
    return CallGraphicController::getConsoleIdentifier(getScilabJavaVM());
}
