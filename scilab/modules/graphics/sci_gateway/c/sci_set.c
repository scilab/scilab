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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        //error
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
        char *pobjUID = NULL;

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
            //error
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

                getScalarHandle(pvApiCtx, piAddr1, (long long*)&hdl);
                pobjUID = (char*)getObjectFromHandle(hdl);

                getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
                getAllocatedSingleString(pvApiCtx, piAddr2, &pstProperty);
                valueType = getInputArgumentType(pvApiCtx, 3);

                getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
                if ((strcmp(pstProperty, "user_data") == 0) || (stricmp(pstProperty, "userdata") == 0))
                {
                    /* in this case set_user_data_property
                     * directly uses the  third position in the stack
                     * to get the variable which is to be set in
                     * the user_data property (any data type is allowed) S. Steer */
                    _pvData = (void*)piAddr3;         /*position in the stack */
                    iRows3 = -1;   /*unused */
                    iCols3 = -1;   /*unused */
                }
                else if (valueType == sci_matrix)
                {
                    getMatrixOfDouble(pvApiCtx, piAddr3, &iRows3, &iCols3, (double**)&_pvData);
                }
                else if (valueType == sci_boolean)
                {
                    getMatrixOfBoolean(pvApiCtx, piAddr3, &iRows3, &iCols3, (int**)&_pvData);
                }
                else if (valueType == sci_handles)
                {
                    getMatrixOfHandle(pvApiCtx, piAddr3, &iRows3, &iCols3, (long long**)&_pvData);
                }
                else if (valueType == sci_strings)
                {
                    if (   strcmp(pstProperty, "tics_labels") != 0 && strcmp(pstProperty, "auto_ticks") != 0 &&
                            strcmp(pstProperty, "axes_visible") != 0 && strcmp(pstProperty, "axes_reverse") != 0 &&
                            strcmp(pstProperty, "text") != 0 && stricmp(pstProperty, "string") != 0 &&
                            stricmp(pstProperty, "tooltipstring") != 0) /* Added for uicontrols */
                    {
                        getAllocatedSingleString(pvApiCtx, piAddr3, (char**)&_pvData);
                        iRows3 = (int)strlen((char*)_pvData);
                        iCols3 = 1;
                    }
                    else
                    {
                        isMatrixOfString = 1;
                        getAllocatedMatrixOfString(pvApiCtx, piAddr3, &iRows3, &iCols3, (char***)&_pvData);
                    }
                }
                else if (valueType == sci_list) /* Added for callbacks */
                {
                    iCols3 = 1;
                    getListItemNumber(pvApiCtx, piAddr3, &iRows3);
                    _pvData = (void*)piAddr3;         /* In this case l3 is the list position in stack */
                }
                break;

            case sci_strings:      /* first is a string argument so it's a set("command",[param]) */
                CheckRhs(2, 2);
                getAllocatedSingleString(pvApiCtx, piAddr1, &pstProperty);
                hdl = 0;
                pobjUID = NULL;
                valueType = getInputArgumentType(pvApiCtx, 2);
                getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);

                if (valueType == sci_matrix)
                {
                    getMatrixOfDouble(pvApiCtx, piAddr2, &iRows3, &iCols3, (double**)&_pvData);
                }
                else if (valueType == sci_handles)
                {
                    getMatrixOfHandle(pvApiCtx, piAddr2, &iRows3, &iCols3, (long long**)&_pvData);
                }
                else if (valueType == sci_strings)
                {
                    if (strcmp(pstProperty, "tics_labels") == 0 || strcmp(pstProperty, "auto_ticks") == 0 ||
                            strcmp(pstProperty, "axes_visible") == 0 || strcmp(pstProperty, "axes_reverse") == 0 ||
                            strcmp(pstProperty, "text") == 0)
                    {
                        isMatrixOfString = 1;
                        getAllocatedMatrixOfString(pvApiCtx, piAddr2, &iRows3, &iCols3, (char***)&_pvData);
                    }
                    else
                    {
                        getAllocatedSingleString(pvApiCtx, piAddr2, (char**)&_pvData);
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
            pobjUID = (char*)getObjectFromHandle(hdl);

            if (pobjUID == NULL)
            {
                Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                return 0;
            }

            // Only set the property whitout doing anythig else.
            //static int sciSet(void* _pvCtx, char *pobjUID, char *marker, void* value, int valueType, int *numrow, int *numcol)
            setStatus = callSetProperty(pvApiCtx, pobjUID, _pvData, valueType, iRows3, iCols3, pstProperty);
            if (valueType == sci_strings)
            {
                //free allocated data
                if (isMatrixOfString == 1)
                {
                    freeAllocatedMatrixOfString(iRows3, iCols3, (char**)_pvData);
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
                // we do nothing with "figure_style" "new" (to remove in 5.4)
                int bDoSet = ((isMatrixOfString) && (strcmp(pstProperty, "figure_style") == 0) && (strcmp(((char**)_pvData)[0], "new") == 0)) != 1;

                if (bDoSet)
                {
                    setStatus = callSetProperty(pvApiCtx, NULL, _pvData, valueType, iRows3, iCols3, pstProperty);
                    if (valueType == sci_strings)
                    {
                        //free allocated data
                        freeAllocatedMatrixOfString(iRows3, iCols3, (char**)_pvData);
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
