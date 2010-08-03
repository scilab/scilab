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
int get_legend_location_property( sciPointObj * pobj )
{
    int* legendLocation;

#if 0
    if (sciGetEntityType (pobj) != SCI_LEGEND)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"legend_location");
        return -1;
    }
#endif

    legendLocation = (int*) getGraphicObjectProperty(pobj->UID, __GO_LEGEND_LOCATION__, jni_int);

    if (legendLocation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"legend_location");
        return -1;
    }

    if (*legendLocation == 0)
    {
        return sciReturnString("in_upper_right");
    }
    else if (*legendLocation == 1)
    { 
        return sciReturnString("in_upper_left");
    }
    else if (*legendLocation == 2)
    {
        return sciReturnString("in_lower_right");
    }
    else if (*legendLocation == 3)
    {
        return sciReturnString("in_lower_left");
    }
    else if (*legendLocation == 4)
    {
        return sciReturnString("out_upper_right");
    }
    else if (*legendLocation == 5)
    {
        return sciReturnString("out_upper_left");
    }
    else if (*legendLocation == 6)
    {
        return sciReturnString("out_lower_right");
    }
    else if (*legendLocation == 7)
    {
        return sciReturnString("out_lower_left");
    }
    else if (*legendLocation == 8)
    {
        return sciReturnString("upper_caption");
    }
    else if (*legendLocation == 9)
    {
        return sciReturnString("lower_caption");
    }
    else if (*legendLocation == 10)
    {
        return sciReturnString("by_coordinates");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"),"legend_location");
    }

    return -1;
}
/*------------------------------------------------------------------------*/
