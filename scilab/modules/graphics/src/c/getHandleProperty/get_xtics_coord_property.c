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
/* file: get_xtics_coord_property.c                                       */
/* desc : function to retrieve in Scilab the xtics_coord field of         */
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
int get_xtics_coord_property(void* _pvCtx, int iObjUID)
{
    int iXNumberTicks = 0;
    int* xNumberTicks = &iXNumberTicks;
    double* xTicksCoords = NULL;

    getGraphicObjectProperty(iObjUID, __GO_X_TICKS_COORDS__, jni_double_vector, (void **)&xTicksCoords);

    if (xTicksCoords == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "xtics_coord");
        return -1;
    }

    getGraphicObjectProperty(iObjUID, __GO_X_NUMBER_TICKS__, jni_int, (void**)&xNumberTicks);
    if (xNumberTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "xtics_coord");
        return -1;
    }

    return sciReturnRowVector(_pvCtx, xTicksCoords, *xNumberTicks);
}
/*------------------------------------------------------------------------*/
