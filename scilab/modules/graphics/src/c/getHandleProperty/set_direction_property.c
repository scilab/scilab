/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_direction_property.c                                         */
/* desc : function to modify in Scilab the direction field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"


#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_direction_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int type = -1;
    int *piType = &type;
    BOOL status = FALSE;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_LIGHT__)
    {
        if (valueType != sci_matrix)
        {
            Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "direction");
            return SET_PROPERTY_ERROR;
        }

        if (nbRow * nbCol != 3)
        {
            Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "direction", 3);
            return SET_PROPERTY_ERROR;
        }

        status = setGraphicObjectProperty(iObjUID, __GO_DIRECTION__, _pvData, jni_double_vector, 3);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "direction");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "direction");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
