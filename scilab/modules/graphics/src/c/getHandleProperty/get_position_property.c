/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_position_property.c                                          */
/* desc : function to retrieve in Scilab the position field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_position_property(void* _pvCtx, char* pobjUID)
{
    int iType = -1;
    int* piType = &iType;
    double* position = NULL;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **) &piType);

    /* Special figure case */
    if (iType == __GO_FIGURE__)
    {
        int* figurePosition;
        int* figureSize;
        double position[4];

        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_int_vector, (void **) &figurePosition);

        getGraphicObjectProperty(pobjUID, __GO_AXES_SIZE__, jni_int_vector, (void **) &figureSize);

        if (figurePosition == NULL || figureSize == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return -1;
        }

        position[0] = (double) figurePosition[0];
        position[1] = (double) figurePosition[1];
        position[2] = (double) figureSize[0];
        position[3] = (double) figureSize[1];

        return sciReturnRowVector(_pvCtx, position, 4);
    }

    /* Special label and legend case : only 2 values for position */
    if (iType == __GO_LABEL__ || iType == __GO_LEGEND__)
    {
        double* position;

        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **) &position);

        if (position == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return -1;
        }

        return sciReturnRowVector(_pvCtx, position, 2);
    }

    /* Generic case : position is a 4 row vector */

    getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **) &position);

    if (position == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
        return -1;
    }

    return sciReturnRowVector(_pvCtx, position, 4);
}
/*------------------------------------------------------------------------*/
