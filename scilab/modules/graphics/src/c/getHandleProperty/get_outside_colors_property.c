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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*------------------------------------------------------------------------*/
/* file: get_outside_colors_property.c                                    */
/* desc : function to retrieve in Scilab the outside_colors field of      */
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
int get_outside_colors_property(void* _pvCtx, char* pobjUID)
{
    int* colors = NULL;

    getGraphicObjectProperty(pobjUID, __GO_OUTSIDE_COLOR__, jni_int_vector, (void **)&colors);

    if (colors == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"outside_colors");
        return -1;
    }

    return sciReturnRowVectorFromInt(_pvCtx, colors, 2);
}
/*------------------------------------------------------------------------*/
