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
/* file: get_interp_color_vector_property.c                               */
/* desc : function to retrieve in Scilab the interp_color_vector field of */
/*        a handle.                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int get_interp_color_vector_property(void* _pvCtx, int iObjUID)
{
    int* piInterpVector = NULL;
    int iInterpVectorSet = 0;
    int *piInterpVectorSet = &iInterpVectorSet;
    int iNumElements = 0;
    int *piNumElements = &iNumElements;

    getGraphicObjectProperty(iObjUID, __GO_INTERP_COLOR_VECTOR_SET__, jni_bool, (void **)&piInterpVectorSet);

    if (piInterpVectorSet == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "interp_color_vector");
        return -1;
    }

    if (iInterpVectorSet == FALSE)
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }
    else
    {
        getGraphicObjectProperty(iObjUID, __GO_INTERP_COLOR_VECTOR__, jni_int_vector, (void **) &piInterpVector);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void **) &piNumElements);

        return sciReturnRowVectorFromInt(_pvCtx, piInterpVector, iNumElements);
    }
}
/*--------------------------------------------------------------------------*/
