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
/* file: sci_xrects.c                                                     */
/* desc : interface for xrects routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "createGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentObject.h"
/*--------------------------------------------------------------------------*/
int sci_xrects(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddr2 = NULL;
    int* l2 = NULL;

    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    long  hdl = 0;
    int i = 0;
    int iSubwinUID = 0;

    int foreground = 0;
    int *piForeground = &foreground;
    int iCompoundUID = 0;

    CheckInputArgument(pvApiCtx, 1, 2);

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

    if (m1 != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %s expected.\n"), fname, 1, "(4,n)");
        return 0;
    }


    if (nbInputArgument(pvApiCtx) == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 2.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr2, &m2, &n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
            return 1;
        }

        //CheckVector
        if (m2 != 1 && n2 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 2);
            return 1;
        }

        if (m2 * n2 != n1)
        {
            Scierror(999, _("%s: Incompatible length for input arguments #%d and #%d.\n"), fname, 1, 2);
            return 0;
        }
    }
    else
    {
        m2 = 1;
        n2 = n1;

        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 2, m2, n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        for (i = 0; i < n2; ++i)
        {
            l2[i] = 0;
        }
    }

    iSubwinUID = getOrCreateDefaultSubwin();

    // Create compound.
    iCompoundUID = createGraphicObject(__GO_COMPOUND__);
    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(iSubwinUID, iCompoundUID);

    /** Get Subwin line color */
    getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piForeground);

    for (i = 0; i < n1; ++i)
    {
        /*       j = (i==0) ? 0 : 1; */
        if (l2[i] == 0)
        {
            /** fil(i) = 0 rectangle i is drawn using the current line style (or color).**/
            /* color setting is done now */

            Objrect((l1 + (4 * i)), (l1 + (4 * i) + 1), (l1 + (4 * i) + 2), (l1 + (4 * i) + 3),
                    &foreground, NULL, FALSE, TRUE, &hdl);
        }
        else
        {
            if (l2[i] < 0)
            {
                /** fil(i) < 0 rectangle i is drawn using the line style (or color) **/
                int tmp = - (*(int*)(l2 + i));
                Objrect((l1 + (4 * i)), (l1 + (4 * i) + 1), (l1 + (4 * i) + 2), (l1 + (4 * i) + 3),
                        &tmp, NULL, FALSE, TRUE, &hdl);
            }
            else
            {
                /** fil(i) > 0   rectangle i is filled using the pattern (or color) **/
                Objrect((l1 + (4 * i)), (l1 + (4 * i) + 1), (l1 + (4 * i) + 2), (l1 + (4 * i) + 3),
                        NULL, l2 + i, TRUE, FALSE, &hdl);
            }
        }
        // Add newly created object to Compound
        setGraphicObjectRelationship(iCompoundUID, getObjectFromHandle(hdl));
    }

    /** make Compound current object **/
    setCurrentObject(iCompoundUID);
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
