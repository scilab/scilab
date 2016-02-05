/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
#include "ScilabView.hxx"

using namespace org_scilab_modules_graphic_objects;

void setGraphicObjectRelationship(int _parentId, int _childId)
{
    // do not perform anything if the id is undefined
    if (_parentId == 0 || _childId == 0)
    {
        return;
    }

    CallGraphicController::setGraphicObjectRelationship(getScilabJavaVM(), _parentId, _childId);
}

BOOL setGraphicObjectProperty(int _iID, int _iName, void const* _pvValue, _ReturnType_ _valueType, int numElements)
{
    return setGraphicObjectPropertyAndWarn(_iID, _iName, _pvValue, _valueType, numElements, 1);
}

BOOL setGraphicObjectPropertyAndNoWarn(int _iID, int _iName, void const* _pvValue, _ReturnType_ _valueType, int numElements)
{
    return setGraphicObjectPropertyAndWarn(_iID, _iName, _pvValue, _valueType, numElements, 0);
}

BOOL setGraphicObjectPropertyAndWarn(int _iID, int _iName, void const* _pvValue, _ReturnType_ _valueType, int numElements, int warnJava)
{
    bool result = false;

    double doubleValue = 0.;
    int intValue = 0;
    BOOL boolValue = FALSE;

    // do not perform anything if the id is undefined
    if (_iID == 0)
    {
        return FALSE;
    }

    switch (_iName)
    {
        case __GO_USER_DATA__ :
        {
            ScilabView::setUserdata(_iID, (int*) _pvValue, numElements);
            return TRUE;
        }
        case __GO_DATA_MODEL__ :
        case __GO_DATA_MODEL_COORDINATES__  :
        case __GO_DATA_MODEL_X__ :
        case __GO_DATA_MODEL_Y__ :
        case __GO_DATA_MODEL_Z__ :
        case __GO_DATA_MODEL_X_COORDINATES_SHIFT__ :
        case __GO_DATA_MODEL_Y_COORDINATES_SHIFT__ :
        case __GO_DATA_MODEL_Z_COORDINATES_SHIFT__ :
        case __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__ :
        case __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__ :
        case __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__ :
        case __GO_DATA_MODEL_NUM_ELEMENTS__ :
        case __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__ :
        case __GO_DATA_MODEL_NUM_VERTICES_PER_GON__ :
        case __GO_DATA_MODEL_NUM_GONS__ :
        case __GO_DATA_MODEL_Z_COORDINATES_SET__ :
        case __GO_DATA_MODEL_COLORS__ :
        case __GO_DATA_MODEL_NUM_COLORS__ :
        case __GO_DATA_MODEL_NUM_VERTICES__ :
        case __GO_DATA_MODEL_NUM_INDICES__ :
        case __GO_DATA_MODEL_INDICES__ :
        case __GO_DATA_MODEL_VALUES__ :
        case __GO_DATA_MODEL_FEC_ELEMENTS__ :
        case __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__ :
        case __GO_DATA_MODEL_NUM_X__ :
        case __GO_DATA_MODEL_NUM_Y__ :
        case __GO_DATA_MODEL_NUM_Z__ :
        case __GO_DATA_MODEL_GRID_SIZE__ :
        case __GO_DATA_MODEL_X_DIMENSIONS__ :
        case __GO_DATA_MODEL_Y_DIMENSIONS__ :
        case __GO_DATA_MODEL_MATPLOT_BOUNDS__ :
        case __GO_DATA_MODEL_MATPLOT_TYPE__ :
        case __GO_DATA_MODEL_MATPLOT_DATA_INFOS__ :
        case __GO_DATA_MODEL_MATPLOT_DATA_TYPE__ :
        case __GO_DATA_MODEL_MATPLOT_DATA_ORDER__ :
        case __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__ :
        case __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__ :
        case __GO_DATA_MODEL_DISPLAY_FUNCTION__ :
        {
            BOOL bFalse = FALSE;
            BOOL bTrue = TRUE;
            BOOL res;
            setGraphicObjectProperty(_iID, __GO_VALID__, &bFalse, jni_bool, 1);
            res = DataController::setGraphicObjectProperty(_iID, _iName, _pvValue, numElements);
            setGraphicObjectProperty(_iID, __GO_VALID__, &bTrue, jni_bool, 1);
            if ((res || _iName == __GO_DATA_MODEL__) && warnJava)
            {
                ///printf("debug property value=%d\n",_iName);
                CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, __GO_DATA_MODEL__, _iID);
            }
            return res;
        }
        default :
        {
            switch (_valueType)
            {
                case jni_string :
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (char *)_pvValue);
                    break;

                case jni_string_vector :
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (char**)_pvValue, numElements);
                    break;
                case jni_double :
                    doubleValue = *(double*)_pvValue;
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (double)doubleValue);
                    break;
                case jni_double_vector :
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (double*)_pvValue, numElements);
                    break;
                case jni_int :
                    intValue = *(int*)_pvValue;
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (int)intValue);
                    break;
                case jni_int_vector :
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (int*)_pvValue, numElements);
                    break;
                case jni_bool :
                    boolValue = *(BOOL*)_pvValue;
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, BOOLtobool(boolValue));
                    break;
                case jni_bool_vector :
                    result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _iID, _iName, (bool*)_pvValue, numElements);
                    break;
                default :
                    /* Do Nothing */
                    break;
            }
        }
    }
    return booltoBOOL(result);
}

