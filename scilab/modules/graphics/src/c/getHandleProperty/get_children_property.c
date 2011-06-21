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


#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int get_children_property(char *pobjUID)
{
    int i = 0;
    int status = 0;
    long *plChildren = NULL;
    char **pstChildrenUID;

    // All Graphic Objects have __GO_CHILDREN__ & __GO_CHILDREN_COUNT__ properties.
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    getGraphicObjectProperty(pobjUID, __GO_CHILDREN_COUNT__, jni_int, &piChildrenCount);


    if (piChildrenCount[0] == 0)
    {
        // No Child
        return sciReturnEmptyMatrix();
    }

    getGraphicObjectProperty(pobjUID, __GO_CHILDREN__, jni_string_vector, &pstChildrenUID);

    plChildren = MALLOC(piChildrenCount[0] * sizeof(long));

    for (i = 0 ; i < piChildrenCount[0] ; ++i)
    {
        plChildren[i] = getHandle(pstChildrenUID[i]);
    }

    status = sciReturnColHandleVector(plChildren, piChildrenCount[0]);

    FREE( plChildren ) ;

    return status ;
}
/*--------------------------------------------------------------------------*/
