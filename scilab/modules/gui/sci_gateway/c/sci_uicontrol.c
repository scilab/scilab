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

/* DO NOT CHANGE ORDER !! */
static const char* propertiesNames[] =
{
    "style",
    "parent",
    "backgroundcolor",
    "foregroundcolor",
    "string",
    "units",
    "fontweight",
    "min",
    "max",
    "tag",
    "position",
    "relief",
    "horizontalalignment",
    "verticalalignment",
    "sliderstep",
    "fontname",
    "callback",
    "fontangle",
    "fontunits",
    "fontsize",
    "listboxtop",
    "user_data",
    "value",
    "userdata",
    "visible",
    "enable",
    "callback_type",
    "tooltipstring"
};

/*--------------------------------------------------------------------------*/
//#define NBPROPERTIES 28
//#define MAXPROPERTYNAMELENGTH 20
/*--------------------------------------------------------------------------*/
// callSetProperty get a stack pointer in input argument.
/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int nbRow = 0, nbCol = 0, k = 0;
    int setStatus = SET_PROPERTY_SUCCEED;
    int NOT_FOUND = -1;
    int inputIndex = 0, beginIndex = 0;
    char *propertyName = NULL;
    char *styleProperty = NULL;
    char *pParentUID = NULL;
    char *pUicontrol = NULL;

    int iPropertiesCount = sizeof(propertiesNames) / sizeof(char**);
    unsigned long GraphicHandle = 0;

    int found = 0;              /* Does the property exists ? */

    int *propertiesValuesIndices = NULL;

    int iParentType = -1;
    int *piParentType = &iParentType;
    int iParentStyle = -1;
    int *piParentStyle = &iParentStyle;
    char const* pstCurrentFigure = NULL;

    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        /* Create a pushbutton in current figure */

        /* Create a new pushbutton */
        GraphicHandle = getHandle(CreateUIControl(NULL));

        /* Set current figure as parent */
        pstCurrentFigure = getCurrentFigure();
        if (pstCurrentFigure == NULL)
        {
            pstCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(pstCurrentFigure, (char*)getObjectFromHandle(GraphicHandle));
        pUicontrol = (char*)getObjectFromHandle(GraphicHandle);
    }
    else if (nbInputArgument(pvApiCtx) == 1)
    {
        /* Create a pushbutton in figure given as parameter */
        /* Or give focus to the uicontrol given as parameter */

        if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
        {
            OverLoad(1);
            return FALSE;
        }
        else /* Get parent ID */
        {
            int* piAddr = NULL;
            long long hParent = 0;
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if (isScalar(pvApiCtx, piAddr) == 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }

            if (getScalarHandle(pvApiCtx, piAddr, &hParent))
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 1;
            }

            pParentUID = (char*)getObjectFromHandle((long)hParent);
            if (pParentUID != NULL)
            {
                getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
                if (iParentType == __GO_UICONTROL__)  /* Focus management */
                {
                    GraphicHandle = (unsigned long)hParent;
                    requestFocus(pParentUID);
                }
                else if (iParentType == __GO_FIGURE__ || iParentType == __GO_UIMENU__)  /* PushButton creation */
                {
                    /* Create a new pushbutton */
                    GraphicHandle = getHandle(CreateUIControl(NULL));

                    /* First parameter is the parent */
                    setGraphicObjectRelationship(pParentUID, getObjectFromHandle(GraphicHandle));
                    setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), &hParent, sci_handles, 1, 1, (char*)propertiesNames[1]);
                    if (setStatus == SET_PROPERTY_ERROR)
                    {
                        Scierror(999, _("%s: Could not set property '%s'.\n"), fname, propertyName);
                        return FALSE;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s', '%s' or '%s' handle expected.\n"), fname, 1, "Uicontrol",
                             "Figure", "Uimenu");
                    return FALSE;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s', '%s' or '%s' handle expected.\n"), fname, 1, "Uicontrol", "Figure",
                         "Uimenu");
                return FALSE;
            }
            pUicontrol = (char*)getObjectFromHandle(GraphicHandle);
        }
    }
    else
    {
        if (!checkInputArgumentType(pvApiCtx, 1, sci_handles) && !checkInputArgumentType(pvApiCtx, 1, sci_strings))
        {
            OverLoad(1);
            return FALSE;
        }

        /* Allocate memory to store the position of properties in uicontrol call */
        if ((propertiesValuesIndices = (int*)MALLOC(sizeof(int) * iPropertiesCount)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return FALSE;
        }

        /* Init all positions to NOT_FOUND */
        for (inputIndex = 0; inputIndex < iPropertiesCount; inputIndex++)
        {
            propertiesValuesIndices[inputIndex] = NOT_FOUND;    /* Property initialized as not found */
        }

        /**
         * Odd number of input arguments
         * First input is the parent ID
         * All event inputs are property names
         * All odd (except first) inputs are property values
         */
        if (nbInputArgument(pvApiCtx) % 2 == 1)
        {
            if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
            {
                if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)))
                {
                    int* piAddr = NULL;
                    double dblValue = 0;

                    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if (isScalar(pvApiCtx, piAddr) == 0)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                        return FALSE;
                    }

                    if (getScalarDouble(pvApiCtx, piAddr, &dblValue))
                    {
                        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
                        return 1;
                    }

                    pParentUID = (char*)getFigureFromIndex((int)dblValue);

                    if (pParentUID == NULL)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                                 "Frame uicontrol");
                        return FALSE;
                    }

                    getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
                    if (iParentType != __GO_FIGURE__)
                    {
                        getGraphicObjectProperty(pParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
                        if (iParentType != __GO_UICONTROL__ || iParentStyle != __GO_UI_FRAME__)
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                                     "Frame uicontrol");
                            return FALSE;
                        }
                    }
                    /* First parameter is the parent */
                    propertiesValuesIndices[1] = 1;
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    return FALSE;
                }
            }
            else /* Get parent ID */
            {
                int* piAddr = NULL;
                long long hParent = 0;
                sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (isScalar(pvApiCtx, piAddr) == 0)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    return FALSE;
                }

                if (getScalarHandle(pvApiCtx, piAddr, &hParent))
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                    return 1;
                }

                pParentUID = (char*)getObjectFromHandle((long)hParent);
                if (pParentUID == NULL)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    return FALSE;
                }
                getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
                if (iParentType != __GO_FIGURE__)
                {
                    getGraphicObjectProperty(pParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
                    if (iParentType != __GO_UICONTROL__ || iParentStyle != __GO_UI_FRAME__)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                                 "Frame uicontrol");
                        return FALSE;
                    }
                }
                /* First parameter is the parent */
                propertiesValuesIndices[1] = 1;
            }

            // First input parameter which is a property name
            beginIndex = 2;
        }
        /**
         * Even number of input arguments
         * All odd inputs are property names
         * All even inputs are property values
         */
        else
        {
            // First input parameter which is a property name
            beginIndex = 1;
        }

        /* Get all properties positions */
        for (inputIndex = beginIndex; inputIndex < Rhs; inputIndex = inputIndex + 2)
        {
            /* Read property name */
            if ((!checkInputArgumentType(pvApiCtx, inputIndex, sci_strings)))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, inputIndex);
                return FALSE;
            }
            else
            {
                int* piAddr = NULL;
                sciErr = getVarAddressFromPosition(pvApiCtx, inputIndex, &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddr, &propertyName))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, inputIndex);
                    return 1;
                }


                /* Bug 3031 */
                /* We only want to compare propertyName along its length */
                /* 'posi' must be matched to 'position' */
                found = 0;
                for (k = 0; k < iPropertiesCount ; k++)
                {
                    if (strlen(propertyName) <= strlen(propertiesNames[k]))
                    {
                        if (strnicmp(propertyName, propertiesNames[k], strlen(propertyName)) == 0)
                        {
                            propertiesValuesIndices[k] = inputIndex + 1;    /* Position of value for property */
                            found = 1;
                            break;
                        }
                    }
                }

                if (found == 0)
                {
                    Scierror(999, _("%s: Unknown property: %s for '%s' handles.\n"), fname, propertyName, "Uicontrol");
                    return FALSE;
                }
            }
        }

        if (propertiesValuesIndices[0] != NOT_FOUND)    /* Style found */
        {
            if ((checkInputArgumentType(pvApiCtx, propertiesValuesIndices[0], sci_strings)))
            {
                int* piAddr = NULL;
                sciErr = getVarAddressFromPosition(pvApiCtx, propertiesValuesIndices[0], &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddr, &styleProperty))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, propertiesValuesIndices[0]);
                    return 1;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, propertiesValuesIndices[0]);
                return FALSE;
            }
        }

        /* Create a new uicontrol */
        pUicontrol = CreateUIControl(styleProperty);
        freeAllocatedSingleString(styleProperty);
        if (pUicontrol == NULL) /* Error in creation */
        {
            Scierror(999, _("%s: Could not create 'Uicontrol' handle.\n"), fname);
            return FALSE;
        }
        GraphicHandle = getHandle(pUicontrol);

        /* If no parent given then the current figure is the parent */
        if (propertiesValuesIndices[1] == NOT_FOUND)
        {
            char *graphicObjectUID = (char*)getObjectFromHandle(GraphicHandle);

            /* Set the parent */
            pstCurrentFigure = (char*)getCurrentFigure();

            if (pstCurrentFigure == NULL)
            {
                pstCurrentFigure = createNewFigureWithAxes();
            }
            setGraphicObjectRelationship(pstCurrentFigure, graphicObjectUID);
        }

        /* Read and set all properties */
        for (inputIndex = 1; inputIndex < iPropertiesCount; inputIndex++)   /* Style has already been set */
        {
            if (propertiesValuesIndices[inputIndex] != NOT_FOUND)
            {
                int* piAddr = NULL;
                sciErr = getVarAddressFromPosition(pvApiCtx, propertiesValuesIndices[inputIndex], &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (inputIndex == 21 || inputIndex == 23)   /* User data settings */
                {
                    nbRow = -1;
                    nbCol = -1;
                    setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), piAddr, 0, 0, 0, (char*)propertiesNames[inputIndex]);
                }
                else            /* All other properties */
                {
                    /* Read property value */
                    switch (getInputArgumentType(pvApiCtx, propertiesValuesIndices[inputIndex]))
                    {
                        case sci_matrix:
                        {
                            double* pdblValue = NULL;
                            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &pdblValue);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                return 1;
                            }

                            setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), pdblValue, sci_matrix, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                            break;
                        }
                        case sci_strings:
                            /* Index for String & TooltipString properties: Can be more than one character string */
                            if ((inputIndex == 4) || (inputIndex == 27))
                            {
                                char** pstValue = NULL;
                                if (getAllocatedMatrixOfString(pvApiCtx, piAddr, &nbRow, &nbCol, &pstValue))
                                {
                                    Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                    return 1;
                                }

                                setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), pstValue, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                                freeAllocatedMatrixOfString(nbRow, nbCol, pstValue);
                            }
                            else
                            {
                                char* pstValue = NULL;
                                if (getAllocatedSingleString(pvApiCtx, piAddr, &pstValue))
                                {
                                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                    return 1;
                                }

                                nbRow = (int)strlen(pstValue);
                                nbCol = 1;
                                setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), pstValue, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                                freeAllocatedSingleString(pstValue);
                            }
                            break;
                        case sci_handles:
                        {
                            long long* pHandles = NULL;
                            sciErr = getMatrixOfHandle(pvApiCtx, piAddr, &nbRow, &nbCol, &pHandles);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                return 1;
                            }

                            setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), pHandles, sci_handles, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                            break;
                        }
                        default:
                            setStatus = SET_PROPERTY_ERROR;
                            break;
                    }
                }
                if (setStatus == SET_PROPERTY_ERROR)
                {
                    Scierror(999, _("%s: Could not set property '%s'.\n"), fname, (char*)propertiesNames[inputIndex]);
                    return FALSE;
                }
            }
        }
    }

    if ((nbInputArgument(pvApiCtx) < 2) || (propertiesValuesIndices[24] == NOT_FOUND))    /* Visible property not set */
    {
        /* Force the uicontrol to be visible because is invisible by default in the model (See bug #10346) */
        int b = (int)TRUE;
        setGraphicObjectProperty(pUicontrol, __GO_VISIBLE__, &b, jni_bool, 1);
    }

    if ((nbInputArgument(pvApiCtx) < 2) || (propertiesValuesIndices[14] == NOT_FOUND))    /* SliderStep property not set */
    {
        /* Set SliderStep property to [1/100*(Max-Min) 1/10*(Max-Min)] */
        double maxValue = 0;
        double* pdblMaxValue = &maxValue;
        double minValue = 0;
        double* pdblMinValue = &minValue;
        double pdblStep[2];

        getGraphicObjectProperty(pUicontrol, __GO_UI_MIN__, jni_double, (void**) &pdblMinValue);
        getGraphicObjectProperty(pUicontrol, __GO_UI_MAX__, jni_double, (void**) &pdblMaxValue);

        pdblStep[0] = 0.01 * (maxValue - minValue);
        pdblStep[1] = 0.1 * (maxValue - minValue);

        setGraphicObjectProperty(pUicontrol, __GO_UI_SLIDERSTEP__, pdblStep, jni_double_vector, 2);
    }

    if ((Rhs < 2) || (propertiesValuesIndices[10] == NOT_FOUND))    /* Position property not set */
    {
        double* pdblPosition = NULL;

        getGraphicObjectProperty(pUicontrol, __GO_POSITION__, jni_double_vector, (void**) &pdblPosition);
        setGraphicObjectProperty(pUicontrol, __GO_POSITION__, pdblPosition, jni_double_vector, 4);
    }

    FREE(propertiesValuesIndices);

    /* Create return variable */
    if (createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, GraphicHandle))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
