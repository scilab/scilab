/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: get_x_shift_property.c                                           */
/* desc : function to retrieve in Scilab the x_shift field of a           */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_x_shift_property(void* _pvCtx, int iObjUID)
{
    double* shiftCoordinates = NULL;
    int iValue = 0;
    int* piValue = &iValue;

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_int, (void**)&piValue);

    if (piValue == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "x_shift");
        return NULL;
    }

    if (iValue == 0)
    {
        return sciReturnEmptyMatrix();
    }
    else
    {
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void **)&shiftCoordinates);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&piValue);

        return sciReturnRowVector(shiftCoordinates, iValue);
    }
}
/*------------------------------------------------------------------------*/
