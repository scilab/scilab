/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
/* file: get_anti_aliasing_property.c                                     */
/* desc : function to retrieve in Scilab the anti_aliasing field of a     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_anti_aliasing_property(void* _pvCtx, int iObjUID)
{
    int iAntialiasing = 0;
    int* piAntialiasing = &iAntialiasing;
    getGraphicObjectProperty(iObjUID, __GO_ANTIALIASING__, jni_int, (void **)&piAntialiasing);

    if (piAntialiasing == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "anti_aliasing");
        return -1;
    }

    switch (iAntialiasing)
    {
        case 0:
            return sciReturnString(_pvCtx, "off");
            break;
        case 1:
            return sciReturnString(_pvCtx, "2x");
            break;
        case 2:
            return sciReturnString(_pvCtx, "4x");
            break;
        case 3:
            return sciReturnString(_pvCtx, "8x");
            break;
        case 4:
            return sciReturnString(_pvCtx, "16x");
            break;
        default:
            Scierror(999, _("Wrong value for '%s' property.\n"), "anti_aliasing");
            return -1;
            break;
    }

    return sciReturnString(_pvCtx, "off");
}
/*------------------------------------------------------------------------*/
