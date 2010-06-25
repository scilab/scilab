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
#include "setGraphicObjectProperty.h"
#include "getScilabJavaVM.h"
}

#include "CallGraphicController.hxx"

using namespace org_scilab_modules_graphic_objects;

void setGraphicObjectProperty(char *_pstID, char *_pstName, void *_pvValue, _ReturnType_ _valueType)
{
 switch(_valueType)
    {
    case jni_string :
        CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (char *)_pvValue);
        break;
    default :
        /* Do Nothing */
        break;
    }
}
