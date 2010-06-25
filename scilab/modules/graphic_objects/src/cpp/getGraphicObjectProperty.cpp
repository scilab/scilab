/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


extern "C"
{
#include <stdlib.h>
#include "getGraphicObjectProperty.h"
#include "getScilabJavaVM.h"
}

#include "CallGraphicController.hxx"

using namespace org_scilab_modules_graphic_objects;

void *getGraphicObjectProperty(char *_pstID, char *_pstName, _ReturnType_ _returnType)
{
    switch(_returnType)
    {
    case jni_string :
        return CallGraphicController::getGraphicObjectPropertyAsString(getScilabJavaVM(), _pstID, _pstName);
    default :
        return NULL;
    }
}
