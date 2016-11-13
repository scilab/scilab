/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
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
/* file: set_outside_colors_property.c                                     */
/* desc : function to modify in Scilab the outside_colors field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "Sciwarning.h"

/*------------------------------------------------------------------------*/
int set_outside_colors_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int  values[2];
    int nbColors = 0;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "outside_colors");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 2)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "outside_colors", 2);
        return SET_PROPERTY_ERROR;
    }

    copyDoubleVectorToIntFromStack(_pvData, values, 2);

    /* Returns the number of colors of pobj's parent Figure */
    nbColors = sciGetNumColors(iObjUID);

    if (  values[0] > nbColors || values[0] < -1
            || values[1] > nbColors || values[1] < -1)
    {
        /* It is possible to set color_range outside the colormap, however it won't be used.*/
        Sciwarning(_("WARNING: Wrong value for '%s' property: indices outside the colormap will be clamped.\n"), "outside_colors");
    }

    status = setGraphicObjectProperty(iObjUID, __GO_OUTSIDE_COLOR__, values, jni_int_vector, 2);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "outside_colors");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
