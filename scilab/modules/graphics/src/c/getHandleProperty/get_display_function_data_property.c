/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_display_function_data_property(void* _pvCtx, int iObjUID)
{
    int iDisplayDataSize = 0;
    int *piDisplayDataSize = &iDisplayDataSize;
    int *piDisplayData = NULL;

    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_DISPLAY_FUNCTION_SIZE__, jni_int, (void **)&piDisplayDataSize);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_DISPLAY_FUNCTION__, jni_int_vector, (void **)&piDisplayData);

    if ((piDisplayData == NULL) || (piDisplayDataSize == NULL))
    {
        status = sciReturnEmptyMatrix();
    }
    else
    {
        status = sciReturnUserData(piDisplayData, iDisplayDataSize);
    }

    return status;
}

/*------------------------------------------------------------------------*/
