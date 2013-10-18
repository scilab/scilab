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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_y_label_property.c                                           */
/* desc : function to modify in Scilab the y_label field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_y_label_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_AXES__)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "y_label");
        return SET_PROPERTY_ERROR;
    }

    Scierror(999, _("Can not set directly a label object.\n"));
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
