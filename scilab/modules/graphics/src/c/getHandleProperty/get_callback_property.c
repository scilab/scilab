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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_callback_property.c                                          */
/* desc : function to retrieve in Scilab the callback field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_callback_property(void* _pvCtx, int iObjUID)
{
    char* callback = NULL;
    int status = 0;

    getGraphicObjectProperty(iObjUID,  __GO_CALLBACK__, jni_string, (void**) &callback);

    if (callback == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Callback");
        return FALSE;
    }
    else
    {
        status = sciReturnString(_pvCtx, callback);
        free(callback);
        return status;
    }
}
/*------------------------------------------------------------------------*/
