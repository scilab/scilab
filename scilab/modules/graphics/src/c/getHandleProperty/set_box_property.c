/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_box_property.c                                               */
/* desc : function to modify in Scilab the box field of                   */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_box_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int type = -1;
    int *piType = &type;

    if ( !( valueType == sci_strings ) )
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "box");
        return SET_PROPERTY_ERROR ;
    }

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

    /*
     * Required since the Box property is implemented differently for the Axes and Text
     * objects (respectively as an Integer and a Boolean).
     * To be corrected
     */
    if (type == __GO_AXES__)
    {
        int boxType;

        if ( isStringParamEqual( stackPointer, "off" ) )
        {
            boxType = 0;
        }
        else if ( isStringParamEqual( stackPointer, "on" ) )
        {
            boxType = 1;
        }
        else if ( isStringParamEqual( stackPointer, "hidden_axes" ) )
        {
            boxType = 2;
        }
        else if ( isStringParamEqual( stackPointer, "back_half" ) )
        {
            boxType = 3;
        }
        else if ( isStringParamEqual( stackPointer, "hidden_axis" ) )
        {
            sciprint(_("WARNING !!!\nIn '%s' property: '%s' is deprecated use '%s' instead.\n"), "box", "hidden_axis", "hidden_axes");
            boxType = 2;
        }
        else
        {
            Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "box", "on, off, hidden_axes, back_half");
            return SET_PROPERTY_ERROR;
        }

        status = setGraphicObjectProperty(pobjUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "box");
            return SET_PROPERTY_ERROR;
        }

    }
    else if (type == __GO_TEXT__)
    {
        int box;

        if ( isStringParamEqual( stackPointer, "on" ) )
        {
            box = 1;
        }
        else if ( isStringParamEqual( stackPointer, "off" ) )
        {
            box = 0;
        }
        else
        {
            Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "box", "on", "off");
            return SET_PROPERTY_ERROR;
        }

        status = setGraphicObjectProperty(pobjUID, __GO_BOX__, &box, jni_bool, 1);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "box");
            return SET_PROPERTY_ERROR;
        }

    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "box");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
