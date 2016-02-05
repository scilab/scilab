/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: sci_get.c                                                        */
/* desc : interface for sci_get routine                                   */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "HandleManagement.h"
#include "GetHashTable.h"
#include "BuildObjects.h"
#include "localization.h"
#include "Scierror.h"

#include "HandleManagement.h"
#include "CurrentObject.h"
#include "CurrentSubwin.h"
#include "getConsoleIdentifier.h"
#include "returnProperty.h"

#include "SetPropertyStatus.h"
#include "GetScreenProperty.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "FigureList.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
int sci_get(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    long long* l1 = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    int m1 = 0, n1 = 0;
    long hdl = 0;

    int lw = 0;
    int iObjUID = 0;

    int status = SET_PROPERTY_ERROR;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isMListType(pvApiCtx, piAddrl1) || isTListType(pvApiCtx, piAddrl1))
    {
        OverLoad(1);
        return 0;
    }

    /*
     * The first input argument can be an ID or a marker (in this case, get returns the value of the current object */
    switch (getInputArgumentType(pvApiCtx, 1))
    {
        case sci_matrix: //console handle
        {
            double dbll1 = 0;

            if (isScalar(pvApiCtx, piAddrl1) == 0)
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getScalarDouble(pvApiCtx, piAddrl1, &dbll1))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if ((int)dbll1 == 0) /* Console property */
            {
                int* piAddrstkAdr = NULL;
                char *stkAdr = NULL;
                if (nbInputArgument(pvApiCtx) == 1)
                {
                    if (sciReturnHandle(getHandle(getConsoleIdentifier())) != 0)    /* Get Console handle */
                    {
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                    ReturnArguments(pvApiCtx);
                    return 0;
                }

                CheckInputArgument(pvApiCtx, 2, 2);
                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrstkAdr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of string at position 2.
                if (getAllocatedSingleString(pvApiCtx, piAddrstkAdr, &stkAdr))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                    return 1;
                }

                if (GetScreenProperty(pvApiCtx, stkAdr) != SET_PROPERTY_SUCCEED)
                {
                    Scierror(999, _("%s: Could not read property '%s' for console object.\n"), "get", stkAdr[0]);
                    freeAllocatedSingleString(stkAdr);
                    return 1;
                }
                freeAllocatedSingleString(stkAdr);
                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }

            return 0;
            break;
        }
        case sci_handles:          /* scalar argument (hdl + string) */
            CheckInputArgument(pvApiCtx, 1, 2);

            // Retrieve a matrix of handle at position 1.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &m1, &n1, &l1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 1;
            }

            if (m1 != 1 || n1 != 1)
            {
                //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
                OverLoad(1);
                return 0;
            }

            if (nbInputArgument(pvApiCtx) == 1)
            {
                //get path from handle
                int uic = getObjectFromHandle((long) * l1);
                char* path = get_path(uic);
                if (path[0] == '\0')
                {
                    Scierror(999, _("%s: Unable to get useful path from this handle.\n"), fname);
                    return 1;
                }

                createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, path);
                FREE(path);

                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
                return 0;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if (isScalar(pvApiCtx, piAddrl2) == 0 || isStringType(pvApiCtx, piAddrl2) == 0)
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                return 1;
            }

            hdl = (long) * l1; /* on recupere le pointeur d'objet par le handle */
            break;
        case sci_strings:          /* string argument (string) */
        {
            char* pstFirst = NULL;
            CheckInputArgument(pvApiCtx, 1, 2);

            if (isScalar(pvApiCtx, piAddrl1) == 0)
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrl1, &pstFirst))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
                return 1;
            }

            if (strcmp(pstFirst, "default_figure") == 0 ||
                    strcmp(pstFirst, "default_axes") == 0 ||
                    strcmp(pstFirst, "current_figure") == 0 ||
                    strcmp(pstFirst, "current_axes") == 0 ||
                    strcmp(pstFirst, "current_entity") == 0 ||
                    strcmp(pstFirst, "hdl") == 0 ||
                    strcmp(pstFirst, "figures_id") == 0)
            {
                hdl = 0;
                l2 = pstFirst;
            }
            else
            {
                int uid = search_path(pstFirst);
                if (uid != 0)
                {
                    freeAllocatedSingleString(pstFirst);
                    hdl = getHandle(uid);

                    if (nbInputArgument(pvApiCtx) == 1)
                    {
                        createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, hdl);
                        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }

                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if (isScalar(pvApiCtx, piAddrl2) == 0 || isStringType(pvApiCtx, piAddrl2) == 0)
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                        return 1;
                    }

                    if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                        return 1;
                    }
                }
                else
                {
                    createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1);
                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                    ReturnArguments(pvApiCtx);
                    return 0;
                }
            }
            break;
        }
        default:
            //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
            OverLoad(1);
            return 0;
            break;
    }
    /* (l2) est la commande, l3 l'indice sur les parametres de la commande */
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (hdl == 0)
    {
        /* No handle specified */
        if (callGetProperty(pvApiCtx, 0, (l2)) != 0)
        {
            /* An error has occurred */
            freeAllocatedSingleString(l2);
            ReturnArguments(pvApiCtx);
            return 0;
        }
    }
    else
    {
        iObjUID = getObjectFromHandle(hdl);
        if (iObjUID != 0)
        {

            if (callGetProperty(pvApiCtx, iObjUID, (l2)) != 0)
            {
                /* An error has occurred */
                freeAllocatedSingleString(l2);
                ReturnArguments(pvApiCtx);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            freeAllocatedSingleString(l2);
            return 0;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    freeAllocatedSingleString(l2);
    return 0;
}

/*--------------------------------------------------------------------------*/
