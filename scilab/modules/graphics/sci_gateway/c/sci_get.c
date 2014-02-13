/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_get.c                                                        */
/* desc : interface for sci_get routine                                   */
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
/*--------------------------------------------------------------------------*/

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
int sciGet(void* _pvCtx, int iObjUID, char *marker)
{
    /* find the function in the hashtable relative to the property name */
    /* and call it */
    return callGetProperty(_pvCtx, iObjUID, marker);
}

/*--------------------------------------------------------------------------*/
int sci_get(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* pdbll1 = NULL;
    int* piAddrstkAdr = NULL;
    long long* l1 = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    int m1 = 0, n1 = 0;
    long hdl = 0;

    int lw = 0;
    int iObjUID = 0;

    /* Root properties */
    char **stkAdr = NULL;
    int status = SET_PROPERTY_ERROR;

    if (((checkInputArgumentType(pvApiCtx, 1, sci_mlist))) || ((checkInputArgumentType(pvApiCtx, 1, sci_tlist))))
    {
        //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
        OverLoad(1);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /*  set or create a graphic window */

    /*
     * The first input argument can be an ID or a marker (in this case, get returns the value of the current object */
    switch (getInputArgumentType(pvApiCtx, 1))
    {
        case 1:                    /* tclsci handle */
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &pdbll1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if ((int)pdbll1[0] == 0) /* Root property */
            {
                if (nbInputArgument(pvApiCtx) == 1)
                {
                    if (sciReturnHandle(pvApiCtx, getHandle(getConsoleIdentifier())) != 0)    /* Get Console handle */
                    {
                        /* An error has occurred */
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                    ReturnArguments(pvApiCtx);
                    return 0;
                }
                CheckInputArgument(pvApiCtx, 2, 2);
                if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrstkAdr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    // Retrieve a matrix of string at position 2.
                    if (getAllocatedMatrixOfString(pvApiCtx, piAddrstkAdr, &m1, &n1, &stkAdr))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
                        return 1;
                    }


                    if (m1 * n1 != 1)
                    {
                        freeAllocatedMatrixOfString(m1, n1, stkAdr);
                        Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get", 2);
                        return SET_PROPERTY_ERROR;
                    }

                    status = GetScreenProperty(pvApiCtx, stkAdr[0]);

                    if (status != SET_PROPERTY_SUCCEED) /* Return property */
                    {
                        Scierror(999, _("%s: Could not read property '%s' for root object.\n"), "get", stkAdr[0]);
                        freeAllocatedMatrixOfString(m1, n1, stkAdr);
                        return FALSE;
                    }
                    freeAllocatedMatrixOfString(m1, n1, stkAdr);
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get", 2);
                    return FALSE;
                }
                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }
            else                    /* tclsci handle: should no more happen */
            {
                //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
                OverLoad(1);
            }
            return 0;
            break;
        case sci_handles:          /* scalar argument (hdl + string) */
            CheckInputArgument(pvApiCtx, 1, 2);
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

            // Retrieve a matrix of double at position 2.
            if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
                return 1;
            }

            hdl = (long) * l1; /* on recupere le pointeur d'objet par le handle */
            break;
        case sci_strings:          /* string argument (string) */
        {
            int* piAddr = NULL;
            char* pstFirst = NULL;
            CheckInputArgument(pvApiCtx, 1, 2);
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFirst))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
                return 1;
            }

            if (strcmp(pstFirst, "default_figure") != 0 && strcmp(pstFirst, "default_axes") != 0)
            {
                if (strcmp(pstFirst, "current_figure") == 0 || strcmp(pstFirst, "current_axes") == 0 || strcmp(pstFirst, "current_entity") == 0
                        || strcmp(pstFirst, "hdl") == 0 || strcmp(pstFirst, "figures_id") == 0)
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

                        if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
                        {
                            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
                            return 1;
                        }
                    }
                    else
                    {
                        l2 = pstFirst;
                    }
                }
            }
            else
            {
                hdl = 0;
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
        if (sciGet(pvApiCtx, 0, (l2)) != 0)
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

            if (sciGet(pvApiCtx, iObjUID, (l2)) != 0)
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
