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
/* file: sci_xrect.c                                                      */
/* desc : interface for xrect routine                                     */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "localization.h"
#include "Scierror.h"

#include "HandleManagement.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
int sci_xrect(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrl3 = NULL;
    double* l3 = NULL;
    int* piAddrl4 = NULL;
    double* l4 = NULL;

    long hdl = 0;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0;
    int iSubwinUID = 0;

    int foreground = 0;
    int *piForeground = &foreground;

    CheckInputArgument(pvApiCtx, 1, 4);

    iSubwinUID = getOrCreateDefaultSubwin();

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

    switch (nbInputArgument(pvApiCtx))
    {
        case 1 :
            //CheckLength
            if (m1 * n1 != 4)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 1, m1 * n1);
                return 1;
            }


            getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piForeground);

            if (strcmp(fname, "xrect") == 0)
            {
                Objrect ((l1), (l1 + 1), (l1 + 2), (l1 + 3),
                         &foreground, NULL, FALSE, TRUE, &hdl);
            }
            else
            {
                /* xfrect case */
                Objrect ((l1), (l1 + 1), (l1 + 2), (l1 + 3),
                         NULL, &foreground, TRUE, FALSE, &hdl);
            }

            if (hdl < 0)
            {
                break;
            }

            break;
        case 4 :
            getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piForeground);

            //CheckScalar
            if (m1 != 1 || n1 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 1);
                return 1;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            // YOU MUST REMOVE YOUR VARIABLE DECLARATION "int l2".
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
                return 1;
            }

            //CheckScalar
            if (m2 != 1 || n2 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 2);
                return 1;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 3.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl3, &m3, &n3, &l3);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
                return 1;
            }

            //CheckScalar
            if (m3 != 1 || n3 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 3);
                return 1;
            }

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


            if (strcmp(fname, "xrect") == 0)
            {
                Objrect ((l1), (l2), (l3), (l4),
                         &foreground, NULL, FALSE, TRUE, &hdl);
            }
            else
            {
                Objrect ((l1), (l2), (l3), (l4),
                         NULL, &foreground, TRUE, FALSE, &hdl);
            }

            if (hdl < 0)
            {
                break;
            }

            break;
        default :
            Scierror(999, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), fname, 1, 4);
            break;
    }

    if (hdl > 0)
    {
        setGraphicObjectRelationship(iSubwinUID, getObjectFromHandle(hdl));
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
