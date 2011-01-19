/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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

/*------------------------------------------------------------------------*/
int get_links_property( sciPointObj * pobj )
{
    int nbLegends;
    int i;
    long *handles;
    char** links;
    int status;
    sciPointObj* pLinks;
    int iLinksCount = 0;
    int* piLinksCount = &iLinksCount;

    getGraphicObjectProperty(pobj->UID, __GO_LINKS_COUNT__, jni_int, &piLinksCount);

    if (piLinksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        return -1;
    }

    handles = (long *)MALLOC(iLinksCount*sizeof(long));
    if (handles==NULL)
    {
        Scierror(999, _("%s: No more memory.\n"),"get_links_property");
        return -1;
    }

    getGraphicObjectProperty(pobj->UID, __GO_LINKS__, jni_string_vector, &links);

    if (links == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "links");
        return -1;
    }

    /*
     * Temporary, as sciPointObj structures should not be allocated.
     * To be modified
     */
    for (i = 0; i < iLinksCount; i++)
    {
        pLinks = MALLOC(sizeof(sciPointObj));
        pLinks->UID = links[i];

        sciAddNewHandle(pLinks);
        handles[i] = sciGetHandle(pLinks);
    }

    status = sciReturnRowHandleVector(handles, iLinksCount);

    FREE(handles);

    return status;
}
/*------------------------------------------------------------------------*/
