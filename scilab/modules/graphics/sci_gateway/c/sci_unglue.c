/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
/* file: sci_unglue.c                                                     */
/* desc : interface for unglue routine                                    */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "gw_graphics.h"
#include "api_scilab.h"
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
int sci_unglue(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    long long* l1 = NULL;

    int m1 = 0, n1 = 0;
    unsigned long hdl = 0;
    int i = 0;
    long long* outindex = NULL;

    int iObjUID = 0;
    int iObjectType = -1;
    int *piObjectType = &iObjectType;
    int iParentUID = 0;
    int* piParentUID = &iParentUID;
    int* piChildrenUID = 0;
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);
    /*  set or create a graphic window */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of handle at position 1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
        return 1;
    }

    hdl = (unsigned long) * (l1);

    iObjUID = getObjectFromHandle(hdl);

    if (iObjUID == 0)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return 0;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piObjectType);

    if (iObjectType != -1 && iObjectType == __GO_COMPOUND__)
    {
        // Retrieve number of children.
        getGraphicObjectProperty(iObjUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &piChildrenCount);

        // Retrieve all children UID.
        getGraphicObjectProperty(iObjUID, __GO_CHILDREN__, jni_int_vector, (void **) &piChildrenUID);

        // Retrieve Compound Parent.
        iParentUID = getParentObject(iObjUID);

        sciErr = allocMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, *piChildrenCount, 1, &outindex); /* We get the scalar value if it is ones */
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }

        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            outindex[i] = getHandle(piChildrenUID[i]);
            /*
             * Register Child to its new parent.
             * Children are added from the last to the first to obtain the same ordering
             * as the previous one (insertion is done at the head of the list).
             */
            setGraphicObjectRelationship(iParentUID, piChildrenUID[iChildrenCount - i - 1]);
        }

        deleteGraphicObject(iObjUID);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Object must be a Compound.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
