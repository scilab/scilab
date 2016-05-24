/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
void* get_anti_aliasing_property(void* _pvCtx, int iObjUID)
{
    int iAntialiasing = 0;
    int* piAntialiasing = &iAntialiasing;
    getGraphicObjectProperty(iObjUID, __GO_ANTIALIASING__, jni_int, (void **)&piAntialiasing);

    if (piAntialiasing == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "anti_aliasing");
        return NULL;
    }

    switch (iAntialiasing)
    {
        case 0:
            return sciReturnString("off");
        case 1:
            return sciReturnString("2x");
        case 2:
            return sciReturnString("4x");
        case 3:
            return sciReturnString("8x");
        case 4:
            return sciReturnString("16x");
        default:
            Scierror(999, _("Wrong value for '%s' property.\n"), "anti_aliasing");
            return NULL;
    }
}
/*------------------------------------------------------------------------*/
