/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_margins_property.c                                           */
/* desc : function to modify in Scilab the margins field of               */
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
int set_margins_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "margins");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 4)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "margins", 4);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_MARGINS__, _pvData, jni_double_vector, 4);

    if (status == TRUE)
    {
        int axes_margins_set = 0;
        setGraphicObjectProperty(iObjUID, __GO_AUTO_MARGINS__, &axes_margins_set, jni_bool, 1);
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "margins");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
