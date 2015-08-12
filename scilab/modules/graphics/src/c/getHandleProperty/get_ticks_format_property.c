/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: get_ticks_format_property.c                                      */
/* desc : function to retrieve in Scilab the ticks_format field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_ticks_format_property(void* _pvCtx, int iObjUID)
{
    int const propr[3] = {__GO_X_AXIS_FORMAT__, __GO_Y_AXIS_FORMAT__, __GO_Z_AXIS_FORMAT__};
    char * fmts[3]  = { NULL, NULL, NULL };
    int i = 0;

    for (i = 0 ; i < 3 ; i++)
    {
        getGraphicObjectProperty(iObjUID, propr[i], jni_string, (void **)(fmts + i));

        if (fmts[i] == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "ticks_format");
            return -1;
        }
    }

    return sciReturnRowStringVector(_pvCtx, fmts, 3);
}
/*------------------------------------------------------------------------*/
