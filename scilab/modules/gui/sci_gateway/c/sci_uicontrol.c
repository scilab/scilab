/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
#include "CreateUIControl.h"
#include "HandleManagement.h"
#include "MALLOC.h"             /* MALLOC */
#include "localization.h"
#include "stricmp.h"
#include "stack-c.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "Scierror.h"
#include "FigureList.h"         /* getFigureFromIndex */
#include "Widget.h"             /* requestWidgetFocus */
#include "freeArrayOfString.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
#include "api_scilab.h"
#include "createGraphicObject.h"

/*--------------------------------------------------------------------------*/
// callSetProperty get a stack pointer in input argument.
/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int nbRow = 0;
    int nbCol = 0;
    int k = 0;
    int setStatus = SET_PROPERTY_SUCCEED;
    int beginIndex = 0;
    BOOL bSetVisible = FALSE;
    char *styleProperty = NULL;
    BOOL bSetProperty = FALSE;

    unsigned long GraphicHandle = 0;

    int iParentType = -1;
    int *piParentType = &iParentType;
    int iParentStyle = -1;
    int *piParentStyle = &iParentStyle;

    int iParentUID      = 0;
    int iUicontrol      = 0;
    int iCurrentFigure  = 0;

    int iRhs = nbInputArgument(pvApiCtx);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (iRhs == 0)
    {
        /* Create a pushbutton in current figure */
        iUicontrol = CreateUIControl(NULL);
        GraphicHandle = getHandle(iUicontrol);

        /* Set current figure as parent */
        iCurrentFigure = getCurrentFigure();
        if (iCurrentFigure == 0)
        {
            iCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(iCurrentFigure, iUicontrol);
    }
    else if (iRhs == 1)
    {
        /* Create a pushbutton in figure given as parameter */
        /* Or give focus to the uicontrol given as parameter */
        int* piAddr = NULL;
        long long hParent = 0;

        if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
        {
            OverLoad(1);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isScalar(pvApiCtx, piAddr) == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarHandle(pvApiCtx, piAddr, &hParent))
        {
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 0;
        }

        iParentUID = getObjectFromHandle((long)hParent);
        if (iParentUID == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s', '%s' or '%s' handle expected.\n"), fname, 1, "Uicontrol", "Figure");
            return 0;
        }

        getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
        if (iParentType == __GO_UICONTROL__)  /* Focus management */
        {
            GraphicHandle = (unsigned long)hParent;
            requestFocus(iParentUID);
        }
        else if (iParentType == __GO_FIGURE__)  /* PushButton creation */
        {
            /* Create a new pushbutton */
            iUicontrol = CreateUIControl(NULL);
            GraphicHandle = getHandle(iUicontrol);

            /* First parameter is the parent */
            setGraphicObjectRelationship(iParentUID, iUicontrol);
            //setStatus = callSetProperty(pvApiCtx, iUicontrol, &hParent, sci_handles, 1, 1, "parent");
            //if (setStatus == SET_PROPERTY_ERROR)
            //{
            //    Scierror(999, _("%s: Could not set property '%s'.\n"), fname, "parent");
            //    return 0;
            //}
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Uicontrol", "Figure");
            return 0;
        }

        iUicontrol = getObjectFromHandle(GraphicHandle);
    }
    else
    {
        /**
         * Odd number of input arguments
         * First input is the parent ID
         * All event inputs are property names
         * All odd (except first) inputs are property values
         */
        if (iRhs % 2 == 1)
        {
            int* piAddr = NULL;
            long long hParent = 0;

            if (checkInputArgumentType(pvApiCtx, 1, sci_handles) == FALSE)
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 0;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if (isScalar(pvApiCtx, piAddr) == 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure", "Frame uicontrol");
                return 0;
            }

            if (getScalarHandle(pvApiCtx, piAddr, &hParent))
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 0;
            }

            iParentUID = getObjectFromHandle((long)hParent);
            if (iParentUID == 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure", "Frame uicontrol");
                return 0;
            }

            getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
            if (iParentType != __GO_FIGURE__)
            {
                getGraphicObjectProperty(iParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
                if (iParentType != __GO_UICONTROL__ || iParentStyle != __GO_UI_FRAME__)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure", "Frame uicontrol");
                    return 0;
                }
            }

            // First input parameter which is a property name
            beginIndex = 1;
        }

        /* Get all properties positions */
        for (k = beginIndex; k < Rhs; k += 2)
        {
            int iPos = k + 1;
            /* Read property name */
            int* piAddr = NULL;
            int* piAddr2 = NULL;
            char* pstProperty = NULL;
            char* pstStyle = NULL;

            sciErr = getVarAddressFromPosition(pvApiCtx, iPos, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if (isStringType(pvApiCtx, piAddr) == 0 || isScalar(pvApiCtx, piAddr) == 0)
            {
                cleanSetProperty();
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, iPos);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddr, &pstProperty))
            {
                cleanSetProperty();
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, iPos);
                return 0;
            }

            iPos++;
            getVarAddressFromPosition(pvApiCtx, iPos, &piAddr2);

            //manage style property
            if (stricmp(pstProperty, "style") == 0)
            {
                if (isStringType(pvApiCtx, piAddr2) == 0 || isScalar(pvApiCtx, piAddr2) == 0)
                {
                    cleanSetProperty();
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, iPos);
                    return 0;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstStyle))
                {
                    cleanSetProperty();
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, iPos);
                    return 0;
                }

                iUicontrol = CreateUIControl(pstStyle);
                GraphicHandle = getHandle(iUicontrol);

                /* Set current figure as parent */
                iCurrentFigure = getCurrentFigure();
                if (iCurrentFigure == 0)
                {
                    iCurrentFigure = createNewFigureWithAxes();
                }
                setGraphicObjectRelationship(iCurrentFigure, iUicontrol);

                freeAllocatedSingleString(pstStyle);
                if (iUicontrol == 0) /* Error in creation */
                {
                    cleanSetProperty();
                    Scierror(999, _("%s: Could not create 'Uicontrol' handle.\n"), fname);
                    return 0;
                }

                GraphicHandle = getHandle(iUicontrol);
            }
            else
            {
                if (stricmp(pstProperty, "visible") == 0)
                {
                    bSetVisible = TRUE;
                }

                if (setGraphicObjectVariable(pvApiCtx, pstProperty, piAddr2) == FALSE)
                {
                    cleanSetProperty();
                    Scierror(999, _("%s: Could not set property '%s'.\n"), fname, pstProperty);
                    return 0;
                }

                bSetProperty = TRUE;
            }
        }

        if (iUicontrol == 0)
        {
            iUicontrol = CreateUIControl(NULL);
            GraphicHandle = getHandle(iUicontrol);

            /* Set current figure as parent */
            iCurrentFigure = getCurrentFigure();
            if (iCurrentFigure == 0)
            {
                iCurrentFigure = createNewFigureWithAxes();
            }
            setGraphicObjectRelationship(iCurrentFigure, iUicontrol);
        }

        if (bSetProperty)
        {
            if (runSetProperty(getObjectFromHandle(GraphicHandle)) == FALSE)
            {
                char** lastError = getPropertyLastError();
                Scierror(999, lastError[1], fname, lastError[0]);
                freePropertyLastError(lastError);
                return 0;
            }
        }
    }

    //if visible is not set by input arguments, force to visible = "on"
    if (bSetVisible == FALSE)
    {
        int iVisible = 1;
        setGraphicObjectProperty(iUicontrol, __GO_VISIBLE__, &iVisible, jni_bool, 1);
    }

    /* Create return variable */
    if (createScalarHandle(pvApiCtx, iRhs + 1, GraphicHandle))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
