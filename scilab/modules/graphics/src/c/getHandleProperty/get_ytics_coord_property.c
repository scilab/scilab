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
/* file: get_ytics_coord_property.c                                       */
/* desc : function to retrieve in Scilab the ytics_coord field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_ytics_coord_property(void* _pvCtx, int iObjUID)
{
    int iYNumberTicks = 0;
    int* piYNumberTicks = &iYNumberTicks;
    double* yTicksCoords = NULL;

    getGraphicObjectProperty(iObjUID, __GO_Y_TICKS_COORDS__, jni_double_vector, (void **)&yTicksCoords);

    if (yTicksCoords == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ytics_coord");
        return NULL;
    }

    getGraphicObjectProperty(iObjUID, __GO_Y_NUMBER_TICKS__, jni_int, (void**)&piYNumberTicks);
    if (piYNumberTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ytics_coord");
        return NULL;
    }

    return sciReturnRowVector(yTicksCoords, iYNumberTicks);
}
/*------------------------------------------------------------------------*/
