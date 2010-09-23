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
#include <string.h>
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getScilabJavaVM.h"
}

#include "CallGraphicController.hxx"
#include "DataController.hxx"


using namespace org_scilab_modules_graphic_objects;

void *getGraphicObjectProperty(char *_pstID, char *_pstName, _ReturnType_ _returnType)
{
    int lenRow;
    static int localIntResult;
    static int localBoolResult;
    static double localDoubleResult;

    /* All the Data model properties have the DATA_MODEL prefix */
    if (strncmp(_pstName, __GO_DATA_MODEL__, strlen(__GO_DATA_MODEL__)) == 0)
    {
        return DataController::getGraphicObjectProperty(_pstID, _pstName);
    }

    try {
        switch(_returnType)
        {
        case jni_string :
            return CallGraphicController::getGraphicObjectPropertyAsString(getScilabJavaVM(), _pstID, _pstName);
        case jni_string_vector :
            return CallGraphicController::getGraphicObjectPropertyAsStringVector(getScilabJavaVM(), _pstID, _pstName, &lenRow);
        case jni_double :
        {
            localDoubleResult = CallGraphicController::getGraphicObjectPropertyAsDouble(getScilabJavaVM(), _pstID, _pstName);
            return &localDoubleResult;
        }
        case jni_double_vector :
            return CallGraphicController::getGraphicObjectPropertyAsDoubleVector(getScilabJavaVM(), _pstID, _pstName, &lenRow);
        case jni_bool :
        {
            localBoolResult = (int)CallGraphicController::getGraphicObjectPropertyAsBoolean(getScilabJavaVM(), _pstID, _pstName);
            return &localBoolResult;
        }
        case jni_bool_vector :
            return CallGraphicController::getGraphicObjectPropertyAsBooleanVector(getScilabJavaVM(), _pstID, _pstName, &lenRow);
        case jni_int :
        {
            localIntResult = CallGraphicController::getGraphicObjectPropertyAsInteger(getScilabJavaVM(), _pstID, _pstName);
            return &localIntResult;
        }
        case jni_int_vector :
            return CallGraphicController::getGraphicObjectPropertyAsIntegerVector(getScilabJavaVM(), _pstID, _pstName, &lenRow);

        default :
            return NULL;
        }
    }
    catch (std::exception e)
    {
        // If we have an exception, return null to scilab to manage error.
        return NULL;
    }

}

