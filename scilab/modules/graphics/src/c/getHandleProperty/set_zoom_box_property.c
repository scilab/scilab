/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: set_zoom_box_property.c                                          */
/* desc : function to modify in Scilab the zoom_box field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "axesScale.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_zoom_box_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iType = -1;
    int *piType = &iType;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "zoom_box");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_AXES__)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "zoom_box");
        return SET_PROPERTY_ERROR;
    }

    /* We must have a 4x1 matrix */
    if (nbRow * nbCol == 6)
    {
        return sciZoom3D(iObjUID, (double*)_pvData);
    }
    else if (nbRow * nbCol == 4)
    {
        return sciZoom2D(iObjUID, (double*)_pvData);
    }
    else if (nbCol * nbRow == 0)
    {
        sciUnzoomSubwin(iObjUID);
    }
    else
    {
        Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "zoom_box", "0x0, 2x2, 2x3");
        return SET_PROPERTY_ERROR;
    }
    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
