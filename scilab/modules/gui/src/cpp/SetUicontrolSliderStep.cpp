/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the slider step property of an uicontrol object
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolSliderStep.hxx"
#include "stack-c.h"

int SetUicontrolSliderStep(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A 1 x %d real row vector expected.\n")), "SliderStep", 2);
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow == 1 && nbCol == 1)
    {
        double pdblStep[2];
        double* pdblStackVal = stk(stackPointer);
        double maxValue = 0;
        double* pdblMaxValue = &maxValue;
        double minValue = 0;
        double* pdblMinValue = &minValue;

        getGraphicObjectProperty(sciObjUID, __GO_UI_MIN__, jni_double, (void**) &pdblMinValue);
        getGraphicObjectProperty(sciObjUID, __GO_UI_MAX__, jni_double, (void**) &pdblMaxValue);

        pdblStep[0] = pdblStackVal[0];
        pdblStep[1] = 0.1 * (maxValue - minValue);// default big value : 10% of the scale

        status = setGraphicObjectProperty(sciObjUID, __GO_UI_SLIDERSTEP__, pdblStep, jni_double_vector, 2);
    }
    else if (nbRow == 1 && nbCol == 2)
    {
        status = setGraphicObjectProperty(sciObjUID, __GO_UI_SLIDERSTEP__, stk(stackPointer), jni_double_vector, 2);
    }
    else
    {
        /* Wrong value size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A 1 x %d real row vector expected.\n")), "SliderStep", 2);
        return SET_PROPERTY_ERROR;
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "SliderStep");
        return SET_PROPERTY_ERROR;
    }
}
