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
#include "graphicObjectProperties.h"
#include "getScilabJavaVM.h"

#include <stdio.h>
#include <string.h>
}

#include "CallGraphicController.hxx"
#include "DataController.hxx"

using namespace org_scilab_modules_graphic_objects;

void setGraphicObjectRelationship(char const* _parentId, char const* _childId)
{
    // do not perform anything if the id is undefined
    if (_parentId == NULL || _childId == NULL)
    {
        return;
    }

    CallGraphicController::setGraphicObjectRelationship(getScilabJavaVM(), _parentId, _childId);
}

BOOL setGraphicObjectProperty(char const* _pstID, char const* _pstName, void const* _pvValue, _ReturnType_ _valueType, int numElements)
{
    bool result = false;

    double doubleValue = 0.;
    int intValue = 0;
    BOOL boolValue = FALSE;

    // do not perform anything if the id is undefined
    if (_pstID == NULL)
    {
        return FALSE;
    }


    // Special Case for data, no need to go through Java.
    if (strncmp(_pstName, __GO_DATA_MODEL__, strlen(__GO_DATA_MODEL__)) == 0)
    {
        result = BOOLtobool(DataController::setGraphicObjectProperty(_pstID, _pstName, _pvValue, numElements));
        CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, __GO_DATA_MODEL__, _pstID);
        return booltoBOOL(result);
    }

    switch (_valueType)
    {
        case jni_string :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (char *)_pvValue);
            break;

        case jni_string_vector :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (char**)_pvValue, numElements);
            break;

        case jni_double :
            doubleValue = *(double*)_pvValue;
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (double)doubleValue);
            break;

        case jni_double_vector :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (double*)_pvValue, numElements);
            break;

        case jni_int :

            intValue = *(int*)_pvValue;

            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (int)intValue);
            break;

        case jni_int_vector :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (int*)_pvValue, numElements);
            break;

        case jni_bool :

            boolValue = *(BOOL*)_pvValue;

            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, BOOLtobool(boolValue));
            break;

        case jni_bool_vector :

            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (bool*)_pvValue, numElements);

            break;

        default :
            /* Do Nothing */
            break;
    }

    return booltoBOOL(result);
}

