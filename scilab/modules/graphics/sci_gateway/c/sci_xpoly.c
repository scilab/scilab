/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: sci_xpoly.c                                                      */
/* desc : interface for xpoly routine                                     */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "localization.h"
#include "Scierror.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "CurrentObject.h"
#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
int sci_xpoly(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrl3 = NULL;
    char* l3 = NULL;
    int* piAddrl4 = NULL;
    double* l4 = NULL;

    int iSubwinUID = 0;
    int iObjUID = 0;

    int m1 = 0, n1 = 0, m2 = 0 , n2 = 0, m4 = 0, n4 = 0, close = 0, mn2 = 0;

    long hdl = 0;/* NG */
    int mark = 0;/* NG */
    int markMode = 0;
    int lineMode = 0;
    int foreground = 0;
    int iTmp = 0;
    int* piTmp = &iTmp;

    CheckInputArgument(pvApiCtx, 2, 4);
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckSameDims
    if (m1 != m2 || n1 != n2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
        return 1;
    }

    mn2 = m2 * n2;

    if (nbInputArgument(pvApiCtx) >= 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        if (getAllocatedSingleString(pvApiCtx, piAddrl3, &l3))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            return 1;
        }

        if (strcmp((l3), "lines") == 0)
        {
            mark = 1; /* NG */
        }
        else if (strcmp((l3), "marks") == 0)
        {
            mark = 0; /* NG */
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, "lines", "marks");
            freeAllocatedSingleString(l3);
            return 0;
        }
        freeAllocatedSingleString(l3);
    }
    else
    {
        mark = 1; /* NG */
    }

    if (nbInputArgument(pvApiCtx) >= 4)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrl4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 4.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl4, &m4, &n4, &l4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
            return 1;
        }

        //CheckScalar
        if (m4 != 1 || n4 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 4);
            return 1;
        }

        close = (int)  * (l4);
    }
    /* NG beg */

    iSubwinUID = getOrCreateDefaultSubwin();

    Objpoly ((l1), (l2), mn2, close, mark, &hdl);

    iObjUID = getObjectFromHandle(hdl); /* the polyline newly created */

    setGraphicObjectRelationship(iSubwinUID, iObjUID);

    /*
     * The contour properties set calls below were
     * already present and have been updated for the MVC.
     */
    if (mark == 0)
    {
        /* marks are enabled but markstyle & foreground
           is determined by parents' markstyle & foreground */

        markMode = 1;
        lineMode = 0;

        getGraphicObjectProperty(iSubwinUID, __GO_MARK_STYLE__, jni_int, (void**)&piTmp);
        setGraphicObjectProperty(iObjUID, __GO_MARK_STYLE__, piTmp, jni_int, 1);
    }
    else
    {
        markMode = 0;
        lineMode = 1;

        getGraphicObjectProperty(iSubwinUID, __GO_LINE_STYLE__, jni_int, (void**)&piTmp);
        sciSetLineStyle(iObjUID, iTmp);
    }

    getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piTmp);
    foreground = iTmp;

    setGraphicObjectProperty(iObjUID, __GO_LINE_COLOR__, &foreground, jni_int, 1);

    setGraphicObjectProperty(iObjUID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
    setGraphicObjectProperty(iObjUID, __GO_LINE_MODE__, &lineMode, jni_bool, 1);

    /* NG end */
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
