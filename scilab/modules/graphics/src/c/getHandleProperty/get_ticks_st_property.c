/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
void* get_ticks_st_property(void* _pvCtx, int iObjUID)
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
            return NULL;
        }
        factors[2 * i] = dbls[0];
        factors[2 * i + 1] = dbls[1];
    }

    return sciReturnMatrix(factors, 2, 3);
}
/*------------------------------------------------------------------------*/
