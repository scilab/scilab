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
/* file: set_rotation_style_property.c                                    */
/* desc : function to modify in Scilab the rotation_style field of        */
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
#include "sci_malloc.h"
/*------------------------------------------------------------------------*/
int set_rotation_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int rotationStyle = 0;
    BOOL status = FALSE;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "rotation_style");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "unary") == 0)
    {
        rotationStyle = 0;
    }
    else if (stricmp((char*)_pvData, "multiple") == 0)
    {
        rotationStyle = 1;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "rotation_style", "unary", "multiple");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_ROTATION_TYPE__, &rotationStyle, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "rotation_style");
        return SET_PROPERTY_ERROR;
    }
}

/*------------------------------------------------------------------------*/
