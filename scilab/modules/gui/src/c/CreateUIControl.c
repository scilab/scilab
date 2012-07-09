/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Create Java object matching Scilab uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "CreateUIControl.h"
#include "BuildObjects.h"
#include "DestroyObjects.h" /* sciStandardDestroyOperations */

#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"

/**CreateUIControl
 * This function creates Uicontrol structure.
 * @param char style[]: the type of the uicontrol object
 * @return  : created object UID if ok , NULL if not
 */
char *CreateUIControl(char *style)
{
    char *pobjUID = NULL;
    if (style == NULL || strcmp(style, "pushbutton") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_PUSHBUTTON__);
    }
    else if (strcmp(style, "checkbox") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_CHECKBOX__);
    }
    else if (strcmp(style, "edit") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_EDIT__);
    }
    else if (strcmp(style, "frame") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_FRAME__);
    }
    else if (strcmp(style, "listbox") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_LISTBOX__);
    }
    else if (strcmp(style, "image") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_IMAGE__);
    }
    else if (strcmp(style, "popupmenu") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_POPUPMENU__);
    }
    else if (strcmp(style, "radiobutton") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_RADIOBUTTON__);
    }
    else if (strcmp(style, "slider") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_SLIDER__);
    }
    else if (strcmp(style, "table") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_TABLE__);
    }
    else if (strcmp(style, "text") == 0)
    {
        pobjUID = createGraphicObject(__GO_UI_TEXT__);
    }

    return pobjUID;
}
/*----------------------------------------------------------------------------*/

