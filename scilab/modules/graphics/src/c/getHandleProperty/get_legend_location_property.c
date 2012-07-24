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
/* file: get_legend_location_property.c                                        */
/* desc : function to retrieve in Scilab the legend_location (place) field of          */
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
int get_legend_location_property(void* _pvCtx, char* pobjUID)
{
    int iLegendLocation = 0;
    int* piLegendLocation = &iLegendLocation;

    getGraphicObjectProperty(pobjUID, __GO_LEGEND_LOCATION__, jni_int, (void**)&piLegendLocation);

    if (piLegendLocation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"legend_location");
        return -1;
    }

    if (iLegendLocation == 0)
    {
        return sciReturnString(_pvCtx, "in_upper_right");
    }
    else if (iLegendLocation == 1)
    {
        return sciReturnString(_pvCtx, "in_upper_left");
    }
    else if (iLegendLocation == 2)
    {
        return sciReturnString(_pvCtx, "in_lower_right");
    }
    else if (iLegendLocation == 3)
    {
        return sciReturnString(_pvCtx, "in_lower_left");
    }
    else if (iLegendLocation == 4)
    {
        return sciReturnString(_pvCtx, "out_upper_right");
    }
    else if (iLegendLocation == 5)
    {
        return sciReturnString(_pvCtx, "out_upper_left");
    }
    else if (iLegendLocation == 6)
    {
        return sciReturnString(_pvCtx, "out_lower_right");
    }
    else if (iLegendLocation == 7)
    {
        return sciReturnString(_pvCtx, "out_lower_left");
    }
    else if (iLegendLocation == 8)
    {
        return sciReturnString(_pvCtx, "upper_caption");
    }
    else if (iLegendLocation == 9)
    {
        return sciReturnString(_pvCtx, "lower_caption");
    }
    else if (iLegendLocation == 10)
    {
        return sciReturnString(_pvCtx, "by_coordinates");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"),"legend_location");
    }

    return -1;
}
/*------------------------------------------------------------------------*/
