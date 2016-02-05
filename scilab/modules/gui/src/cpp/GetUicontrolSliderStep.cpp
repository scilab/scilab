/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Get the SliderStep property of an uicontrol
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
#include "GetUicontrol.h"
}

void* GetUicontrolSliderStep(void* _pvCtx, int iObjUID)
{
    double *sliderStep = NULL;
    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_SLIDERSTEP__, jni_double_vector, (void**) &sliderStep);

    if (sliderStep != NULL)
    {
        status = sciReturnRowVector(sliderStep, 2);
        delete[] sliderStep;
        return status;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "SliderStep");
        return NULL;
    }
}
