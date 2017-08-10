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
/* file: set_interp_color_mode_property.c                                 */
/* desc : function to modify in Scilab the interp_color_mode field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

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
int set_interp_color_mode_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int b =  (int)FALSE;
    int iInterpColorVectorSet = 0;
    int* piInterpColorVectorSet = &iInterpColorVectorSet;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "interp_color_mode");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    if (b == TRUE)
    {
        getGraphicObjectProperty(iObjUID, __GO_INTERP_COLOR_VECTOR_SET__, jni_bool, (void **)&piInterpColorVectorSet);

        if (piInterpColorVectorSet == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "interp_color_mode");
            return SET_PROPERTY_ERROR;
        }

        if (iInterpColorVectorSet  == FALSE)
        {
            Scierror(999, _("You must first specify an %s for this object.\n"), "interp_color_vector");
            return SET_PROPERTY_ERROR;
        }
    }

    status = setGraphicObjectProperty(iObjUID, __GO_INTERP_COLOR_MODE__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "interp_color_mode");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
