/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: set_links_property.c                                             */
/* desc : function to modify in Scilab the related polyline to a legend.  */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "sci_malloc.h"
#include "sci_types.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_links_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int type = -1;
    int *piType = &type;
    int iParentAxes = 0;
    int* piParentAxes = &iParentAxes;
    int* links = NULL;
    int i = 0;
    int iLinksCount = 0;
    int* piLinksCount = &iLinksCount;

    if (valueType != sci_handles)
    {
        Scierror(999, _("Wrong type for '%s' property: Graphic handle array expected.\n"), "links");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_LINKS_COUNT__, jni_int, (void**)&piLinksCount);

    if (piLinksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != iLinksCount)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "links", iLinksCount);
        return SET_PROPERTY_ERROR;
    }

    links = (int*) MALLOC(iLinksCount * sizeof(int));

    if (links == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "set_z_ticks_property");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piParentAxes);

    status = TRUE;

    for (i = 0 ; i < iLinksCount ; i++)
    {
        int iPolylineParentAxes;
        int* piPoly = &iPolylineParentAxes;
        int iPolylineObjectUID = getObjectFromHandle((long)((long long*)_pvData)[i]);

        getGraphicObjectProperty(iPolylineObjectUID, __GO_TYPE__, jni_int, (void **)&piType);

        if (type != __GO_POLYLINE__)
        {
            Scierror(999, _("%s: Input argument #%d must be a '%s' handle.\n"), "links", i, "polyline");
            status = FALSE;
            break;
        }

        links[i] = iPolylineObjectUID;

        getGraphicObjectProperty(iPolylineObjectUID, __GO_PARENT_AXES__, jni_int, (void **)&piPoly);

        if (iPolylineParentAxes != iParentAxes)
        {
            Scierror(999, _("%s: Input argument and the legend must have the same parent axes.\n"), "links");
            status = FALSE;
            break;
        }
    }

    if (status == FALSE)
    {
        FREE(links);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_LINKS__, links, jni_int_vector, iLinksCount);

    FREE(links);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
