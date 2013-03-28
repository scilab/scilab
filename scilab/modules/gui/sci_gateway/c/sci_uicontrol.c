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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
/*--------------------------------------------------------------------------*/
#define NBPROPERTIES 28
#define MAXPROPERTYNAMELENGTH 20
/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, unsigned long fname_len)
{
    int nbRow = 0, nbCol = 0, stkAdr = 0, k = 0;
    char **stkAdrForStrings = NULL;

    int setStatus = SET_PROPERTY_SUCCEED;

    int NOT_FOUND = -1;

    int inputIndex = 0, beginIndex = 0;

    char *propertyName = NULL;
    char *styleProperty = NULL;

    char *pParentUID = NULL;
    char *pUicontrol = NULL;

    unsigned long GraphicHandle = 0;

    int found = 0;              /* Does the property exists ? */

    /* @TODO remove this crappy initialization */
    /* DO NOT CHANGE ORDER !! */
    char propertiesNames[NBPROPERTIES][MAXPROPERTYNAMELENGTH] =
    {
        "style", "parent", "backgroundcolor", "foregroundcolor", "string", "units", "fontweight", "min", "max", "tag", "position", "relief",
        "horizontalalignment", "verticalalignment", "sliderstep", "fontname", "callback", "fontangle", "fontunits", "fontsize", "listboxtop", "user_data", "value", "userdata", "visible", "enable",
        "callback_type", "tooltipstring"
    };
    int *propertiesValuesIndices = NULL;
    int lw = 0;
    char *propertyPart = NULL;

    int iParentType = -1;
    int *piParentType = &iParentType;
    int iParentStyle = -1;
    int *piParentStyle = &iParentStyle;
    char const* pstCurrentFigure = NULL;

    CheckLhs(0, 1);

    if (Rhs == 0)
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
    else if (Rhs == 1)
    {
        /* Create a pushbutton in figure given as parameter */
        /* Or give focus to the uicontrol given as parameter */

        if (VarType(1) != sci_handles)
        {
            lw = 1 + Top - Rhs;
            C2F(overload) (&lw, "uicontrol", 9);
            return FALSE;
        }
        else                    /* Get parent ID */
        {
            GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                return FALSE;
            }
            pParentUID = (char*)getObjectFromHandle((long) * hstk(stkAdr));
            if (pParentUID != NULL)
            {
                getGraphicObjectProperty(pParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
                if (iParentType == __GO_UICONTROL__)  /* Focus management */
                {
                    GraphicHandle = (long) * hstk(stkAdr);
                    requestFocus(pParentUID);
                }
                else if (iParentType == __GO_FIGURE__ || iParentType == __GO_UIMENU__)  /* PushButton creation */
                {
                    /* Create a new pushbutton */
                    GraphicHandle = getHandle(CreateUIControl(NULL));

                    /* First parameter is the parent */
                    setGraphicObjectRelationship(pParentUID, getObjectFromHandle(GraphicHandle));
                    setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), stkAdr, sci_handles, nbRow, nbCol, (char *)propertiesNames[1]);
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
        if ((VarType(1) != sci_handles) && (VarType(1) != sci_strings))
        {
            lw = 1 + Top - Rhs;
            C2F(overload) (&lw, "uicontrol", 9);
            return FALSE;
        }

        /* Allocate memory to store the position of properties in uicontrol call */
        if ((propertiesValuesIndices = (int *)MALLOC(sizeof(int) * NBPROPERTIES)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return FALSE;
        }

        /* Init all positions to NOT_FOUND */
        for (inputIndex = 0; inputIndex < NBPROPERTIES; inputIndex++)
        {
            propertiesValuesIndices[inputIndex] = NOT_FOUND;    /* Property initialized as not found */
        }

        /**
         * Odd number of input arguments
         * First input is the parent ID
         * All event inputs are property names
         * All odd (except first) inputs are property values
         */
        if (Rhs % 2 == 1)
        {
            if (VarType(1) != sci_handles)
            {
                if (VarType(1) == sci_matrix)
                {
                    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
                    if (nbRow * nbCol == 1)
                    {
                        pParentUID = (char*)getFigureFromIndex((int)(*stk(stkAdr)));

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
                        Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                        return FALSE;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    return FALSE;
                }
            }
            else                /* Get parent ID */
            {
                GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

                if (nbRow * nbCol != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    return FALSE;
                }
                pParentUID = (char*)getObjectFromHandle((long) * hstk(stkAdr));
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
            if (VarType(inputIndex) != sci_strings)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, inputIndex);
                return FALSE;
            }
            else
            {
                GetRhsVar(inputIndex, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
                propertyName = cstk(stkAdr);

                /* Bug 3031 */
                /* We only want to compare propertyName along its length */
                /* 'posi' must be matched to 'position' */
                propertyPart = (char *)MALLOC(sizeof(char) * (MAXPROPERTYNAMELENGTH + 1));
                if (propertyPart == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return FALSE;
                }

                found = 0;
                for (k = 0; k < NBPROPERTIES; k++)
                {
                    if (strlen(propertyName) <= MAXPROPERTYNAMELENGTH)
                    {
                        strncpy(propertyPart, propertiesNames[k], strlen(propertyName));
                        propertyPart[strlen(propertyName)] = '\0';

                        if (stricmp(propertyName, propertyPart) == 0)
                        {
                            propertiesValuesIndices[k] = inputIndex + 1;    /* Position of value for property */
                            found = 1;
                            break;
                        }
                    }
                }

                FREE(propertyPart);

                if (found == 0)
                {
                    Scierror(999, _("%s: Unknown property: %s for '%s' handles.\n"), fname, propertyName, "Uicontrol");
                    return FALSE;
                }
            }
        }

        if (propertiesValuesIndices[0] != NOT_FOUND)    /* Style found */
        {
            if (VarType(propertiesValuesIndices[0]) == sci_strings)
            {
                GetRhsVar(propertiesValuesIndices[0], STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
                styleProperty = cstk(stkAdr);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, propertiesValuesIndices[0]);
                return FALSE;
            }
        }

        /* Create a new uicontrol */
        pUicontrol = CreateUIControl(styleProperty);
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
        for (inputIndex = 1; inputIndex < NBPROPERTIES; inputIndex++)   /* Style has already been set */
        {
            if (propertiesValuesIndices[inputIndex] != NOT_FOUND)
            {
                if (inputIndex == 21 || inputIndex == 23)   /* User data settings */
                {
                    stkAdr = propertiesValuesIndices[inputIndex];   /* Special management */
                    nbRow = -1;
                    nbCol = -1;
                    setStatus =
                        callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), stkAdr, VarType(propertiesValuesIndices[inputIndex]), nbRow, nbCol,
                                        (char *)propertiesNames[inputIndex]);
                }
                else            /* All other properties */
                {
                    /* Read property value */
                    switch (VarType(propertiesValuesIndices[inputIndex]))
                    {
                        case sci_matrix:
                            GetRhsVar(propertiesValuesIndices[inputIndex], MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
                            setStatus =
                                callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), stkAdr, sci_matrix, nbRow, nbCol,
                                                (char *)propertiesNames[inputIndex]);
                            break;
                        case sci_strings:
                            /* Index for String & TooltipString properties: Can be more than one character string */
                            if ((inputIndex == 4) || (inputIndex == 27))
                            {
                                GetRhsVar(propertiesValuesIndices[inputIndex], MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, &stkAdrForStrings);
                                setStatus =
                                    callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), (size_t) stkAdrForStrings, sci_strings, nbRow, nbCol,
                                                    (char *)propertiesNames[inputIndex]);
                                freeArrayOfString(stkAdrForStrings, nbRow * nbCol);
                            }
                            else
                            {
                                GetRhsVar(propertiesValuesIndices[inputIndex], STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
                                setStatus =
                                    callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), stkAdr, sci_strings, nbRow, nbCol,
                                                    (char *)propertiesNames[inputIndex]);
                            }
                            break;
                        case sci_handles:
                            GetRhsVar(propertiesValuesIndices[inputIndex], GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
                            setStatus =
                                callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), stkAdr, sci_handles, nbRow, nbCol,
                                                (char *)propertiesNames[inputIndex]);
                            break;
                        default:
                            setStatus = SET_PROPERTY_ERROR;
                            break;
                    }
                }
                if (setStatus == SET_PROPERTY_ERROR)
                {
                    Scierror(999, _("%s: Could not set property '%s'.\n"), fname, (char *)propertiesNames[inputIndex]);
                    return FALSE;
                }
            }
        }
    }

    if ((Rhs < 2) || (propertiesValuesIndices[24] == NOT_FOUND))    /* Visible property not set */
    {
        /* Force the uicontrol to be visible because is invisible by default in the model (See bug #10346) */
        int b = (int)TRUE;
        setGraphicObjectProperty(pUicontrol, __GO_VISIBLE__, &b, jni_bool, 1);
    }

    if ((Rhs < 2) || (propertiesValuesIndices[14] == NOT_FOUND))    /* SliderStep property not set */
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
    nbRow = 1;
    nbCol = 1;
    CreateVar(Rhs + 1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
    *hstk(stkAdr) = GraphicHandle;

    LhsVar(1) = Rhs + 1;

    PutLhsVar();

    return TRUE;
}

/*--------------------------------------------------------------------------*/
