/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"

/*------------------------------------------------------------------------*/
int get_links_property(void* _pvCtx, char* pobjUID)
{
    int i = 0;
    long *handles = NULL;
    char** links = NULL;
    int status = 0;
    int iLinksCount = 0;
    int* piLinksCount = &iLinksCount;

    getGraphicObjectProperty(pobjUID, __GO_LINKS_COUNT__, jni_int, (void **) &piLinksCount);

    if (piLinksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        return -1;
    }

    if (iLinksCount == 0)
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    handles = (long *)MALLOC(iLinksCount * sizeof(long));
    if (handles == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "get_links_property");
        return -1;
    }

    getGraphicObjectProperty(pobjUID, __GO_LINKS__, jni_string_vector, (void **) &links);

    if (links == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        FREE(handles);
        return -1;
    }

    for (i = 0; i < iLinksCount; i++)
    {
        handles[i] = getHandle(links[i]);
    }

    status = sciReturnRowHandleVector(_pvCtx, handles, iLinksCount);

    FREE(handles);

    return status;
}
/*------------------------------------------------------------------------*/
