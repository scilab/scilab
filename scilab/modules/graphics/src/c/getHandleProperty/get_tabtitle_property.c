/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "TitlePositionType.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_title_scroll_property(void* _pvCtx, int iObjUID)
{
    int scroll = 0;
    int* piScroll = &scroll;

    getGraphicObjectProperty(iObjUID, __GO_UI_TITLE_SCROLL__, jni_bool, (void **)&piScroll);
    if (piScroll == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "title_scroll");
        return FALSE;
    }

    if (scroll)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}
/*------------------------------------------------------------------------*/
void* get_title_position_property(void* _pvCtx, int iObjUID)
{
    int iPos = 0;
    int* piPos = &iPos;
    getGraphicObjectProperty(iObjUID, __GO_UI_TITLE_POSITION__, jni_int, (void **)&piPos);

    if (piPos == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "title_position");
        return NULL;
    }

    switch (iPos)
    {
        default :
        case TITLE_TOP :
            return sciReturnString("top");
        case TITLE_LEFT :
            return sciReturnString("left");
        case TITLE_BOTTOM :
            return sciReturnString("bottom");
        case TITLE_RIGHT :
            return sciReturnString("right");
    }
}
