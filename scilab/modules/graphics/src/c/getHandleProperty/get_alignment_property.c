/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: get_alignment_property.c                                         */
/* desc : function to retrieve in Scilab the alignment field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_alignment_property(void* _pvCtx, char* pobjUID)
{
    int iAlignment = 0;
    int* piAlignment = &iAlignment;

    getGraphicObjectProperty(pobjUID, __GO_ALIGNMENT__, jni_int, (void **)&piAlignment);

    if (piAlignment == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"alignment");
        return -1;
    }

    if (iAlignment == 0)
    {
        return sciReturnString(_pvCtx, "left");
    }
    else if (iAlignment == 1)
    {
        return sciReturnString(_pvCtx, "center");
    }
    else if (iAlignment == 2)
    {
        return sciReturnString(_pvCtx, "right");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"),"alignment");
        return -1;
    }

}
/*------------------------------------------------------------------------*/
