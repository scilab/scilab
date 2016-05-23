/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: sci_xtring.c                                                     */
/* desc : interface for xstring routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "sciCall.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "BuildObjects.h"

#include "CurrentSubwin.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "createGraphicObject.h"

/*--------------------------------------------------------------------------*/
int sci_xstring(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrStr = NULL;
    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrl4 = NULL;
    double* l4 = NULL;
    int* piAddrl5 = NULL;
    double* l5 = NULL;

    double rect[4];
    double x = 0., y = 0., angle = 0.0;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0, m5 = 0, n5 = 0;
    char **Str = NULL;
    char **sendStr = NULL;
    int sendm3 = 0, sendn3 = 0;
    long hdlstr = 0;
    int nbElement = 0, i = 0;
    BOOL isboxed = FALSE;
    int iCurrentSubWin = 0;

    CheckInputArgument(pvApiCtx, 3, 5);

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrStr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of string at position 3.
    if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m3, &n3, &Str))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
        return 1;
    }

    if (m3 * n3 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        freeArrayOfString(Str, m3 * n3);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }


    if (m1 * n1 == 1 || m2 * n2 == 1)
    {
        nbElement = m1 * n1 * m2 * n2;
    }
    else if (m1 * n1 == m2 * n2)
    {
        nbElement = m1 * n1;
    }
    else
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same element number expected.\n"), fname, 1, 2);
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        freeArrayOfString(Str, m3 * n3);
        return 0;
    }

    if (nbElement == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        freeArrayOfString(Str, m3 * n3);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) >= 4)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrl4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        // Retrieve a matrix of double at position 4.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl4, &m4, &n4, &l4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        if (m4 * n4 != 1 && m4 * n4 != nbElement)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d or %d elements expected.\n"), fname, 4, 1, nbElement);
            AssignOutputVariable(pvApiCtx, 1) = 0;
            ReturnArguments(pvApiCtx);
            freeArrayOfString(Str, m3 * n3);
            return 0;
        }
    }
    if (nbInputArgument(pvApiCtx) >= 5)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrl5);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        // Retrieve a matrix of double at position 5.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl5, &m5, &n5, &l5);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        if (m5 * n5 != 1 && m5 * n5 != nbElement)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d or %d elements expected.\n"), fname, 5, 1, nbElement);
            AssignOutputVariable(pvApiCtx, 1) = 0;
            ReturnArguments(pvApiCtx);
            freeArrayOfString(Str, m3 * n3);
            return 0;
        }
    }

    x = *l1;
    y = *l2;
    sendStr = Str;
    sendm3 = m3;
    sendn3 = n3;
    if (nbInputArgument(pvApiCtx) >= 4)
    {
        angle = DEG2RAD(*l4);
    }
    if (nbInputArgument(pvApiCtx) >= 5)
    {
        isboxed = (*l5 != 0);
    }

    iCurrentSubWin = getOrCreateDefaultSubwin();

    if (nbElement == 1)
    {
        Objstring(sendStr, sendm3, sendn3, x, y, &angle, rect, TRUE, NULL, &hdlstr, 0, NULL, NULL, isboxed
                  && (angle == 0), TRUE, FALSE, ALIGN_LEFT);
    }
    else
    {
        for (i = 0; i < nbElement; i++)
        {
            if (m1 * n1 == nbElement)
            {
                x = *((l1) + i);
            }
            if (m2 * n2 == nbElement)
            {
                y = *((l2) + i);
            }
            if (m3 * n3 == nbElement)
            {
                sendStr = Str + i;
                sendm3 = sendn3 = 1;
            }
            if (nbInputArgument(pvApiCtx) >= 4 && m4 * n4 == nbElement)
            {
                angle = DEG2RAD(*((l4) + i));
            }
            if (nbInputArgument(pvApiCtx) >= 5 && m5 * n5 == nbElement)
            {
                isboxed = ((l5) != 0);
            }

            Objstring(sendStr, sendm3, sendn3, x, y, &angle, rect, TRUE, NULL, &hdlstr, 0, NULL, NULL, isboxed
                      && (angle == 0), TRUE, FALSE, ALIGN_LEFT);
        }

        /*
         * If one of the string creation calls fails,
         * the compound build call will crash.
         * To be modified
         */
        {
            int o = createCompoundSeq(iCurrentSubWin, nbElement);
        }
    }

    /* we must free Str memory */

    freeArrayOfString(Str, m3 * n3);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
