/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Juergen KOCH
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_sizes_property.c                                             */
/* desc : function to get in Scilab the polyline sizes                    */
/*------------------------------------------------------------------------*/

#include "MALLOC.h"
#include "SetPropertyStatus.h"
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "HandleManagement.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_sizes_property(void* _pvCtx, int iObjUID)
{
	int i = 0;
    int status = 0;
    int iSizesCount = 0;
    int* piSizesCount = &iSizesCount;
    int* piSizes = NULL;
    long* plSizes = NULL;

    getGraphicObjectProperty(iObjUID, __GO_NUM_SIZES__, jni_int, (void **)&piSizesCount);
    if (piSizesCount == NULL || iSizesCount == 0)
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    getGraphicObjectProperty(iObjUID, __GO_SIZES__, jni_int_vector, (void **)&piSizes);
    plSizes = (long*)MALLOC(iSizesCount * sizeof(long));

    for (i = 0; i < iSizesCount; ++i)
    {
        plSizes[i] = getHandle(piSizes[i]);
    }

 	status = sciReturnRowHandleVector(_pvCtx, plSizes, iSizesCount);
    FREE(plSizes);

    return status;
}
/*------------------------------------------------------------------------*/
