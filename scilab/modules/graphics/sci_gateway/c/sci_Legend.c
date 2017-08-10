/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: sci_Legend.c                                                     */
/* desc : interface for captions routine                                  */
/*------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "DrawObjects.h"
#include "Axes.h" /* propertyNameToLegendPlace */
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentObject.h"

#define DEF_LEGEND_LOCATION "in_upper_right"

/*--------------------------------------------------------------------------*/
int sci_Legend(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    double* Empty = NULL;
    int* piAddrl1 = NULL;
    long long* l1 = NULL;
    int* piAddrStr = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    int numrow = 0, numcol = 0, n = 0, m2 = 0, n2 = 0;
    long handlesvalue = 0;
    int i = 0;
    int iObjUID = 0;
    int* tabofhandles = NULL;
    int iFigureUID = 0;
    int* piFigureUID = &iFigureUID;
    int iSubwinUID;
    int* piSubWin = &iSubwinUID;
    sciLegendPlace location;
    int type = -1;
    int *piType = &type;
    char **Str = NULL;
    int iLegendUID = 0;

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);

    getVarAddressFromPosition(pvApiCtx, 1 , &piAddrl1);
    getVarDimension(pvApiCtx, piAddrl1, &numrow, &numcol);
    n = numrow * numcol;
    if (numrow == 0 || numcol == 0)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, numrow, numcol, &Empty);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    getVarAddressFromPosition(pvApiCtx, 2 , &piAddrStr);
    getVarDimension(pvApiCtx, piAddrStr, &m2, &n2);
    if (m2 * n2 != n)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Incompatible length.\n"), fname, 1, 2);
        return 1;
    }


    // Retrieve a matrix of handle at position 1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &numrow, &numcol, &l1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
        return 1;
    }

    // Retrieve a matrix of string at position 2.
    if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m2, &n2, &Str))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
        return 1;
    }

    if (nbInputArgument(pvApiCtx) == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedMatrixOfString(m2, n2, Str);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            freeAllocatedMatrixOfString(m2, n2, Str);
            return 1;
        }

        location = propertyNameToLegendPlace((l2));
        freeAllocatedSingleString(l2);

        if (location == SCI_LEGEND_POSITION_UNSPECIFIED)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Incorrect value.\n"), fname, 3);
            freeAllocatedMatrixOfString(m2, n2, Str);
            return 1;
        }
    }
    else
    {
        location = propertyNameToLegendPlace(DEF_LEGEND_LOCATION);
    }

    tabofhandles = (int*)MALLOC(n * sizeof(int));
    if (tabofhandles == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        freeAllocatedMatrixOfString(m2, n2, Str);
        return 1;
    }

    for (i = 0; i < n; i++)
    {
        int iObj;
        int* piObj = &iObj;

        handlesvalue = (unsigned long) ((long long*)(l1))[n - 1 - i];
        iObjUID = getObjectFromHandle(handlesvalue);

        if (iObjUID == 0)
        {
            Scierror(999, _("%s: The handle is no more valid.\n"), fname);
            freeAllocatedMatrixOfString(m2, n2, Str);
            FREE(tabofhandles);
            return 1;
        }

        /**
          * We get the current pSubwin & pFigure from the first handle's parents.
          */
        if (i == 0)
        {
            getGraphicObjectProperty(iObjUID, __GO_PARENT_FIGURE__, jni_int, (void **)&piFigureUID);
            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piSubWin);
        }

        /**
         * We check that the pSubwin UID is the same for all given handles.
         */
        getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piObj);

        if (iObj != iSubwinUID)
        {
            Scierror(999, _("%s: Objects must have the same axes.\n"), fname);
            freeAllocatedMatrixOfString(m2, n2, Str);
            FREE(tabofhandles);
            return 1;
        }

        getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

        if (type != __GO_POLYLINE__)
        {
            Scierror(999, _("%s: The %d th handle is not a polyline handle.\n"), fname, i + 1);
            freeAllocatedMatrixOfString(m2, n2, Str);
            FREE(tabofhandles);
            return 1;
        }

        tabofhandles[i] = iObjUID;
    }

    /* Create the legend */
    iLegendUID = ConstructLegend (iSubwinUID, Str, tabofhandles, n);

    setGraphicObjectProperty(iLegendUID, __GO_LEGEND_LOCATION__, &location, jni_int, 1);

    freeAllocatedMatrixOfString(m2, n2, Str);
    FREE(tabofhandles);

    /* Return the handle of the newly created legend */
    if (createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, getHandle(getCurrentObject())))
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
