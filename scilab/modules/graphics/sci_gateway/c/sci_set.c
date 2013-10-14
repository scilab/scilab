/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_set.h                                                        */
/* desc : interface for sci_set routine                                   */
/*------------------------------------------------------------------------*/
#include <stdio.h>
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
#include "stack-c.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "freeArrayOfString.h"

#include "SetHashTable.h"
#include "SetPropertyStatus.h"

#include "MALLOC.h"             /* MALLOC */
#include "localization.h"
#include "stricmp.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------
 * sciset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/
int sci_set(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int lw = 0;
    int isMatrixOfString = 0;

    char* pstProperty = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if (isMListType(pvApiCtx, piAddr1) || isTListType(pvApiCtx, piAddr1))
    {
        OverLoad(1);
        return 0;
    }

    CheckRhs(2, 3);
    CheckLhs(0, 1);

    if (isDoubleType(pvApiCtx, piAddr1))   /* tclsci handle */
    {
        /* call "set" for tcl/tk see tclsci/sci_gateway/c/sci_set.c */
        OverLoad(1);
        return 0;
    }
    else                        /* others types */
    {
        int iRows1 = 0, iCols1 = 0;
        int iRows2 = 0, iCols2 = 0;
        int iRows3 = 0, iCols3 = 0;
        void* _pvData = NULL;
        unsigned long hdl;
        int iObjUID = 0;

        int iType1 = 0;

        int valueType = 0;      /* type of the rhs */

        int setStatus = 0;

        /* after the call to sciSet get the status : 0 <=> OK,          */
        /*                                          -1 <=> Error,       */
        /*                                           1 <=> nothing done */

        /*  set or create a graphic window */
        sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not get type of input argument #%d.\n"), fname, 1);
            return 1;
        }

        switch (iType1)
        {
            case sci_handles:
                /* first is a scalar argument so it's a gset(hdl,"command",[param]) */
                /* F.Leray; INFO: case 9 is considered for a matrix of graphic handles */
                CheckRhs(3, 3);

                if (isScalar(pvApiCtx, piAddr1) == FALSE)
                {
                    OverLoad(1);
                    return 0;
                }

                if (getScalarHandle(pvApiCtx, piAddr1, (long long*)&hdl))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single handle expected.\n"), fname, 1);
                    return 1;
                }

                iObjUID = getObjectFromHandle(hdl);

                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstProperty))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 2);
                    return 1;
                }
                valueType = getInputArgumentType(pvApiCtx, 3);

                sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 1;
                }

                if ((strcmp(pstProperty, "user_data") == 0) || (stricmp(pstProperty, "userdata") == 0))
                {
                    /* in this case set_user_data_property
                     * directly uses the  third position in the stack
                     * to get the variable which is to be set in
                     * the user_data property (any data type is allowed) S. Steer */
                    _pvData = (void*)piAddr3;         /*position in the stack */
                    iRows3 = -1;   /*unused */
                    iCols3 = -1;   /*unused */
                    valueType = -1;
                }
                else
                {
                    switch (valueType)
                    {
                        case sci_matrix :
                            getMatrixOfDouble(pvApiCtx, piAddr3, &iRows3, &iCols3, (double**)&_pvData);
                            break;
                        case sci_boolean :
                            getMatrixOfBoolean(pvApiCtx, piAddr3, &iRows3, &iCols3, (int**)&_pvData);
                            break;
                        case sci_handles :
                            getMatrixOfHandle(pvApiCtx, piAddr3, &iRows3, &iCols3, (long long**)&_pvData);
                            break;
                        case sci_strings :
                            if (   strcmp(pstProperty, "tics_labels") != 0 && strcmp(pstProperty, "auto_ticks") != 0 &&
                                    strcmp(pstProperty, "axes_visible") != 0 && strcmp(pstProperty, "axes_reverse") != 0 &&
                                    strcmp(pstProperty, "text") != 0 && stricmp(pstProperty, "string") != 0 &&
                                    stricmp(pstProperty, "tooltipstring") != 0) /* Added for uicontrols */
                            {
                                if (getAllocatedSingleString(pvApiCtx, piAddr3, (char**)&_pvData))
                                {
                                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 3);
                                    return 1;
                                }
                                iRows3 = (int)strlen((char*)_pvData);
                                iCols3 = 1;
                            }
                            else
                            {
                                isMatrixOfString = 1;
                                getAllocatedMatrixOfString(pvApiCtx, piAddr3, &iRows3, &iCols3, (char***)&_pvData);
                            }
                            break;
                        case sci_list :
                            iCols3 = 1;
                            getListItemNumber(pvApiCtx, piAddr3, &iRows3);
                            _pvData = (void*)piAddr3;         /* In this case l3 is the list position in stack */
                            break;
                        default :
                            _pvData = (void*)piAddr3;         /* In this case l3 is the list position in stack */
                            break;
                    }
                }
                break;
            case sci_strings:      /* first is a string argument so it's a set("command",[param]) */
                CheckRhs(2, 2);
                if (getAllocatedSingleString(pvApiCtx, piAddr1, &pstProperty))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 1);
                    return 1;
                }

                hdl = 0;
                iObjUID = 0;
                valueType = getInputArgumentType(pvApiCtx, 2);
                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                    return 1;
                }

                if (valueType == sci_matrix)
                {
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows3, &iCols3, (double**)&_pvData);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), fname, 2);
                        return sciErr.iErr;
                    }
                }
                else if (valueType == sci_handles)
                {
                    sciErr = getMatrixOfHandle(pvApiCtx, piAddr2, &iRows3, &iCols3, (long long**)&_pvData);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of handle expected.\n"), fname, 3);
                        return 1;
                    }
                }
                else if (valueType == sci_strings)
                {
                    if (strcmp(pstProperty, "tics_labels") == 0 || strcmp(pstProperty, "auto_ticks") == 0 ||
                            strcmp(pstProperty, "axes_visible") == 0 || strcmp(pstProperty, "axes_reverse") == 0 ||
                            strcmp(pstProperty, "text") == 0)
                    {
                        isMatrixOfString = 1;
                        if (getAllocatedMatrixOfString(pvApiCtx, piAddr2, &iRows3, &iCols3, (char***)&_pvData))
                        {
                            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of string expected.\n"), fname, 2);
                            return 1;
                        }
                    }
                    else
                    {
                        if (getAllocatedSingleString(pvApiCtx, piAddr2, (char**)&_pvData))
                        {
                            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 2);
                            return 1;
                        }
                        iRows3 = (int)strlen((char*)_pvData);
                        iCols3 = 1;
                    }
                }
                break;

            default:
                Scierror(999, _("%s: Wrong type for input argument #%d: String or handle expected.\n"), fname, 1);
                return 0;
                break;
        }

        if (hdl != 0)
        {
            iObjUID = getObjectFromHandle(hdl);

            if (iObjUID == 0)
            {
                Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                return 0;
            }

            // Only set the property whitout doing anythig else.
            //static int sciSet(void* _pvCtx, int iObjUID, char *marker, void* value, int valueType, int *numrow, int *numcol)
            setStatus = callSetProperty(pvApiCtx, iObjUID, _pvData, valueType, iRows3, iCols3, pstProperty);
            if (valueType == sci_strings)
            {
                //free allacted data
                if (isMatrixOfString == 1)
                {
                    freeAllocatedMatrixOfString(iRows3, iCols3, (char**)_pvData);
                }
                else
                {
                    freeAllocatedSingleString((char*)_pvData);
                }
            }
        }
        else
        {
#define NB_PROPERTIES_SUPPORTED 7
            /* No object specified */
            /* ONLY supported properties are */
            /* 'current_entity' */
            /* 'hdl' */
            /* 'current_figure' */
            /* 'current_axes' */
            /* 'default_values' */
            /* 'figure_style' for compatibility but do nothing */
            /* others values must return a error */
            char *propertiesSupported[NB_PROPERTIES_SUPPORTED] = { "current_entity",
                    "hdl",
                    "current_figure",
                    "current_axes",
                    "figure_style",
                    "default_values",
                    "auto_clear"
                                                                 };

            int i = 0;
            int iPropertyFound = 0;

            for (i = 0; i < NB_PROPERTIES_SUPPORTED; i++)
            {

                if (strcmp(propertiesSupported[i], pstProperty) == 0)
                {
                    iPropertyFound = 1;
                }
            }

            if (iPropertyFound)
            {
                setStatus = callSetProperty(pvApiCtx, NULL, _pvData, valueType, iRows3, iCols3, pstProperty);
                if (valueType == sci_strings)
                {
                    //free allocated data
                    if (isMatrixOfString == 1)
                    {
                        freeAllocatedMatrixOfString(iRows3, iCols3, (char**)_pvData);
                    }
                    else
                    {
                        freeAllocatedSingleString((char*)_pvData);
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: a valid property expected.\n"), fname, 1);
                if (isMatrixOfString)
                {
                    freeArrayOfString((char **)_pvData, iRows3 * iCols3);
                }
                return 0;
            }
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
