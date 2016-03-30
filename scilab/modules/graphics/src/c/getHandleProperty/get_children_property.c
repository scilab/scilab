/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_children_property.c                                          */
/* desc : function to retrieve in Scilab the children field of a          */
/*        handle except datatips                                          */
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

/*--------------------------------------------------------------------------*/
void* get_children_property(void* _pvCtx, int iObjUID)
{
    int i = 0;
    void* status = NULL;
    long *plChildren = NULL;
    int* piChildrenUID = NULL;
    int iHidden = 0;
    int *piHidden = &iHidden;
    int childrenNumber = 0;
    int iChildIndex = 0;
    int iShowHiddenHandles = 0;
    int *piShowHiddenHandles = &iShowHiddenHandles;

    // All Graphic Objects have __GO_CHILDREN__ & __GO_CHILDREN_COUNT__ properties.
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;

    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piChildrenCount);
    if (piChildrenCount == NULL || piChildrenCount[0] == 0)
    {
        // No Child
        return sciReturnEmptyMatrix();
    }

    getGraphicObjectProperty(iObjUID, __GO_CHILDREN__, jni_int_vector, (void **)&piChildrenUID);

    getGraphicObjectProperty(getConsoleIdentifier(), __GO_SHOWHIDDENHANDLES__, jni_bool, (void **)&piShowHiddenHandles);

    if (iShowHiddenHandles == 0)
    {
        // Find number of not hidden children except datatips
        for (i = 0; i < piChildrenCount[0]; ++i)
        {
            getGraphicObjectProperty(piChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
            if (iHidden == 0)
            {
                getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void**) &piType);
                if (type != __GO_DATATIP__)
                {
                    childrenNumber++;
                }
            }
        }

        if (childrenNumber == 0)
        {
            // No Child
            releaseGraphicObjectProperty(__GO_CHILDREN__, piChildrenUID, jni_int_vector, iChildrenCount);
            return sciReturnEmptyMatrix();
        }
    }
    else
    {
        // Find all children except datatips
        for (i = 0; i < piChildrenCount[0]; ++i)
        {
            getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void**) &piType);
            if (type != __GO_DATATIP__)
            {
                childrenNumber++;
            }
        }

        if (childrenNumber == 0)
        {
            // No Child
            releaseGraphicObjectProperty(__GO_CHILDREN__, piChildrenUID, jni_int_vector, iChildrenCount);
            return sciReturnEmptyMatrix();
        }

    }

    plChildren = MALLOC(childrenNumber * sizeof(long));

    for (i = 0; i < piChildrenCount[0]; ++i)
    {
        getGraphicObjectProperty(piChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
        if (iHidden == 0 || iShowHiddenHandles == 1)
        {
            getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void**)&piType);
            if (type != __GO_DATATIP__)
            {
                plChildren[iChildIndex++] = getHandle(piChildrenUID[i]);
            }
        }
    }

    status = sciReturnColHandleVector(plChildren, childrenNumber);
    FREE(plChildren);
    releaseGraphicObjectProperty(__GO_CHILDREN__, piChildrenUID, jni_int_vector, iChildrenCount);
    return status;
}

/*--------------------------------------------------------------------------*/
