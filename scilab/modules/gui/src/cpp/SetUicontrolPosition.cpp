/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the position of an uicontrol object
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

int SetUicontrolPosition(int iObjUID, void* pvData, int valueType, int nbRow, int nbCol)
{
    // Position can be [x, y, width, height] or "x|y|width|height"

    double *position = NULL;
    int nbValues = 0;
    BOOL status = FALSE;
    int type = -1;
    int *piType = &type;

    if (valueType == sci_strings)
    {
        if (nbCol != 1)
        {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: string or 1 x %d real row vector expected.\n")), "Position", 4);
            return SET_PROPERTY_ERROR;
        }

        position = new double[4];
        nbValues = sscanf((char*)pvData, "%lf|%lf|%lf|%lf", &position[0], &position[1], &position[2], &position[3]);

        if (nbValues != 4)
        {
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: string or 1 x %d real row vector expected.\n")), "Position", 4);
            delete[] position;
            return SET_PROPERTY_ERROR;
        }
    }
    else if (valueType == sci_matrix)
    {
        if (nbCol != 4 || nbRow != 1)
        {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: string or 1 x %d real row vector expected.\n")), "Position", 4);
            return SET_PROPERTY_ERROR;
        }

        position = (double*)pvData;

    }
    else
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: string or 1 x %d real row vector expected.\n")), "Position", 4);
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void**)&piType);

    /* Figure position set as an uicontrol one */
    if (type == __GO_FIGURE__)
    {
        int figurePosition[2];
        int figureSize[2];

        figurePosition[0] = (int) position[0];
        figurePosition[1] = (int) position[1];
        figureSize[0] = (int) position[2];
        figureSize[1] = (int) position[3];

        status = setGraphicObjectProperty(iObjUID, __GO_POSITION__, figurePosition, jni_int_vector, 2);
        if (status == FALSE)
        {
            Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Position");
            if (valueType == sci_strings)
            {
                delete[] position;
            }
            return SET_PROPERTY_ERROR;
        }
        status = setGraphicObjectProperty(iObjUID, __GO_AXES_SIZE__, figureSize, jni_int_vector, 2);
    }
    else
    {
        status = setGraphicObjectProperty(iObjUID, __GO_POSITION__, position, jni_double_vector, 4);
    }

    if (valueType == sci_strings)
    {
        delete[] position;
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Position");
        return SET_PROPERTY_ERROR;
    }
}
