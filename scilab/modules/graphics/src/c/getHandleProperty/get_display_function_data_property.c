/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_display_function_data_property(void* _pvCtx, int iObjUID)
{
    int iDisplayDataSize = 0;
    int *piDisplayDataSize = &iDisplayDataSize;
    int *piDisplayData = NULL;

    int status = 0;

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_DISPLAY_FUNCTION_SIZE__, jni_int, (void **)&piDisplayDataSize);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_DISPLAY_FUNCTION__, jni_int_vector, (void **)&piDisplayData);

    if ((piDisplayData == NULL) || (piDisplayDataSize == NULL))
    {
        status = sciReturnEmptyMatrix(_pvCtx);
    }
    else
    {
        status = sciReturnUserData(_pvCtx, piDisplayData, iDisplayDataSize);
    }

    return status;
}

/*------------------------------------------------------------------------*/
