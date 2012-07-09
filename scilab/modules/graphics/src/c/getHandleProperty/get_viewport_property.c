/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
int get_viewport_property(void* _pvCtx, char* pobjUID)
{
    int* viewport = NULL;
    getGraphicObjectProperty(pobjUID, __GO_VIEWPORT__, jni_int_vector, (void **)&viewport);

    if (viewport == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "viewport") ;
        return -1 ;
    }

    return sciReturnRowIntVector(_pvCtx, viewport , 2);
}
/*------------------------------------------------------------------------*/
