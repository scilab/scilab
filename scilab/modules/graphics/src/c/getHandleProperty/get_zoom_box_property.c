/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_zoom_box_property.c                                          */
/* desc : function to retrieve in Scilab the zoom_box field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "axesScale.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_zoom_box_property(void* _pvCtx, int iObjUID)
{
    double dblTmp = 0;
    double* zoomBox = NULL;
    int iZoomEnabled = 0;
    int* zoomEnabled = &iZoomEnabled;

    getGraphicObjectProperty(iObjUID, __GO_ZOOM_ENABLED__, jni_bool, (void **)&zoomEnabled);

    getGraphicObjectProperty(iObjUID, __GO_ZOOM_BOX__, jni_double_vector, (void **)&zoomBox);

    if (zoomEnabled == NULL || zoomBox == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "zoom_box");
        return NULL;
    }

    if (iZoomEnabled)
    {
        // Swap zoomBox values to feat Scilab View ordering.
        // MVC stores [xmin, xmax, ymin, ymax, zmin, zmax]
        // Scilab expects [xmin, ymin, xmax, ymax, zmin, zmax]
        dblTmp = zoomBox[2];
        zoomBox[2] = zoomBox[1];
        zoomBox[1] = dblTmp;
        return sciReturnRowVector(zoomBox, 6);
    }
    else
    {
        return sciReturnEmptyMatrix();
    }
}
/*------------------------------------------------------------------------*/
