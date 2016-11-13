/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the background of an uicontrol object
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
#include "addColor.h"
#include "SetUicontrol.h"
}

int SetUicontrolBackgroundColor(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /* Color can be [R, G, B] or "R|G|B" */

    double* allColors = NULL;
    BOOL status = FALSE;
    int nbValues = 0;
    int iType = -1;
    int *piType = &iType;
    int iColorIndex = 0;

    if (valueType == sci_strings)
    {
        if (nbCol != 1 || nbRow == 0)
        {
            /* Wrong string size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "BackgroundColor");
            return SET_PROPERTY_ERROR;
        }

        allColors = new double[3];
        nbValues = sscanf((char*)_pvData, "%lf|%lf|%lf", &allColors[0], &allColors[1], &allColors[2]);

        if (nbValues != 3)
        {
            /* Wrong string format */
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "BackgroundColor");
            delete[] allColors;
            return SET_PROPERTY_ERROR;
        }

    }
    else if (valueType == sci_matrix)
    {
        if (nbCol != 3 || nbRow != 1)
        {
            /* Wrong matrix size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "BackgroundColor");
            return SET_PROPERTY_ERROR;
        }

        allColors = (double*)_pvData;

    }
    else
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "BackgroundColor");
        return SET_PROPERTY_ERROR;
    }

    if (!checkColorRange(allColors[0], allColors[1], allColors[2]))
    {
        Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: Numbers between 0 and 1 expected.\n")), "BackgroundColor");
        if (valueType == sci_strings)
        {
            delete[] allColors;
        }
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **) &piType);
    switch (iType)
    {
        case __GO_FIGURE__ :
            iColorIndex = addColor(iObjUID, allColors);
            status = setGraphicObjectProperty(iObjUID, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);
            break;
        default :
            status = setGraphicObjectProperty(iObjUID, __GO_UI_BACKGROUNDCOLOR__, allColors, jni_double_vector, 3);
            break;
    }


    if (valueType == sci_strings)
    {
        delete[] allColors;
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "BackgroundColor");
        return SET_PROPERTY_ERROR;
    }
}
