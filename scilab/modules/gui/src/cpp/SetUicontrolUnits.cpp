/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the units of an uicontrol object
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

int SetUicontrolUnits(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /* Units can be points, normalized, inches, centimeters or pixels */
    BOOL status = FALSE;
    char* units = NULL;
    int type = -1;
    int *piType = &type;

    /* Handle must be a uicontrol */
    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void**) &piType);
    if (type != __GO_UICONTROL__)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Units");
        return SET_PROPERTY_ERROR;
    }

    if (valueType == sci_strings)
    {
        if (nbCol != 1 || nbRow == 0)
        {
            /* Wrong string size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Units", "points", "normalized", "inches", "centimeters", "pixels");
            return SET_PROPERTY_ERROR;
        }

        units = (char*)_pvData;

        if (stricmp(units, "points") != 0
                && stricmp(units, "normalized") != 0
                && stricmp(units, "inches") != 0
                && stricmp(units, "centimeters") != 0
                && stricmp(units, "pixels") != 0)
        {
            /* Wrong string value */
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Units", "points", "normalized", "inches", "centimeters", "pixels");
            return SET_PROPERTY_ERROR;
        }

        status = setGraphicObjectProperty(iObjUID, __GO_UI_UNITS__, units, jni_string, 1);
        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Units");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Units", "points", "normalized", "inches", "centimeters", "pixels");
        return SET_PROPERTY_ERROR;
    }

}
