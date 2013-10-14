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
/* file: get_figure_position_property.c                                   */
/* desc : function to retrieve in Scilab the figure_position field of a   */
/*        handle                                                          */
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
int get_figure_position_property(void* _pvCtx, int iObjUID)
{
    double figurePos[2];
    int* position = NULL;
    getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_int_vector, (void **)&position);

    if (position == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "figure_position");
        return -1;
    }

    figurePos[0] = (double) position[0];
    figurePos[1] = (double) position[1];

    return sciReturnRowVector(_pvCtx, figurePos, 2);

}
/*------------------------------------------------------------------------*/
