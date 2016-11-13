/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Vincent COUVERT
* Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

/**
 ** \brief Get the style of an uicontrol object
 */

const char *IntToStyle(int _iStyle)
{
    switch (_iStyle)
    {
        case __GO_UI_CHECKBOX__ :
            return "checkbox";
        case __GO_UI_EDIT__ :
            return "edit";
        case __GO_UI_FRAME__ :
            return "frame";
        case __GO_UI_IMAGE__ :
            return "image";
        case __GO_UI_LISTBOX__ :
            return "listbox";
        case __GO_UI_POPUPMENU__ :
            return "popupmenu";
        case __GO_UI_PUSHBUTTON__ :
            return "pushbutton";
        case __GO_UI_RADIOBUTTON__ :
            return "radiobutton";
        case __GO_UI_SLIDER__ :
            return "slider";
        case __GO_UI_TABLE__ :
            return "table";
        case __GO_UI_TEXT__ :
            return "text";
        case __GO_UI_TAB__ :
            return "tab";
        case __GO_UI_LAYER__ :
            return "layer";
        case __GO_UI_SPINNER__ :
            return "spinner";
    }

    return NULL;
}

void* GetUicontrolStyle(void* _pvCtx, int iObjUID)
{
    int iStyle = -1;
    int *piStyle = &iStyle;

    getGraphicObjectProperty(iObjUID, __GO_STYLE__, jni_int, (void **) &piStyle);

    if (piStyle != NULL)
    {
        return sciReturnString(IntToStyle(iStyle));
    }
    else
    {
        Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Style");
        return NULL;
    }

}
