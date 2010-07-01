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

BOOL setGraphicObjectProperty(char *_pstID, char *_pstName, void *_pvValue, _ReturnType_ _valueType, int numElements)
{
 bool result = false;

 double doubleValue;
 int intValue;
 bool boolValue;

 switch(_valueType)
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

	boolValue = *(bool*)_pvValue;

        result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (bool)boolValue);
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

/* The following functions are used for debugging purposes */

BOOL setGraphicObjectStringVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements)
{
  bool result;

  result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (char**)_pstName, numElements);

  return booltoBOOL(result);
}

BOOL setGraphicObjectDoubleProperty(char *_pstID, char *_pstName, double value)
{
  bool result;

  result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, value);

  return booltoBOOL(result);
}

BOOL setGraphicObjectDoubleVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements)
{
  bool result;

  result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (double*)_pvValue, numElements);

  return booltoBOOL(result);
}

BOOL setGraphicObjectIntegerVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements)
{
  bool result;

  result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (int*)_pvValue, numElements);

  return booltoBOOL(result);
}

BOOL setGraphicObjectBooleanVectorProperty(char *_pstID, char *_pstName, void *_pvValue, int numElements)
{
  bool result;

  result = CallGraphicController::setGraphicObjectProperty(getScilabJavaVM(), _pstID, _pstName, (bool*)_pvValue, numElements);

  return booltoBOOL(result);
}

