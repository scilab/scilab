/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_datatips_property.c                                          */
/* desc : function to retrieve in Scilab the datatips polyline's          */
/*        field handle                                                    */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "getConsoleIdentifier.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

int get_datatips_property(void* _pvCtx, int iObjUID)
{
    int i = 0;
    int status = 0;
    int iDatatipCount = 0;
    int* piDatatipCount = &iDatatipCount;
    int* piDatatip = NULL;
    long* plDatatip = NULL;

    getGraphicObjectProperty(iObjUID, __GO_DATATIPS_COUNT__, jni_int, (void **)&piDatatipCount);
    if (piDatatipCount == NULL || iDatatipCount == 0)
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    getGraphicObjectProperty(iObjUID, __GO_DATATIPS__, jni_int_vector, (void **)&piDatatip);
    plDatatip = (long*)MALLOC(iDatatipCount * sizeof(long));

    for (i = 0; i < iDatatipCount; ++i)
    {
        plDatatip[i] = getHandle(piDatatip[i]);
    }

    status = sciReturnColHandleVector(_pvCtx, plDatatip, iDatatipCount);
    FREE(plDatatip);

    return status;
}
