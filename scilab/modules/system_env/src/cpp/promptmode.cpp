/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>

#include "promptmode.h"

#include "configvariable.hxx"

#include "setenvvar.hxx"
#include "string.hxx"
#include "context.hxx"

int getPromptMode(void)
{
    return static_cast<int>(ConfigVariable::getPromptMode());
}

void setPromptMode(int _iMode)
{
    ConfigVariable::setPromptMode(_iMode);
}
