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
/* file: set_tics_style_property.c                                        */
/* desc : function to modify in Scilab the tics_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "os_string.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "Format.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_tics_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int ticksStyle = 0;

    char xy_type = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "tics_style");
        return SET_PROPERTY_ERROR;
    }

    if (   stricmp((char*)_pvData, "v") != 0 &&
            stricmp((char*)_pvData, "r") != 0 &&
            stricmp((char*)_pvData, "i") != 0)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "tics_style", "v, r, i");
        return SET_PROPERTY_ERROR;
    }

    /* get the character 'v', 'r' or 'i' */
    xy_type = ((char*)_pvData)[0];

    if (xy_type == 'v')
    {
        ticksStyle = 0;
    }
    else if (xy_type == 'r')
    {
        ticksStyle = 1;
    }
    else if (xy_type == 'i')
    {
        ticksStyle = 2;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_TICKS_STYLE__, &ticksStyle, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_style");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
