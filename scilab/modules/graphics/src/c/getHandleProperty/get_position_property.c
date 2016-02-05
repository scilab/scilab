/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2013 - Pedro SOUZA
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
void* get_position_property(void* _pvCtx, int iObjUID)
{
    int iType = -1;
    int* piType = &iType;
    double* position = NULL;
    void* ret = NULL;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **) &piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "type");
        return NULL;
    }

    /* Special figure case */
    if (iType == __GO_FIGURE__)
    {
        int* figurePosition;
        int* figureSize;
        double position[4];

        getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_int_vector, (void **) &figurePosition);

        getGraphicObjectProperty(iObjUID, __GO_AXES_SIZE__, jni_int_vector, (void **) &figureSize);

        if (figurePosition == NULL || figureSize == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return NULL;
        }

        position[0] = (double) figurePosition[0];
        position[1] = (double) figurePosition[1];
        position[2] = (double) figureSize[0];
        position[3] = (double) figureSize[1];

        ret = sciReturnRowVector(position, 4);
        releaseGraphicObjectProperty(__GO_POSITION__, figurePosition, jni_int_vector, 2);
        releaseGraphicObjectProperty(__GO_AXES_SIZE__, figureSize, jni_int_vector, 2);
        return ret;
    }

    /* Special label and legend case : only 2 values for position */
    if (iType == __GO_LABEL__ || iType == __GO_LEGEND__)
    {
        double* position;

        getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void **) &position);

        if (position == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return NULL;
        }

        ret = sciReturnRowVector(position, 2);
        releaseGraphicObjectProperty(__GO_POSITION__, position, jni_double_vector, 2);
        return ret;
    }

    if (iType == __GO_LIGHT__)
    {
        double* position = NULL;

        getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void **)&position);

        if (position == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return NULL;
        }

        ret = sciReturnRowVector(position, 3);
        releaseGraphicObjectProperty(__GO_POSITION__, position, jni_double_vector, 3);
        return ret;
    }

    /* Generic case : position is a 4 row vector */

    getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void **) &position);

    if (position == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
        return NULL;
    }

    ret = sciReturnRowVector(position, 4);
    releaseGraphicObjectProperty(__GO_POSITION__, position, jni_double_vector, 4);
    return ret;
}
/*------------------------------------------------------------------------*/
