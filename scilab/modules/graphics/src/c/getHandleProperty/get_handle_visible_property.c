/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_handle_visible_property.c                                    */
/* desc : function to retrieve in Scilab the handle_visible field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_handle_visible_property(void* _pvCtx, char* pobjUID)
{
    int handleVisible = 0;
    int *piHandleVisible = &handleVisible;

    getGraphicObjectProperty(pobjUID, __GO_HIDDEN__, jni_bool, (void **)&piHandleVisible);

    if (piHandleVisible == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "handle_visible");
        return FALSE;
    }

    if (1 - handleVisible)      /* Handle visible is equivalent to not hidden */
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}

/*------------------------------------------------------------------------*/
