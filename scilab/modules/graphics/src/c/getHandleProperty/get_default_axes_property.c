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
/* file: get_default_axes_property.c                                      */
/* desc : function to retrieve in Scilab the default_axes field of a      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "InitObjects.h"
#include "sci_malloc.h"
#include "HandleManagement.h"
#include "AxesModel.h"
/*--------------------------------------------------------------------------*/
void* get_default_axes_property(void* _pvCtx, int iObjUID)
{

    if (iObjUID != 0)
    {
        /* This property exists for figures since Scilab 5.5.0 */

        int iVisible = 0;
        int* piVisible = &iVisible;
        getGraphicObjectProperty(iObjUID, __GO_DEFAULT_AXES__, jni_bool, (void **)&piVisible);

        if (piVisible == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "default_axes");
            return FALSE;
        }

        if (iVisible == 0)
        {
            return sciReturnString("off");
        }
        else
        {
            return sciReturnString("on");
        }
    }

    /* Return default axes (gda()) */
    return sciReturnHandle(getHandle(getAxesModel()));
}
/*--------------------------------------------------------------------------*/
