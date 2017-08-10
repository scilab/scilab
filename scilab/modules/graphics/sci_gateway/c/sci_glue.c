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
/* file: sci_glue.h                                                       */
/* desc : interface for glue routine                                      */
/*------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>

#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "sci_malloc.h"
#include "GetProperty.h"
#include "elementary_functions.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "elem_common.h"

#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentObject.h"
/*--------------------------------------------------------------------------*/
int sci_glue(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    long long* l1 = NULL;
    double* l2 = NULL;
    int* lind = NULL;
    long long* outindex = NULL;

    int numrow = 0, numcol = 0, n = 0, cx1 = 1;
    int *pObj = NULL;
    int i = 0;

    int iCompoundUID = 0;
    int iParentUID = 0;
    int iCurrentParentUID = 0;
    int* piCurrentParentUID = &iCurrentParentUID;
    int iObjUID = 0;

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
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &numrow, &numcol, &l1); /* We get the scalar value if it is ones */
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
        return 1;
    }

    n = numrow * numcol;
    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, numrow, numcol, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 2, numrow, numcol, &lind);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    if (n > 1)
    {
        C2F(dcopy)(&n, (double*)l1, &cx1, l2, &cx1);
        C2F(dsort)(l2, &n, (int*)(lind));
        for (i = 1; i < n; i++)
        {
            long long i1 = ((long long*)l2)[i];
            long long i2 = ((long long*)l2)[i - 1];

            if (i1 == i2)
            {
                Scierror(999, _("%s: Each handle should not appear twice.\n"), fname);
                return 0;
            }
        }
    }

    /* we must change the pobj to the Compound type */
    pObj = (int*)MALLOC(n * sizeof(int));
    for (i = 0 ; i < n ; i++)
    {
        iObjUID = getObjectFromHandle((long)l1[i]);
        pObj[i] = iObjUID;
        if (iObjUID == 0)
        {
            FREE(pObj);
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            return 0;
        }

        iCurrentParentUID = getParentObject(iObjUID);
        //getGraphicObjectProperty(iObjUID, __GO_PARENT__, jni_string, (void **)&piCurrentParentUID);
        if (i == 0)
        {
            iParentUID = iCurrentParentUID;
        }

        if (iParentUID != iCurrentParentUID)
        {
            FREE(pObj);
            Scierror(999, _("%s: Objects must have the same parent.\n"), fname);
            return 0;
        }

    }

    //ret = CheckForCompound (handelsvalue, n);
    //if (ret>0)
    //{
    // MEM LEAK
    //    Scierror(999,_("%s: Handle %d cannot be glued (invalid parent).\n"),fname,ret);
    //    return 0;
    //}

    //if (ret<0)
    //{
    // MEM LEAK
    //   Scierror(999,_("%s: Handle %d cannot be glued (invalid type).\n"),fname,-ret);
    //    return 0;
    //}

    iCompoundUID = createCompound(iParentUID, pObj, n);
    setCurrentObject(iCompoundUID);

    numrow = 1;
    numcol = 1;

    sciErr = allocMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 3, numrow, numcol, &outindex);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    outindex[0] = getHandle(iCompoundUID);
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 3;
    ReturnArguments(pvApiCtx);
    FREE(pObj);

    return 0;
}
/*--------------------------------------------------------------------------*/
