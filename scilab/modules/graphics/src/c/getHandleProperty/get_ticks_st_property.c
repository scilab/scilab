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
/* file: get_ticks_st_property.c                                          */
/* desc : function to retrieve in Scilab the ticks_st field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_ticks_st_property(void* _pvCtx, int iObjUID)
{
    int const propr[3] = {__GO_X_AXIS_ST_FACTORS__, __GO_Y_AXIS_ST_FACTORS__, __GO_Z_AXIS_ST_FACTORS__};
    double factors[] = {1., 0., 1., 0., 1., 0.};
    int i = 0;

    for (i = 0 ; i < 3 ; i++)
    {
        double * dbls = NULL;
        getGraphicObjectProperty(iObjUID, propr[i], jni_double_vector, (void **)&dbls);
        if (dbls == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "ticks_st");
            return -1;
        }
        factors[2 * i] = dbls[0];
        factors[2 * i + 1] = dbls[1];
    }

    return sciReturnMatrix(_pvCtx, factors, 2, 3);
}
/*------------------------------------------------------------------------*/
