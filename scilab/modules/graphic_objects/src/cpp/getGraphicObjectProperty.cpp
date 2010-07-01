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


/*
 * This function is only partially implemented, the functions below are used instead
 * to perform property get calls
 */
void *getGraphicObjectProperty(char *_pstID, char *_pstName, _ReturnType_ _returnType)
{
    switch(_returnType)
    {
    case jni_string :
       return CallGraphicController::getGraphicObjectPropertyAsString(getScilabJavaVM(), _pstID, _pstName);
    case jni_bool :
	return (void*)CallGraphicController::getGraphicObjectPropertyAsBoolean(getScilabJavaVM(), _pstID, _pstName);
    default :
        return NULL;
    }

}

/* These functions are used to get properties for now */

char* getGraphicObjectStringProperty(char *_pstID, char *_pstName)
{
    return (char*)CallGraphicController::getGraphicObjectPropertyAsString(getScilabJavaVM(), _pstID, _pstName);
}

char** getGraphicObjectStringVectorProperty(char *_pstID, char *_pstName)
{
    return (char**)CallGraphicController::getGraphicObjectPropertyAsStringVector(getScilabJavaVM(), _pstID, _pstName);
}

double getGraphicObjectDoubleProperty(char *_pstID, char *_pstName)
{
    return (double)CallGraphicController::getGraphicObjectPropertyAsDouble(getScilabJavaVM(), _pstID, _pstName);
}

double* getGraphicObjectDoubleVectorProperty(char *_pstID, char *_pstName)
{
    return (double*)CallGraphicController::getGraphicObjectPropertyAsDoubleVector(getScilabJavaVM(), _pstID, _pstName);
}

int getGraphicObjectIntegerProperty(char *_pstID, char *_pstName)
{
    return (int)CallGraphicController::getGraphicObjectPropertyAsInteger(getScilabJavaVM(), _pstID, _pstName);
}

int* getGraphicObjectIntegerVectorProperty(char *_pstID, char *_pstName)
{
    return (int*)CallGraphicController::getGraphicObjectPropertyAsIntegerVector(getScilabJavaVM(), _pstID, _pstName);
}

int getGraphicObjectBooleanProperty(char *_pstID, char *_pstName)
{
    return (int)CallGraphicController::getGraphicObjectPropertyAsBoolean(getScilabJavaVM(), _pstID, _pstName);
}

int* getGraphicObjectBooleanVectorProperty(char *_pstID, char *_pstName)
{
    return (int*)CallGraphicController::getGraphicObjectPropertyAsIntegerVector(getScilabJavaVM(), _pstID, _pstName);
}

