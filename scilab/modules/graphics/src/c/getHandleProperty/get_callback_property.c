/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
void* get_callback_property(void* _pvCtx, int iObjUID)
{
    char* callback = NULL;
    void* status = NULL;

    getGraphicObjectProperty(iObjUID,  __GO_CALLBACK__, jni_string, (void**) &callback);

    if (callback == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Callback");
        return FALSE;
    }
    else
    {
        status = sciReturnString(callback);
        free(callback);
        return status;
    }
}
/*------------------------------------------------------------------------*/
