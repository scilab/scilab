/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
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
/* file: get_color_range_property.c                                       */
/* desc : function to retrieve in Scilab the color_range field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_color_range_property(void* _pvCtx, int iObjUID)
{
    int* range = NULL;

    getGraphicObjectProperty(iObjUID, __GO_COLOR_RANGE__, jni_int_vector, (void **)&range);

    if (range == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "color_range");
        return -1;
    }

    return sciReturnRowVectorFromInt(_pvCtx, range, 2);
}
/*------------------------------------------------------------------------*/
