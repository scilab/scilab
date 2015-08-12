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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_z_label_property.c                                           */
/* desc : function to retrieve in Scilab the z_label field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_z_label_property(void* _pvCtx, int iObjUID)
{
    int iLabelUID = 0;
    int* piLabelUID = &iLabelUID;

    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_LABEL__, jni_int, (void **) &piLabelUID);

    if (iLabelUID == 0)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "z_label");
        return -1;
    }

    return sciReturnHandle(_pvCtx, getHandle(iLabelUID));
}
/*------------------------------------------------------------------------*/
