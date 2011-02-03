/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_y_shift_property.c                                           */
/* desc : function to retrieve in Scilab the y_shift field of a           */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_y_shift_property( sciPointObj * pobj )
{
    double* shiftCoordinates;
    int iSize = 0;
    int* piSize = &iSize;

    getGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_int, &piSize);

    if (piSize == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"y_shift");
        return -1;
    }

    if (iSize == 0)
    {
        return sciReturnEmptyMatrix();
    }
    else
    {
        getGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, &shiftCoordinates);
        getGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, &piSize);

        return sciReturnRowVector(shiftCoordinates, iSize);
    }
}
/*------------------------------------------------------------------------*/
