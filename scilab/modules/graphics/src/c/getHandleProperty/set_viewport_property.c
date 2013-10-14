/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_viewport_property.c                                          */
/* desc : function to modify in Scilab the auto_rotation field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_viewport_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int values[4];
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "viewport");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 2)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "viewport", 2);
        return SET_PROPERTY_ERROR;
    }

    /* For now we just use viewport positions */
    copyDoubleVectorToIntFromStack(_pvData, values, 2);

    /* dummy values */
    values[2] = 0;
    values[3] = 0;

    status = setGraphicObjectProperty(iObjUID, __GO_VIEWPORT__, values, jni_int_vector, 2);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "viewport");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
