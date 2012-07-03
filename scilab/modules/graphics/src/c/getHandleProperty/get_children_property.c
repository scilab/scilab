/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_children_property.c                                          */
/* desc : function to retrieve in Scilab the children field of a          */
/*        handle                                                          */
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

/*--------------------------------------------------------------------------*/
int get_children_property(void* _pvCtx, char* pobjUID)
{
    int i = 0;
    int status = 0;
    long *plChildren = NULL;
    char **pstChildrenUID;
    int iHidden = 0;
    int *piHidden = &iHidden;
    int iNotHiddenChildrenNumber = 0;
    int iChildIndex = 0;
    int iShowHiddenHandles = 0;
    int *piShowHiddenHandles = &iShowHiddenHandles;

    // All Graphic Objects have __GO_CHILDREN__ & __GO_CHILDREN_COUNT__ properties.
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;

    getGraphicObjectProperty(pobjUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piChildrenCount);

    if (piChildrenCount[0] == 0)
    {
        // No Child
        return sciReturnEmptyMatrix(_pvCtx);
    }

    getGraphicObjectProperty(pobjUID, __GO_CHILDREN__, jni_string_vector, (void **)&pstChildrenUID);

    getGraphicObjectProperty(getConsoleIdentifier(), __GO_SHOWHIDDENHANDLES__, jni_bool, (void **)&piShowHiddenHandles);

    if (iShowHiddenHandles == 0)
    {
        // Find number of not hidden children
        for (i = 0; i < piChildrenCount[0]; ++i)
        {
            getGraphicObjectProperty(pstChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
            if (iHidden == 0)
            {
                iNotHiddenChildrenNumber++;
            }
        }

        if (iNotHiddenChildrenNumber == 0)
        {
            // No Child
            return sciReturnEmptyMatrix(_pvCtx);
        }
    }
    else
    {
        iNotHiddenChildrenNumber = piChildrenCount[0];
    }

    plChildren = MALLOC(iNotHiddenChildrenNumber * sizeof(long));

    for (i = 0; i < piChildrenCount[0]; ++i)
    {
        getGraphicObjectProperty(pstChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
        if (iHidden == 0 || iShowHiddenHandles == 1)
        {
            plChildren[iChildIndex++] = getHandle(pstChildrenUID[i]);
        }
    }

    status = sciReturnColHandleVector(_pvCtx, plChildren, iNotHiddenChildrenNumber);
    FREE(plChildren);

    return status;
}

/*--------------------------------------------------------------------------*/
