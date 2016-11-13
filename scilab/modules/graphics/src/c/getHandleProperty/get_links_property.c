/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: get_link_property.c                                              */
/* desc : function to retrieve in Scilab the  handles used by Legend      */
/*        to assign polyline styles                                       */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"

/*------------------------------------------------------------------------*/
void* get_links_property(void* _pvCtx, int iObjUID)
{
    int i = 0;
    long *handles = NULL;
    int* links = NULL;
    void* status = 0;
    int iLinksCount = 0;
    int* piLinksCount = &iLinksCount;

    getGraphicObjectProperty(iObjUID, __GO_LINKS_COUNT__, jni_int, (void **) &piLinksCount);

    if (piLinksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        return NULL;
    }

    if (iLinksCount == 0)
    {
        return sciReturnEmptyMatrix();
    }

    handles = (long *)MALLOC(iLinksCount * sizeof(long));
    if (handles == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "get_links_property");
        return NULL;
    }

    getGraphicObjectProperty(iObjUID, __GO_LINKS__, jni_int_vector, (void **) &links);

    if (links == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        FREE(handles);
        return NULL;
    }

    for (i = 0; i < iLinksCount; i++)
    {
        handles[i] = getHandle(links[i]);
    }

    status = sciReturnRowHandleVector(handles, iLinksCount);
    FREE(handles);

    return status;
}
/*------------------------------------------------------------------------*/
