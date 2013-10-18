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
/* file: get_arrow_size_factor_property.c                                 */
/* desc : function to retrieve in Scilab the arrow_size_factor field of a */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_arrow_size_factor_property(void* _pvCtx, int iObjUID)
{
    double dblArrowSizeFactor = 0;
    double* pdblArrowSizeFactor = &dblArrowSizeFactor;

    getGraphicObjectProperty(iObjUID, __GO_ARROW_SIZE_FACTOR__, jni_double, (void **)&pdblArrowSizeFactor);

    if (pdblArrowSizeFactor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "arrow_size_factor");
        return -1;
    }

    return sciReturnDouble(_pvCtx, dblArrowSizeFactor);
}
/*------------------------------------------------------------------------*/
