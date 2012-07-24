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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_rotation_angles_property.c                                   */
/* desc : function to retrieve in Scilab the rotation_angles field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_rotation_angles_property(void* _pvCtx, char* pobjUID)
{
    double* angles = NULL;

    getGraphicObjectProperty(pobjUID, __GO_ROTATION_ANGLES__, jni_double_vector, (void **)&angles);

    if (angles == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"rotation_angles");
        return -1;
    }

    return sciReturnRowVector(_pvCtx, angles, 2);
}
/*------------------------------------------------------------------------*/
