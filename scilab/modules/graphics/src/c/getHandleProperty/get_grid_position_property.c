/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
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
/* file: get_grid_position_property.c                                     */
/* desc : function to retrieve in Scilab the grid_position field of a     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_grid_position_property(void* _pvCtx, int iObjUID)
{
    int iGridPosition = 0;
    int* piGridPosition = &iGridPosition;

    getGraphicObjectProperty(iObjUID, __GO_GRID_POSITION__, jni_int, (void **)&piGridPosition);

    if (piGridPosition == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_position");
        return NULL;
    }

    if (iGridPosition)
    {
        return sciReturnString("foreground");
    }
    else
    {
        return sciReturnString("background");
    }
}
/*------------------------------------------------------------------------*/
