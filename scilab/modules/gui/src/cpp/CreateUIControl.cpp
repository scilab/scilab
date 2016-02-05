/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Create Java object matching Scilab uicontrol
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
#include <string.h>
#include "CreateUIControl.h"
#include "BuildObjects.h"
#include "DestroyObjects.h" /* sciStandardDestroyOperations */

#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
}

#include <map>
#include <string>

static bool init = false;
static std::map<std::string, int> styleMap;

void initStyleMap()
{
    styleMap["pushbutton"] = __GO_UI_PUSHBUTTON__;
    styleMap["checkbox"] = __GO_UI_CHECKBOX__;
    styleMap["edit"] = __GO_UI_EDIT__;
    styleMap["frame"] = __GO_UI_FRAME__;
    styleMap["framescrollable"] = __GO_UI_FRAME_SCROLLABLE__;
    styleMap["listbox"] = __GO_UI_LISTBOX__;
    styleMap["image"] = __GO_UI_IMAGE__;
    styleMap["popupmenu"] = __GO_UI_POPUPMENU__;
    styleMap["radiobutton"] = __GO_UI_RADIOBUTTON__;
    styleMap["slider"] = __GO_UI_SLIDER__;
    styleMap["table"] = __GO_UI_TABLE__;
    styleMap["text"] = __GO_UI_TEXT__;
    styleMap["tab"] = __GO_UI_TAB__;
    styleMap["layer"] = __GO_UI_LAYER__;
    styleMap["spinner"] = __GO_UI_SPINNER__;
    init = true;
}

/**CreateUIControl
 * This function creates Uicontrol structure.
 * @param char style[]: the type of the uicontrol object
 * @return  : created object UID if ok , NULL if not
 */
int CreateUIControl(char *style)
{
    if (init == false)
    {
        initStyleMap();
    }

    if (style == NULL)
    {
        return createGraphicObject(styleMap["pushbutton"]);
    }

    std::map<std::string, int>::iterator it = styleMap.find(style);
    if (it == styleMap.end())
    {
        return 0;
    }
    return createGraphicObject(it->second);
}
/*----------------------------------------------------------------------------*/

