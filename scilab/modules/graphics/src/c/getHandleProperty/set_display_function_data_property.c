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

#include "setHandleProperty.h"
#include "getHandleProperty.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "api_scilab.h"
/*------------------------------------------------------------------------*/
int set_display_function_data_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    void* pPrevious = NULL;
    //temporary, try to write address of user_data in int array
    int iSize = sizeof(void*) / sizeof(int);

    //increase before decrease to not delete new val in case fo in and out are the same
    increaseValRef(_pvCtx, (int*)_pvData);

    pPrevious = get_display_function_data_property(_pvCtx, iObjUID);
    if (pPrevious)
    {
        decreaseValRef(_pvCtx, (int*)pPrevious);
    }

    if (setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_DISPLAY_FUNCTION__, &_pvData, jni_int_vector, iSize) == FALSE)
    {
        //don't need to keep _pvData, operation failed.
        decreaseValRef(_pvCtx, (int*)_pvData);
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "display_function_data");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}

/*------------------------------------------------------------------------*/
