/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: get_user_data_property.c                                         */
/* desc : function to retrieve in Scilab the user_data field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sci_malloc.h"
#include "api_scilab.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"


/*------------------------------------------------------------------------*/
void* get_user_data_property(void* _pvCtx, int iObjUID)
{
    int iUserDataSize = 0;
    int *piUserDataSize = &iUserDataSize;
    int *piUserData = NULL;

    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_USER_DATA_SIZE__, jni_int, (void **)&piUserDataSize);

    getGraphicObjectProperty(iObjUID, __GO_USER_DATA__, jni_int_vector, (void **)&piUserData);

    if ((piUserData == NULL) || (piUserDataSize == NULL))
    {
        int iSize = sizeof(void*) / sizeof(int);
        status = sciReturnEmptyMatrix();
        //force user_data to have something and not create each time a new variable
        setGraphicObjectProperty(iObjUID, __GO_USER_DATA__, &status, jni_int_vector, iSize);
        increaseValRef(_pvCtx, (int*)status);
    }
    else
    {
        status = sciReturnUserData(piUserData, iUserDataSize);
    }

    return status;
}

/*------------------------------------------------------------------------*/
