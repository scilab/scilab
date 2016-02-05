/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: set_x_shift_property.c                                           */
/* desc : function to modify in Scilab the x_shift field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "sci_malloc.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_x_shift_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL result = FALSE;
    double* shiftCoordinates = NULL;
    int nbElement = nbRow * nbCol;
    int iNumElements = 0;
    int* piNumElements = &iNumElements;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "x_shift");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow > 1 && nbCol > 1)
    {
        Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "x_shift", "0x0, 1xn, nx1");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&piNumElements);

    if (piNumElements == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "x_shift");
        return SET_PROPERTY_ERROR;
    }

    if (nbElement != 0 && nbElement != iNumElements) /* we can specify [] (null vector) to reset to default */
    {
        Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "x_shift", 0, iNumElements);
        return SET_PROPERTY_ERROR;
    }

    if (nbElement != 0)
    {
        shiftCoordinates = (double*)_pvData;

        result = setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, shiftCoordinates, jni_double_vector, iNumElements);

        /* The FALSE value is used for now to identify a failed memory allocation */
        if (result == FALSE)
        {
            Scierror(999, _("%s: No more memory.\n"), "set_x_shift_property");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        /*
         * Setting the shift flag to 0 directly in the model
         * when filling the shift coordinates array (0-element case)
         * would probably be better.
         */
        int shiftSet = 0;
        setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, &shiftSet, jni_double_vector, 1);
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
