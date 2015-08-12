/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_x_location_property.c                                        */
/* desc : function to retrieve in Scilab the x_location field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_x_location_property(void* _pvCtx, int iObjUID)
{
    int iLocation = 0;
    int* piLocation = &iLocation;

    getGraphicObjectProperty(iObjUID, __GO_X_AXIS_LOCATION__, jni_int, (void**)&piLocation);

    if (piLocation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "x_location");
        return -1;
    }

    if (iLocation == 0)
    {
        return sciReturnString(_pvCtx, "bottom");
    }
    else if (iLocation == 1)
    {
        return sciReturnString(_pvCtx, "top");
    }
    else if (iLocation == 2)
    {
        return sciReturnString(_pvCtx, "middle");
    }
    else if (iLocation == 3)
    {
        return sciReturnString(_pvCtx, "origin");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"), "x_location");
        return -1;
    }

}
/*------------------------------------------------------------------------*/
