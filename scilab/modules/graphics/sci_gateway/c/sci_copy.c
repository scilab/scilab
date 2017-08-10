/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
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
/* file: sci_copy.h                                                       */
/* desc : interface for copy routine                                      */
/*------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetProperty.h"
#include "CloneObjects.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"
/*--------------------------------------------------------------------------*/
int sci_copy(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    long long* l1 = NULL;
    int* piAddrl2 = NULL;
    long long* l2 = NULL;
    long long* outindex = NULL;

    unsigned long hdl = 0, hdlparent = 0;
    int iObjUID = 0, iSubwinparenttargetUID = 0, iCopyobjUID = 0;
    int* piSubWin = &iSubwinparenttargetUID;
    int iType = -1;
    int *piType = &iType;
    int m1 = 0, n1 = 0;
    int numrow = 0, numcol = 0;
    int isPolyline = 0;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /*  set or create a graphic window*/
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of handle at position 1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &m1, &n1, &l1); /* Gets the Handle passed as argument*/
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
        return 1;
    }

    if (m1 != 1 || n1 != 1)
    {
        OverLoad(1);
        return 0;
    }

    hdl = (unsigned long) * l1; /* on recupere le pointeur d'objet par le handle*/
    iObjUID = getObjectFromHandle(hdl);
    if (iObjUID == 0)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return 0;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_TEXT__ &&
            iType != __GO_ARC__ &&
            iType != __GO_POLYLINE__ &&
            iType != __GO_RECTANGLE__)
    {
        OverLoad(1);
        return 0;
    }

    if (iType == __GO_POLYLINE__)
    {
        isPolyline = 1;
    }
    else
    {
        isPolyline = 0;
    }

    if (nbInputArgument(pvApiCtx) > 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of handle at position 2.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrl2, &m1, &n1, &l2); /* Gets the command name */
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 2);
            return 1;
        }

        hdlparent = (unsigned long) * l2; /* on recupere le pointeur d'objet par le handle*/
        iSubwinparenttargetUID = getObjectFromHandle(hdlparent);
        if (iSubwinparenttargetUID == 0)
        {
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            return 0;
        }
        // Check Parent is an of type Axes.
        getGraphicObjectProperty(iSubwinparenttargetUID, __GO_TYPE__, jni_int, (void **)&piType);

        if (iType != __GO_AXES__)
        {
            Scierror(999, _("%s: Parent entity for destination should be an axes.\n"), fname);
            return 0;
        }

    }
    else
    {
        /* No destination Axes specified, use the copied object's parent Axes */
        getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piSubWin);
    }

    numrow   = 1;
    numcol   = 1;
    sciErr = allocMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, numrow, numcol, &outindex);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }


    if (isPolyline)
    {
        iCopyobjUID = clonePolyline(iObjUID);
    }
    else
    {
        iCopyobjUID = cloneGraphicObject(iObjUID);
    }

    *(outindex) = getHandle(iCopyobjUID);

    setGraphicObjectRelationship(iSubwinparenttargetUID, iCopyobjUID);

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
