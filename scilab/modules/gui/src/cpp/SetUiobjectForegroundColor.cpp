/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the foreground of an uicontrol or an uimenu
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "SetUicontrol.h"
}

int SetUiobjectForegroundColor(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /* Color can be [R, G, B] or "R|G|B" */

    double* allColors = NULL;
    BOOL status = FALSE;
    int nbValues = 0;


    if (valueType == sci_strings)
    {
        if (nbCol != 1 || nbRow == 0) /* More than one string */
        {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
            return SET_PROPERTY_ERROR;
        }

        allColors = new double[3];
        nbValues = sscanf((char*)_pvData, "%lf|%lf|%lf", &allColors[0], &allColors[1], &allColors[2]);

        if (nbValues != 3) /* Wrong format string */
        {
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
            return SET_PROPERTY_ERROR;
        }

    }
    else if (valueType == sci_matrix)
    {
        if (nbCol != 3 || nbRow != 1) /* Wrong matrix size */
        {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
            return SET_PROPERTY_ERROR;
        }

        allColors = (double*)_pvData;
    }
    else
    {
        /* Wrong value type */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: 1 x 3 real vector or a 'R|G|B' string expected.\n")), "ForegroundColor");
        return SET_PROPERTY_ERROR;
    }

    if (!checkColorRange(allColors[0], allColors[1], allColors[2]))
    {
        Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: Numbers between 0 and 1 expected.\n")), "ForegroundColor");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_FOREGROUNDCOLOR__, allColors, jni_double_vector, 3);

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
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "ForegroundColor");
        return SET_PROPERTY_ERROR;
    }
}

