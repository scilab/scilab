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
/* file: get_tics_segment_property.c                                      */
/* desc : function to retrieve in Scilab the tics_segment field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_tics_segment_property(void* _pvCtx, int iObjUID)
{
    int iTicksSegment = 0;
    int* piTicksSegment = &iTicksSegment;

    getGraphicObjectProperty(iObjUID, __GO_TICKS_SEGMENT__, jni_bool, (void **)&piTicksSegment);

    if (piTicksSegment == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_segment");
        return NULL;
    }

    if (iTicksSegment)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }

}
/*------------------------------------------------------------------------*/
