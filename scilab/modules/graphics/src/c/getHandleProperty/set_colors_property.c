/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
/* file: set_colors_property.c                                            */
/* desc : function to modify in Scilab he polylines colors                */
/*------------------------------------------------------------------------*/

#include "sci_malloc.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_colors_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int iNumElements = 0;
    int* piNumElements = &iNumElements;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "colors");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void **) &piNumElements);

    /*
     * A way to display a more explicit message would be to first get the
     * interpolation vector set flag and test it for NULL.
     */
    if (piNumElements == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data");
        return SET_PROPERTY_ERROR;
    }

    if (nbCol == 0)
    {
        int colorSet = 0;
        status = setGraphicObjectProperty(iObjUID, __GO_COLOR_SET__, &colorSet, jni_bool, 1);
        if (status == FALSE)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "colors");
            return SET_PROPERTY_ERROR;
        }
        setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_COLORS__, NULL, jni_int_vector, 0);

        return SET_PROPERTY_SUCCEED;
    }

    if (nbCol == iNumElements)
    {
        int * tmp = MALLOC(nbCol * sizeof(int));
        copyDoubleVectorToIntFromStack(_pvData, tmp, nbCol);

        status = setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_COLORS__, tmp, jni_int_vector, nbCol);
        if (status == TRUE)
        {
            int colorSet = 1;
            setGraphicObjectProperty(iObjUID, __GO_COLOR_SET__, &colorSet, jni_bool, 1);
            FREE(tmp);
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            FREE(tmp);
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "colors");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        Scierror(999, _("The number of column of the color vector must match the number of points defining the line.\n"));
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
