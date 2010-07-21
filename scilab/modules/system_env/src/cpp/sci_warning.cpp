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

#include "sci_warning.h"

#include "configvariable.hxx"

#include "setenvvar.hxx"
#include "string.hxx"
#include "context.hxx"

int getWarningMode(void)
{
    if(ConfigVariable::getWarningMode())
    {
        return 1;
    }
    return 0;
}

void setWarningMode(int _iMode)
{
    if(_iMode == 0)
    {
        ConfigVariable::setWarningMode(false);
    }

    ConfigVariable::setWarningMode(true);
}
