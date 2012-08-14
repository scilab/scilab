/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_tics_style_property.c                                        */
/* desc : function to modify in Scilab the tics_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

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
int set_tics_style_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int ticksStyle = 0;

    char xy_type = 0;

    if ( !( valueType == sci_strings ) )
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "tics_style");
        return SET_PROPERTY_ERROR;
    }

    if (    !isStringParamEqual( stackPointer, "v" )
            && !isStringParamEqual( stackPointer, "r" )
            && !isStringParamEqual( stackPointer, "i" ) )
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "tics_style", "v, r, i");
        return SET_PROPERTY_ERROR;
    }

    /* get the character 'v', 'r' or 'i' */
    xy_type = getStringFromStack( stackPointer )[0];

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

    status = setGraphicObjectProperty(pobjUID, __GO_TICKS_STYLE__, &ticksStyle, jni_int, 1);

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
