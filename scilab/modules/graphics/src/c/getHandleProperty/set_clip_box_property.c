/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_clip_box_property.c                                          */
/* desc : function to modify in Scilab the clip_box field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_clip_box_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[2];
    int status1 = 0;
    int status2 = 0;

    /* 2: on */
    int clipState = 2;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "clip_box");
        return SET_PROPERTY_ERROR;
    }

    /* We must have 4 elements */
    if (nbRow * nbCol != 4)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "clip_box", 4);
        return SET_PROPERTY_ERROR;
    }

    status[0] = setGraphicObjectProperty(iObjUID, __GO_CLIP_BOX__, _pvData, jni_double_vector, 4);
    status[1] = setGraphicObjectProperty(iObjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    if (status[0] == TRUE)
    {
        status1 = SET_PROPERTY_SUCCEED;
    }
    else
    {
        status1 = SET_PROPERTY_ERROR;
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "clip_box");
    }

    if (status[1] == TRUE)
    {
        status2 = SET_PROPERTY_SUCCEED;
    }
    else
    {
        status2 = SET_PROPERTY_ERROR;
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "clip_box");
    }

    return sciSetFinalStatus((SetPropertyStatus)status1, (SetPropertyStatus)status2);
}
/*------------------------------------------------------------------------*/
