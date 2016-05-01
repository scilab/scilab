/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_clip_state_property.c                                        */
/* desc : function to modify in Scilab the clip_state field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "os_string.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "sci_malloc.h"
/*------------------------------------------------------------------------*/
int set_clip_state_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int clipState = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "clip_state");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "clipgrf") == 0)
    {
        clipState = 1;
    }
    else if (stricmp((char*)_pvData, "off") == 0)
    {
        clipState = 0;
    }
    else if (stricmp((char*)_pvData, "on") == 0)
    {
        clipState = 2;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "clip_state", "on, off, clipgrf");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "clip_state");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
