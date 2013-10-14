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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_rotation_style_property.c                                    */
/* desc : function to modify in Scilab the rotation_style field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "stricmp.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "MALLOC.h"
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
