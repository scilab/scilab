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

BOOL setGraphicObjectProperty(char const* _pstID, int _iName, void const* _pvValue, _ReturnType_ _valueType, int numElements)
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
    if (_iName == __GO_DATA_MODEL__
        || _iName == __GO_DATA_MODEL_COORDINATES__
        || _iName == __GO_DATA_MODEL_X__
        || _iName == __GO_DATA_MODEL_Y__
        || _iName == __GO_DATA_MODEL_Z__
        || _iName == __GO_DATA_MODEL_X_COORDINATES_SHIFT__
        || _iName == __GO_DATA_MODEL_Y_COORDINATES_SHIFT__
        || _iName == __GO_DATA_MODEL_Z_COORDINATES_SHIFT__
        || _iName == __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__
        || _iName == __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__
        || _iName == __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__
        || _iName == __GO_DATA_MODEL_NUM_ELEMENTS__
        || _iName == __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__
        || _iName == __GO_DATA_MODEL_NUM_VERTICES_PER_GON__
        || _iName == __GO_DATA_MODEL_NUM_GONS__
        || _iName == __GO_DATA_MODEL_Z_COORDINATES_SET__
        || _iName == __GO_DATA_MODEL_COLORS__
        || _iName == __GO_DATA_MODEL_NUM_COLORS__
        || _iName == __GO_DATA_MODEL_NUM_VERTICES__
        || _iName == __GO_DATA_MODEL_NUM_INDICES__
        || _iName == __GO_DATA_MODEL_INDICES__
        || _iName == __GO_DATA_MODEL_VALUES__
        || _iName == __GO_DATA_MODEL_FEC_TRIANGLES__
        || _iName == __GO_DATA_MODEL_NUM_X__
        || _iName == __GO_DATA_MODEL_NUM_Y__
        || _iName == __GO_DATA_MODEL_NUM_Z__
        || _iName == __GO_DATA_MODEL_GRID_SIZE__
        || _iName == __GO_DATA_MODEL_X_DIMENSIONS__
        || _iName == __GO_DATA_MODEL_Y_DIMENSIONS__
        || _iName == __GO_DATA_MODEL_MATPLOT_BOUNDS__
        || _iName == __GO_DATA_MODEL_MATPLOT_TYPE__
        )
    {
        result = BOOLtobool(DataController::setGraphicObjectProperty(_pstID, _iName, _pvValue, numElements));
        CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, __GO_DATA_MODEL__, _pstID);
        return booltoBOOL(result);
    }

    switch (_valueType)
    {
        case jni_string :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (char *)_pvValue);
            break;

        case jni_string_vector :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (char**)_pvValue, numElements);
            break;

        case jni_double :
            doubleValue = *(double*)_pvValue;
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (double)doubleValue);
            break;

        case jni_double_vector :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (double*)_pvValue, numElements);
            break;

        case jni_int :

            intValue = *(int*)_pvValue;

            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (int)intValue);
            break;

        case jni_int_vector :
            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (int*)_pvValue, numElements);
            break;

        case jni_bool :

            boolValue = *(BOOL*)_pvValue;

            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, BOOLtobool(boolValue));
            break;

        case jni_bool_vector :

            result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _iName, (bool*)_pvValue, numElements);

            break;

        default :
            /* Do Nothing */
            break;
    }

    return booltoBOOL(result);
}

