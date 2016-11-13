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
/* file: sci_xtitle.c                                                     */
/* desc : interface for xtitle routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"

#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
int sci_xtitle(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddr4 = NULL;
    int* boxPtr = NULL;
    int* piAddrStr = NULL;

    int  narg = 0;
    int  nbLabels = 0; /* number of modified labels */
    int  box = 0;
    BOOL isBoxSpecified = FALSE;
    int iSubwinUID = 0;
    static rhs_opts opts[] =
    {
        { -1, "boxed", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, pvApiCtx);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 5);


    nbLabels = nbInputArgument(pvApiCtx);

    /* get the given options from the name in opts */
    if (!getOptionals(pvApiCtx, fname, opts))
    {
        /* error */
        return 0;
    }

    /* compatibility with previous version in which box was put */
    /* at the fourth position */

    if (nbInputArgument(pvApiCtx) == 4)
    {
        int type = getInputArgumentType(pvApiCtx, 4);
        if (type == 1 || type == 8)/* double or int */
        {
            int n = 0, m = 0;
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 4.
            sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr4, &m, &n, &boxPtr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
                return 1;
            }

            //CheckScalar
            if (m != 1 || n != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 4);
                return 1;
            }

            box = *boxPtr;
            nbLabels--; /* it is not a label text */
            isBoxSpecified = TRUE;
        }
    }

    if (opts[0].iPos != -1 && !isBoxSpecified)
    {
        /* check if "box" is in the options */
        getScalarBoolean(pvApiCtx, opts[0].piAddr, &box);
        if (opts[0].iRows != 1 || opts[0].iCols != 1)
        {
            /* check size */
            Scierror(999, _("%s: Wrong type for input argument: Scalar expected.\n"), fname);
            return 1;
        }
        nbLabels--; /* it is not a label text */
    }

    iSubwinUID = getOrCreateDefaultSubwin();

    for (narg = 1 ; narg <= nbLabels ; narg++)
    {
        int m = 0, n = 0;
        char **Str = NULL;
        int iModifiedLabel = 0;
        int* piModifiedLabel = &iModifiedLabel;

        sciErr = getVarAddressFromPosition(pvApiCtx, narg, &piAddrStr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position narg.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m, &n, &Str))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, narg);
            return 1;
        }

        if (m * n == 0)
        {
            freeArrayOfString(Str, m * n);
            continue;
        }

        switch (narg)
        {
            case 1:
                getGraphicObjectProperty(iSubwinUID, __GO_TITLE__, jni_int, (void **)&piModifiedLabel);
                break;
            case 2:
                getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LABEL__, jni_int, (void **)&piModifiedLabel);
                break;
            case 3:
                getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LABEL__, jni_int, (void **)&piModifiedLabel);
                break;
            case 4:
                getGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_LABEL__, jni_int, (void **)&piModifiedLabel);
                break;
            default:
                break;
        }

#if 0
        startFigureDataWriting(pFigure);
#endif

        sciSetText(iModifiedLabel, Str, m, n);

        setGraphicObjectProperty(iModifiedLabel, __GO_FILL_MODE__, &box, jni_bool, 1);

#if 0
        endFigureDataWriting(pFigure);
#endif

        freeArrayOfString(Str, m * n);
    }

    setCurrentObject(iSubwinUID);
#if 0
    sciDrawObj(pFigure);
#endif

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
