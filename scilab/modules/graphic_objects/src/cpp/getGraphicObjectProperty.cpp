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

void getGraphicObjectProperty(char const* _pstID, char const* _pstName, _ReturnType_ _returnType, void **_pvData)
{

    /* All the Data model properties have the DATA_MODEL prefix */
    if (strncmp(_pstName, __GO_DATA_MODEL__, strlen(__GO_DATA_MODEL__)) == 0)
    {
        DataController::getGraphicObjectProperty(_pstID, _pstName, _pvData);
        return;
    }

    try
    {
        switch (_returnType)
        {
            case jni_string:
            {
                *(_pvData) = CallGraphicController::getGraphicObjectPropertyAsString(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_string_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsStringVector(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_double:
            {
                ((double *)*_pvData)[0] = (double)CallGraphicController::getGraphicObjectPropertyAsDouble(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_double_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsDoubleVector(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_bool:
            {
                ((int *)*_pvData)[0] = (int)CallGraphicController::getGraphicObjectPropertyAsBoolean(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_bool_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsBooleanVector(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_int:
            {
                ((int *)*_pvData)[0] = CallGraphicController::getGraphicObjectPropertyAsInteger(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            case jni_int_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsIntegerVector(getScilabJavaVM(), _pstID, _pstName);
                return;
            }
            default:
                *_pvData = NULL;
                return;
        }
    }
    catch (std::exception &e)
    {
        e.what();
        // If we have an exception, return null to scilab to manage error.
        *_pvData = NULL;
        return;
    }

}


void releaseGraphicObjectProperty(char const* _pstName, void * _pvData, enum _ReturnType_ _returnType, int numElements)
{

    /* All the Data model properties have the DATA_MODEL prefix */
    if (strncmp(_pstName, __GO_DATA_MODEL__, strlen(__GO_DATA_MODEL__)) == 0)
    {
        // passed by reference, do not free them
        return;
    }

    switch (_returnType)
    {
        case jni_string:
        {
            delete[] (char*) _pvData;
            return;
        }
        case jni_string_vector:
        {
            char** data = (char**) _pvData;
            for (int i = 0; i < numElements; ++i)
            {
                delete[] data[i];
            }
            delete[] data;
            return;
        }
        case jni_double:
        {
            // passed by value
            return;
        }
        case jni_double_vector:
        {
            delete[] (double*) _pvData;
            return;
        }
        case jni_bool:
        {
            // passed by value
            return;
        }
        case jni_bool_vector:
        {
            delete[] (int*) _pvData;
            return;
        }
        case jni_int:
        {
            // passed by value
            return;
        }
        case jni_int_vector:
        {
            delete[] (int*) _pvData;
            return;
        }
        default:
            return;
    }

}

