/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_unglue.c                                                     */
/* desc : interface for unglue routine                                    */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "gw_graphics.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "DestroyObjects.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "deleteGraphicObject.h"
/*--------------------------------------------------------------------------*/
int sci_unglue(char *fname,unsigned long fname_len)
{
    int m1 = 0,n1 = 0,l1 = 0;
    unsigned long hdl = 0;
    int outindex = 0, i = 0;

    int iOne = 1;
    char *pobjUID = NULL;
    int iObjectType = -1;
    int *piObjectType = &iObjectType;
    char *pstParentUID = NULL;
    char **pstChildrenUID = NULL;
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;

    CheckRhs(1, 1);
    CheckLhs(0, 1);
    /*  set or create a graphic window */
    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &m1, &n1, &l1);
    hdl = (unsigned long)*hstk(l1);

    pobjUID = (char*)getObjectFromHandle(hdl);

    if (pobjUID == NULL)
    {
        Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
        return 0;
    }

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piObjectType);

    if (piObjectType != -1 && iObjectType == __GO_COMPOUND__)
    {
        // Retrieve number of children.
        getGraphicObjectProperty(pobjUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &piChildrenCount);

        // Retrieve all children UID.
        getGraphicObjectProperty(pobjUID, __GO_CHILDREN__, jni_string_vector, (void **) &pstChildrenUID);

        // Retrieve Compound Parent.
        getGraphicObjectProperty(pobjUID, __GO_PARENT__, jni_string, (void **)&pstParentUID);

        CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, piChildrenCount, &iOne, &outindex);

        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            hstk(outindex)[i] = getHandle(pstChildrenUID[i]);
            /*
             * Register Child to its new parent.
             * Children are added from the last to the first to obtain the same ordering
             * as the previous one (insertion is done at the head of the list).
             */
            setGraphicObjectRelationship(pstParentUID, pstChildrenUID[iChildrenCount-i-1]);
        }

        deleteGraphicObject(pobjUID);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999,_("%s: Object must be a Compound.\n"),fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
