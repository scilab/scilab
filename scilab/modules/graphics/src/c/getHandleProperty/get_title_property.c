/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: get_title_property.c                                             */
/* desc : function to retrieve in Scilab the title field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_title_property(void* _pvCtx, char* pobjUID)
{
    char* labelUID = NULL;
    long labelHandle = 0;

    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_AXES__)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "title");
        return -1;
    }

    getGraphicObjectProperty(pobjUID, __GO_TITLE__, jni_string, (void **)&labelUID);

    if (labelUID == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "title");
        return -1;
    }

    labelHandle = getHandle(labelUID);

    return sciReturnHandle(_pvCtx, labelHandle);
}
/*------------------------------------------------------------------------*/
