/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: set_segs_color_property.c                                        */
/* desc : function to modify in Scilab the segs_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_segs_color_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int iNbSegs = 0;
    int* piNbSegs = &iNbSegs;
    int* segsColors = NULL;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "segs_color");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNbSegs);

    if (piNbSegs == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_color");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 1 && nbRow * nbCol != iNbSegs)
    {
        Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "segs_color", 1, iNbSegs);
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol == 1)
    {
        int value = (int) ((double*)_pvData)[0];

        /* 1-element array which is internally duplicated */
        status = setGraphicObjectProperty(iObjUID, __GO_SEGS_COLORS__, &value, jni_int_vector, 1);
    }
    else if (nbRow * nbCol == iNbSegs)
    {
        segsColors = (int*) MALLOC(iNbSegs * sizeof(int));

        if (segsColors == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "set_segs_colors_property");
            return SET_PROPERTY_ERROR;
        }

        copyDoubleVectorToIntFromStack(_pvData, segsColors, iNbSegs);
        status = setGraphicObjectProperty(iObjUID, __GO_SEGS_COLORS__, segsColors, jni_int_vector, iNbSegs);

        FREE(segsColors);
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_colors");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
