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
/* file: set_anti_aliasing_property.c                                     */
/* desc : function to modify in Scilab the anti_aliasing field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "os_string.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_anti_aliasing_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{

    int quality = 0;
    BOOL status = FALSE;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "anti_aliasing");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "off") == 0)
    {
        quality = 0;
    }
    else if (stricmp((char*)_pvData, "2x") == 0)
    {
        quality = 1;
    }
    else if (stricmp((char*)_pvData, "4x") == 0)
    {
        quality = 2;
    }
    else if (stricmp((char*)_pvData, "8x") == 0)
    {
        quality = 3;
    }
    else if (stricmp((char*)_pvData, "16x") == 0)
    {
        quality = 4;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "anti_aliasing", "off, 2x, 4x, 8x, 16x");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_ANTIALIASING__, &quality, jni_int, 1);
    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "anti_aliasing");
        return SET_PROPERTY_ERROR;
    }

    return status;
}
/*------------------------------------------------------------------------*/
