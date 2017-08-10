/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: get_y_location_property.c                                        */
/* desc : function to retrieve in Scilab the y_location field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_y_location_property(void* _pvCtx, int iObjUID)
{
    int iLocation = 0;
    int* piLocation = &iLocation;

    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LOCATION__, jni_int, (void**)&piLocation);

    if (piLocation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "y_location");
        return NULL;
    }

    if (iLocation == 4)
    {
        return sciReturnString("left");
    }
    else if (iLocation == 5)
    {
        return sciReturnString("right");
    }
    else if (iLocation == 2)
    {
        return sciReturnString("middle");
    }
    else if (iLocation == 3)
    {
        return sciReturnString("origin");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"), "y_location");
        return NULL;
    }

}
/*------------------------------------------------------------------------*/
