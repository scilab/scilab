/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
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
/* file: get_datatips_property.c                                          */
/* desc : function to retrieve in Scilab the datatips polyline's          */
/*        field handle                                                    */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "getConsoleIdentifier.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

void* get_datatips_property(void* _pvCtx, int iObjUID)
{
    int i = 0;
    void* status = 0;
    int iDatatipCount = 0;
    int* piDatatipCount = &iDatatipCount;
    int* piDatatip = NULL;
    long* plDatatip = NULL;

    getGraphicObjectProperty(iObjUID, __GO_DATATIPS_COUNT__, jni_int, (void **)&piDatatipCount);
    if (piDatatipCount == NULL || iDatatipCount == 0)
    {
        return sciReturnEmptyMatrix();
    }

    getGraphicObjectProperty(iObjUID, __GO_DATATIPS__, jni_int_vector, (void **)&piDatatip);
    plDatatip = (long*)MALLOC(iDatatipCount * sizeof(long));

    for (i = 0; i < iDatatipCount; ++i)
    {
        plDatatip[i] = getHandle(piDatatip[i]);
    }

    status = sciReturnColHandleVector(plDatatip, iDatatipCount);
    FREE(plDatatip);

    return status;
}
