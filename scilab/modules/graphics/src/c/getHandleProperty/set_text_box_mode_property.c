/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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

/*------------------------------------------------------------------------*/
/* file: set_text_box_mode_property.c                                     */
/* desc : function to modify in Scilab the text_box_mode field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "os_string.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "sci_malloc.h"

/*------------------------------------------------------------------------*/
int set_text_box_mode_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[2];
    int autoSize = 0;
    int textBoxMode = 0;
    int status1 = 0;
    int status2 = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "text_box_mode");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "off") == 0)
    {
        autoSize = 1;
        textBoxMode = 0;
    }
    else if (stricmp((char*)_pvData, "centered") == 0)
    {
        autoSize = 1;
        textBoxMode = 1;
    }
    else if (stricmp((char*)_pvData, "filled") == 0)
    {
        autoSize = 0;
        textBoxMode = 2;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "text_box_mode", "off, centered, filled");
        return SET_PROPERTY_ERROR;
    }

    status[0] = setGraphicObjectProperty(iObjUID, __GO_TEXT_BOX_MODE__, &textBoxMode, jni_int, 1);
    status[1] = setGraphicObjectProperty(iObjUID, __GO_AUTO_DIMENSIONING__, &autoSize, jni_bool, 1);

    if (status[0] == TRUE)
    {
        status1 = SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "text_box_mode");
        status1 = SET_PROPERTY_ERROR;
    }

    if (status[1] == TRUE)
    {
        status2 = SET_PROPERTY_SUCCEED;
    }
    else
    {
        status2 = SET_PROPERTY_ERROR;
    }

    return sciSetFinalStatus((SetPropertyStatus)status1, (SetPropertyStatus)status2);
}
/*------------------------------------------------------------------------*/
