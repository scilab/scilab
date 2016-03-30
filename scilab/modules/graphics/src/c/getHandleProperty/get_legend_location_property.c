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
void* get_legend_location_property(void* _pvCtx, int iObjUID)
{
    int iLegendLocation = 0;
    int* piLegendLocation = &iLegendLocation;

    getGraphicObjectProperty(iObjUID, __GO_LEGEND_LOCATION__, jni_int, (void**)&piLegendLocation);

    if (piLegendLocation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "legend_location");
        return NULL;
    }

    if (iLegendLocation == 0)
    {
        return sciReturnString("in_upper_right");
    }
    else if (iLegendLocation == 1)
    {
        return sciReturnString("in_upper_left");
    }
    else if (iLegendLocation == 2)
    {
        return sciReturnString("in_lower_right");
    }
    else if (iLegendLocation == 3)
    {
        return sciReturnString("in_lower_left");
    }
    else if (iLegendLocation == 4)
    {
        return sciReturnString("out_upper_right");
    }
    else if (iLegendLocation == 5)
    {
        return sciReturnString("out_upper_left");
    }
    else if (iLegendLocation == 6)
    {
        return sciReturnString("out_lower_right");
    }
    else if (iLegendLocation == 7)
    {
        return sciReturnString("out_lower_left");
    }
    else if (iLegendLocation == 8)
    {
        return sciReturnString("upper_caption");
    }
    else if (iLegendLocation == 9)
    {
        return sciReturnString("lower_caption");
    }
    else if (iLegendLocation == 10)
    {
        return sciReturnString("by_coordinates");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"), "legend_location");
    }

    return NULL;
}
/*------------------------------------------------------------------------*/
