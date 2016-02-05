/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: sci_swap_handles.c                                               */
/* desc : interface for swap_handles routine                              */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int sci_swap_handles(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrfirstHdlStkIndex = NULL;
    long long* firstHdlStkIndex = NULL;
    int* piAddrsecondHdlStkIndex = NULL;
    long long* secondHdlStkIndex = NULL;

    int firstHdlCol = 0;
    int firstHdlRow = 0;
    int secondHdlCol = 0;
    int secondHdlRow = 0;
    int iHandle_1 = 0;
    int iHandle_2 = 0;
    int iType_1 = 0;
    int iType_2 = 0;
    int * piType_1 = &iType_1;
    int * piType_2 = &iType_2;
    int iParent_1 = 0;
    int* piParent_1 = &iParent_1;
    int iParent_2 = 0;
    int* piParent_2 = &iParent_2;
    int iParentType_1 = 0;
    int iParentType_2 = 0;
    int * piParentType_1 = &iParentType_1;
    int * piParentType_2 = &iParentType_2;
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    int* piChildrenUID = NULL;
    int i = 0;
    long h = 0;

    CheckInputArgument(pvApiCtx,  2, 2);
    CheckOutputArgument(pvApiCtx,  0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx,  1, &piAddrfirstHdlStkIndex);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of handle at position  1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrfirstHdlStkIndex, &firstHdlRow, &firstHdlCol, &firstHdlStkIndex);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname,  1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx,  2, &piAddrsecondHdlStkIndex);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of handle at position  2.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrsecondHdlStkIndex, &secondHdlRow, &secondHdlCol, &secondHdlStkIndex);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname,  2);
        return 1;
    }


    if (firstHdlRow * firstHdlCol != 1 || secondHdlRow * secondHdlCol != 1)
    {
        Scierror(999, _("%s: Routine can only swap two single handles.\n"), fname);
        return 0;
    }

    /* get the two handles and swap them */
    h = (long) * (firstHdlStkIndex);
    iHandle_1 = getObjectFromHandle(h);

    h = (long) * (secondHdlStkIndex);
    iHandle_2 = getObjectFromHandle(h);

    getGraphicObjectProperty(iHandle_1, __GO_TYPE__, jni_int, (void **)&piType_1);
    getGraphicObjectProperty(iHandle_2, __GO_TYPE__, jni_int, (void **)&piType_2);

    iParent_1 = getParentObject(iHandle_1);
    iParent_2 = getParentObject(iHandle_2);

    getGraphicObjectProperty(iParent_1, __GO_TYPE__, jni_int, (void **)&piParentType_1);
    getGraphicObjectProperty(iParent_2, __GO_TYPE__, jni_int, (void **)&piParentType_2);

    // Check if objects do not have the same parent
    if (iParent_1 == iParent_2)
    {
        getGraphicObjectProperty(iParent_1, __GO_CHILDREN_COUNT__, jni_int, (void **)&piChildrenCount);
        getGraphicObjectProperty(iParent_1, __GO_CHILDREN__, jni_int_vector, (void **)&piChildrenUID);

        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            if (piChildrenUID[i] == iHandle_1)
            {
                piChildrenUID[i] = iHandle_2;
            }
            else if (piChildrenUID[i] == iHandle_2)
            {
                piChildrenUID[i] = iHandle_1;
            }
        }

        setGraphicObjectProperty(iParent_1, __GO_CHILDREN__, piChildrenUID, jni_int_vector, iChildrenCount);
    }
    else if (iType_1 == iType_2 || iParentType_1 == iParentType_2)
    {
        setGraphicObjectRelationship(iParent_1, iHandle_2);
        setGraphicObjectRelationship(iParent_2, iHandle_1);
    }
    else
    {
        Scierror(999, _("%s: Handles do not have the same parent type neither the same type.\n"), fname);
        return 0;
    }
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
