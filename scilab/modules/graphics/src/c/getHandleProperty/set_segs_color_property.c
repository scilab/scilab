/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_segs_color_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int iNbSegs = 0;
    int* piNbSegs = &iNbSegs;
    int* segsColors = NULL;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "segs_color");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(pobjUID, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNbSegs);

    if (piNbSegs == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_color");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow*nbCol != 1 && nbRow*nbCol != iNbSegs)
    {
        Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "segs_color", 1, iNbSegs);
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow * nbCol == 1 )
    {
        int value = (int) getDoubleFromStack( stackPointer );

        /* 1-element array which is internally duplicated */
        status = setGraphicObjectProperty(pobjUID, __GO_SEGS_COLORS__, &value, jni_int_vector, 1);
    }
    else if ( nbRow * nbCol == iNbSegs )
    {
        segsColors = (int*) MALLOC(iNbSegs * sizeof(int));

        if (segsColors == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "set_segs_colors_property");
            return SET_PROPERTY_ERROR;
        }

        copyDoubleVectorToIntFromStack( stackPointer, segsColors, iNbSegs);
        status = setGraphicObjectProperty(pobjUID, __GO_SEGS_COLORS__, segsColors, jni_int_vector, iNbSegs);

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
