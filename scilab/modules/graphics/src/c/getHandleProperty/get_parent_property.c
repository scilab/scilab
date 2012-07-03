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
/* file: get_parent_property.c                                            */
/* desc : function to retrieve in Scilab the parent field of a            */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "HandleManagement.h"
#include "returnProperty.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_parent_property(void* _pvCtx, char* pobjUID)
{
    char* parentID = NULL;

    /* All Graphic Objects have the __GO_PARENT__ property */
    getGraphicObjectProperty(pobjUID, __GO_PARENT__, jni_string, (void **)&parentID);

    if (strcmp(parentID, "") == 0)
    {
        /* No parent for this object */
        return sciReturnEmptyMatrix(_pvCtx);
    }
    else
    {
        return sciReturnHandle(_pvCtx, getHandle(parentID));
    }
}
/*------------------------------------------------------------------------*/
