/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    long *plDatatips = NULL;
    int* piChildrenUID = NULL;
    int iHidden = 0;
    int *piHidden = &iHidden;
    int datatipsNumber = 0;
    int iDatatipIndex = 0;
    int iShowHiddenHandles = 0;
    int *piShowHiddenHandles = &iShowHiddenHandles;
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piChildrenCount);
    if (piChildrenCount == NULL || piChildrenCount[0] == 0)
    {
        // No Child
        return sciReturnEmptyMatrix(_pvCtx);
    }

    getGraphicObjectProperty(iObjUID, __GO_CHILDREN__, jni_int_vector, (void **)&piChildrenUID);

    getGraphicObjectProperty(getConsoleIdentifier(), __GO_SHOWHIDDENHANDLES__, jni_bool, (void **)&piShowHiddenHandles);

    if (iShowHiddenHandles == 0)
    {
        // Find number of not hidden datatips
        for (i = 0; i < piChildrenCount[0]; ++i)
        {
            getGraphicObjectProperty(piChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
            if (iHidden == 0)
            {
                getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void**) &piType);
                if (type == __GO_DATATIP__)
                {
                    datatipsNumber++;
                }
            }
        }

        if (datatipsNumber == 0)
        {
            // No Child
            return sciReturnEmptyMatrix(_pvCtx);
        }
    }
    else
    {
        // Find number of all datatips
        for (i = 0; i < piChildrenCount[0]; ++i)
        {
            getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void**) &piType);
            if (type == __GO_DATATIP__)
            {
                datatipsNumber++;
            }
        }

        if (datatipsNumber == 0)
        {
            // No Child
            return sciReturnEmptyMatrix(_pvCtx);
        }

    }

    plDatatips = MALLOC(datatipsNumber * sizeof(long));

    for (i = 0; i < piChildrenCount[0]; ++i)
    {
        getGraphicObjectProperty(piChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
        if (iHidden == 0 || iShowHiddenHandles == 1)
        {
	    getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void**) &piType);
	    if (type == __GO_DATATIP__)
	    {
                plDatatips[iDatatipIndex++] = getHandle(piChildrenUID[i]);
	    }
        }
    }

    status = sciReturnColHandleVector(_pvCtx, plDatatips, datatipsNumber);
    FREE(plDatatips);

    return status;
}
