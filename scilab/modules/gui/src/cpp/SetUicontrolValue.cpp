/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the value property of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolValue.hxx"

int SetUicontrolValue(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    double *value = NULL;
    int* valueAsInt = NULL;
    int valueSize = 0;
    int nbValues = 0;
    int kValue = 0;
    BOOL status = FALSE;

    if (valueType == sci_matrix)
    {
        if(nbRow > 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real row vector expected.\n")), "Value");
            return SET_PROPERTY_ERROR;
        }

        value = getDoubleMatrixFromStack(stackPointer);
        valueSize = nbCol * nbRow;
    }
    else if (valueType == sci_strings) // Ascendant compatibility
    {
        if(nbCol > 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "Value");
            return SET_PROPERTY_ERROR;
        }

        value = new double[1];
        valueSize = 1;
        nbValues = sscanf(getStringFromStack(stackPointer), "%lf", &value[0]);

        if(nbValues != 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: A String containing a numeric value expected.\n")), "Value");
            return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real row vector or a string expected.\n")), "Value");
      return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObj->UID, const_cast<char*>(__GO_UI_VALUE_SIZE__), &valueSize, jni_int, 1);

    if (status != TRUE)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Value");
        return SET_PROPERTY_ERROR;
    }

    valueAsInt = new int[valueSize];
    for (kValue = 0; kValue < valueSize; kValue++)
    {
        valueAsInt[kValue] = (int) value[kValue];
    }

    status = setGraphicObjectProperty(sciObj->UID, const_cast<char*>(__GO_UI_VALUE__), valueAsInt, jni_int_vector, valueSize);

    delete[] valueAsInt;

    if (valueType == sci_strings)
    {
        delete[] value;
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Value");
        return SET_PROPERTY_ERROR;
    }
}
