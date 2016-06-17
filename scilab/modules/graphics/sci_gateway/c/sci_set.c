/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/* file: sci_set.h                                                        */
/* desc : interface for sci_set routine                                   */
/*------------------------------------------------------------------------*/
#include <stdio.h>
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "freeArrayOfString.h"

#include "SetHashTable.h"
#include "SetPropertyStatus.h"

#include "sci_malloc.h"             /* MALLOC */
#include "localization.h"
#include "os_string.h"
#include "api_scilab.h"
#include "FigureList.h"

/*--------------------------------------------------------------------------
 * sciset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/
int sci_set(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int i = 0;
    int* piAddr1 = NULL;

    int isMatrixOfString = 0;

    char* pstNewProperty = NULL;

    unsigned long hdl;
    int iObjUID = 0;
    int iType = 0;
    int* piType = &iType;

    int iSetProperty = 0;

    int iRhs = nbInputArgument(pvApiCtx);
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

    CheckInputArgumentAtLeast(pvApiCtx, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (isDoubleType(pvApiCtx, piAddr1))   /* tclsci handle */
    {
        /* call "set" for tcl/tk see tclsci/sci_gateway/c/sci_set.c */
        OverLoad(1);
        return 0;
    }

    if (iRhs == 2)
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
        char *propertiesSupported[NB_PROPERTIES_SUPPORTED] =
        {
            "current_entity",
            "hdl",
            "current_figure",
            "current_axes",
            "figure_style",
            "default_values",
            "auto_clear"
        };

        int iPropertyFound = 0;
        int* piAddr2 = NULL;
        int iType2 = 0;
        int iRows2 = 0;
        int iCols2 = 0;
        void* pvData = NULL;
        char* pstProperty = NULL;

        if (isStringType(pvApiCtx, piAddr1) == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr1, &pstProperty))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 1);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstProperty);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 1;
        }

        sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstProperty);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 1;
        }

        switch (iType2)
        {
            case sci_matrix:
                sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, (double**)&pvData);
                if (sciErr.iErr)
                {
                    freeAllocatedSingleString(pstProperty);
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), fname, 2);
                    return sciErr.iErr;
                }
                break;
            case sci_handles :
                sciErr = getMatrixOfHandle(pvApiCtx, piAddr2, &iRows2, &iCols2, (long long**)&pvData);
                if (sciErr.iErr)
                {
                    freeAllocatedSingleString(pstProperty);
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of handle expected.\n"), fname, 3);
                    return 1;
                }
                break;
            case sci_strings :
                if (strcmp(pstProperty, "tics_labels") == 0 || strcmp(pstProperty, "auto_ticks") == 0 ||
                        strcmp(pstProperty, "axes_visible") == 0 || strcmp(pstProperty, "axes_reverse") == 0 ||
                        strcmp(pstProperty, "text") == 0 || strcmp(pstProperty, "ticks_format") == 0)
                {
                    isMatrixOfString = 1;
                    if (getAllocatedMatrixOfString(pvApiCtx, piAddr2, &iRows2, &iCols2, (char***)&pvData))
                    {
                        freeAllocatedSingleString(pstProperty);
                        Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of string expected.\n"), fname, 2);
                        return 1;
                    }
                }
                else
                {
                    if (getAllocatedSingleString(pvApiCtx, piAddr2, (char**)&pvData))
                    {
                        freeAllocatedSingleString(pstProperty);
                        Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 2);
                        return 1;
                    }
                    iRows2 = (int)strlen((char*)pvData);
                    iCols2 = 1;
                    isMatrixOfString = 0;
                }
                break;
        }



        for (i = 0; i < NB_PROPERTIES_SUPPORTED; i++)
        {

            if (strcmp(propertiesSupported[i], pstProperty) == 0)
            {
                iPropertyFound = 1;
            }
        }

        if (iPropertyFound)
        {
            callSetProperty(pvApiCtx, 0, pvData, iType2, iRows2, iCols2, pstProperty);
            if (iType2 == sci_strings)
            {
                //free allocated data
                if (isMatrixOfString == 1)
                {
                    freeAllocatedMatrixOfString(iRows2, iCols2, (char**)pvData);
                }
                else
                {
                    freeAllocatedSingleString((char*)pvData);
                }
            }
        }
        else
        {
            freeAllocatedSingleString(pstProperty);
            Scierror(999, _("%s: Wrong value for input argument #%d: a valid property expected.\n"), fname, 1);
            if (iType2 == sci_strings)
            {
                if (isMatrixOfString == 1)
                {
                    freeAllocatedMatrixOfString(iRows2, iCols2, (char**)pvData);
                }
                else
                {
                    freeAllocatedSingleString((char*)pvData);
                }
            }
            return 0;
        }

        freeAllocatedSingleString(pstProperty);
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (iRhs % 2 != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s) : an odd number is expected.\n"), fname);
        return 0;
    }


    /* after the call to sciSet get the status : 0 <=> OK,          */
    /*                                          -1 <=> Error,       */
    /*                                           1 <=> nothing done */

    /*  set or create a graphic window */
    if (isHandleType(pvApiCtx, piAddr1) == 0 && isStringType(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A handle or a string expected.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr1))
    {
        char* pstPath = NULL;
        if (getAllocatedSingleString(pvApiCtx, piAddr1, &pstPath))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 1);
            return 1;
        }

        iObjUID = search_path(pstPath);
        if (iObjUID == 0)
        {
            Scierror(999, _("%s: Unable to find handle for path %s.\n"), fname, pstPath);
            freeAllocatedSingleString(pstPath);
            return 1;
        }
        freeAllocatedSingleString(pstPath);
    }
    else
    {
        //matrix of handle are managed by a %h_set
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
    }

    if (iObjUID == 0)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return 0;
    }

    for (i = 1 ; i < iRhs ; i = i + 2)
    {
        int setStatus = 0;
        int* piAddr2 = NULL;
        int* piAddr3 = NULL;

        int iPos = i + 1;
        int isData = 0;

        int iRows3 = 0;
        int iCols3 = 0;
        int iType3 = 0;
        void* pvData = NULL;
        char* pstProperty = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr2);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iPos);
            return 1;
        }

        if (isStringType(pvApiCtx, piAddr2) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, iPos);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstProperty))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, iPos);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, iPos + 1, &piAddr3);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iPos + 1);
            freeAllocatedSingleString(pstProperty);
            return 1;
        }

        if ((pstProperty[0] == 'd' || pstProperty[0] == 'D') && stricmp("data", pstProperty) == 0)
        {
            //send to datamodel
            isData = 1;
        }

        if (stricmp(pstProperty, "user_data") == 0 ||
                stricmp(pstProperty, "userdata") == 0 ||
                stricmp(pstProperty, "display_function_data") == 0 ||
                stricmp(pstProperty, "data") == 0)
        {
            /* in this case set_user_data_property
            * directly uses the  third position in the stack
            * to get the variable which is to be set in
            * the user_data property (any data type is allowed) S. Steer */
            pvData = (void*)piAddr3;         /*position in the stack */
            iRows3 = -1;   /*unused */
            iCols3 = -1;   /*unused */
            iType3 = -1;
        }
        else
        {
            sciErr = getVarType(pvApiCtx, piAddr3, &iType3);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iPos + 1);
                freeAllocatedSingleString(pstProperty);
                return 1;
            }

            switch (iType3)
            {
                case sci_matrix :
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows3, &iCols3, (double**)&pvData);
                    break;
                case sci_boolean :
                    sciErr = getMatrixOfBoolean(pvApiCtx, piAddr3, &iRows3, &iCols3, (int**)&pvData);
                    break;
                case sci_handles :
                    sciErr = getMatrixOfHandle(pvApiCtx, piAddr3, &iRows3, &iCols3, (long long**)&pvData);
                    break;
                case sci_strings :
                    if (strcmp(pstProperty, "tics_labels") != 0 && strcmp(pstProperty, "auto_ticks") != 0 && strcmp(pstProperty, "tight_limits") != 0 &&
                            strcmp(pstProperty, "axes_visible") != 0 && strcmp(pstProperty, "axes_reverse") != 0 &&
                            strcmp(pstProperty, "text") != 0 && stricmp(pstProperty, "string") != 0 &&
                            stricmp(pstProperty, "tooltipstring") != 0 && stricmp(pstProperty, "ticks_format") != 0) /* Added for uicontrols */
                    {
                        if (isScalar(pvApiCtx, piAddr3) == 0)
                        {
                            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, iPos + 1);
                            freeAllocatedSingleString(pstProperty);
                            return 1;
                        }

                        if (getAllocatedSingleString(pvApiCtx, piAddr3, (char**)&pvData))
                        {
                            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iPos + 1);
                            freeAllocatedSingleString(pstProperty);
                            return 1;
                        }
                        iRows3 = (int)strlen((char*)pvData);
                        iCols3 = 1;
                        isMatrixOfString = 0;
                    }
                    else
                    {
                        isMatrixOfString = 1;
                        if (getAllocatedMatrixOfString(pvApiCtx, piAddr3, &iRows3, &iCols3, (char***)&pvData))
                        {
                            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iPos + 1);
                            freeAllocatedSingleString(pstProperty);
                            return 1;
                        }
                    }
                    break;
                case sci_list :
                    iCols3 = 1;
                    sciErr = getListItemNumber(pvApiCtx, piAddr3, &iRows3);
                    pvData = (void*)piAddr3;         /* In this case l3 is the list position in stack */
                    break;
                default :
                    pvData = (void*)piAddr3;         /* In this case l3 is the list position in stack */
                    break;
            }

            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iPos + 1);
                freeAllocatedSingleString(pstProperty);
                if (isMatrixOfString == 1)
                {
                    freeAllocatedMatrixOfString(iRows3, iCols3, (char**)pvData);
                }
                else
                {
                    freeAllocatedSingleString((char*)pvData);
                }
                return 1;
            }
        }

        setStatus = callSetProperty(pvApiCtx, iObjUID, pvData, iType3, iRows3, iCols3, pstProperty);
        if (iType3 == sci_strings)
        {
            //free allacted data
            if (isMatrixOfString == 1)
            {
                freeAllocatedMatrixOfString(iRows3, iCols3, (char**)pvData);
            }
            else
            {
                freeAllocatedSingleString((char*)pvData);
            }
        }

        freeAllocatedSingleString(pstProperty);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
