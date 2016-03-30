/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
/* file: get_viewport_property.c                                          */
/* desc : function to retrieve in Scilab the viewport field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_viewport_property(void* _pvCtx, int iObjUID)
{
    int* viewport = NULL;
    getGraphicObjectProperty(iObjUID, __GO_VIEWPORT__, jni_int_vector, (void **)&viewport);

    if (viewport == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "viewport");
        return NULL;
    }

    return sciReturnRowIntVector(viewport , 2);
}
/*------------------------------------------------------------------------*/
