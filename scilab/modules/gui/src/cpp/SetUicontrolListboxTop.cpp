/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
 *
 * Sets the listbox top property of an uicontrol object
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

int SetUicontrolListboxTop(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int value = 0;
    int* valueTab;
    int valueSize = 0;
    int nbValues = 0;
    BOOL status = FALSE;

    if (valueType == sci_matrix)
    {
        if (nbCol > 1 || nbRow > 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real expected.\n")), "ListboxTop");
            return SET_PROPERTY_ERROR;
        }

        value = (int)((double*)_pvData)[0];
    }
    else if (valueType == sci_strings) // Ascendant compatibility
    {
        if (nbCol > 1 || nbRow > 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real expected.\n")), "ListboxTop");
            return SET_PROPERTY_ERROR;
        }

        nbValues = sscanf((char*)_pvData, "%d", &value);

        if (nbValues != 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: A real expected.\n")), "ListboxTop");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real expected.\n")), "ListboxTop");
        return SET_PROPERTY_ERROR;
    }

    valueSize = nbCol * nbRow;
    valueTab = new int[valueSize];
    if (valueSize > 0)
    {
        valueTab[0] = value;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_LISTBOXTOP__, valueTab, jni_int_vector, valueSize);

    delete[] valueTab;

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ListboxTop");
        return SET_PROPERTY_ERROR;
    }
}
