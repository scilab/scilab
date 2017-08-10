/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the slider step property of an uicontrol object
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
#include "SetUicontrol.h"
}

int SetUicontrolSliderStep(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
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
        double* pdblStackVal = (double*)_pvData;
        double maxValue = 0;
        double* pdblMaxValue = &maxValue;
        double minValue = 0;
        double* pdblMinValue = &minValue;

        getGraphicObjectProperty(iObjUID, __GO_UI_MIN__, jni_double, (void**) &pdblMinValue);
        getGraphicObjectProperty(iObjUID, __GO_UI_MAX__, jni_double, (void**) &pdblMaxValue);

        pdblStep[0] = pdblStackVal[0];
        pdblStep[1] = 0.1 * (maxValue - minValue);// default big value : 10% of the scale

        status = setGraphicObjectProperty(iObjUID, __GO_UI_SLIDERSTEP__, pdblStep, jni_double_vector, 2);
    }
    else if (nbRow == 1 && nbCol == 2)
    {
        status = setGraphicObjectProperty(iObjUID, __GO_UI_SLIDERSTEP__, _pvData, jni_double_vector, 2);
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
