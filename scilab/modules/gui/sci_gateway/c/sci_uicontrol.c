/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

#include <stdio.h>
#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
#include "CreateUIControl.h"
#include "HandleManagement.h"
#include "sci_malloc.h"             /* MALLOC */
#include "localization.h"
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
#include "expandPathVariable.h"
#include "os_string.h"

static const char* propertiesNames[] =
{
    "style",
    "constraints",
    "margins",
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
    "tooltipstring",
    "layout",
    "layout_options",
    "border",
    "groupname",
    "title_position",
    "title_scroll",
    "scrollable",
    "icon"
};
/*--------------------------------------------------------------------------*/
static int style_property = -1;
static int parent_property = -1;
static int user_data_property = -1;
static int userdata_property = -1;
static int sliderstep_property = -1;
static int min_property = -1;
static int max_property = -1;
static int visible_property = -1;
static int position_property = -1;
static int scrollable_property = -1;
static int string_property = -1;
static int tooltipstring_property = -1;
/*--------------------------------------------------------------------------*/
void init_property_index();
/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int nbRow = 0, nbCol = 0, k = 0;
    int setStatus = SET_PROPERTY_SUCCEED;
    int PARENT_NOT_FOUND = -2;
    int NOT_FOUND = -1;
    int inputIndex = 0, beginIndex = 0;
    char *propertyName = NULL;
    char *styleProperty = NULL;

    int iPropertiesCount = sizeof(propertiesNames) / sizeof(char**);
    unsigned long GraphicHandle = 0;

    int found = 0;              /* Does the property exists ? */

    int *propertiesValuesIndices = NULL;

    int iParentType = -1;
    int *piParentType = &iParentType;
    int iParentStyle = -1;
    int *piParentStyle = &iParentStyle;

    int iParentUID      = 0;
    int iUicontrol      = 0;
    int iCurrentFigure  = 0;

    CheckOutputArgument(pvApiCtx, 0, 1);

    //init properties index
    init_property_index();

    if (nbInputArgument(pvApiCtx) == 0)
    {
        /* Create a pushbutton in current figure */

        /* Create a new pushbutton */
        GraphicHandle = getHandle(CreateUIControl(NULL));

        /* Set current figure as parent */
        iCurrentFigure = getCurrentFigure();
        if (iCurrentFigure == 0)
        {
            iCurrentFigure = createNewFigureWithAxes();
        }

        iUicontrol = getObjectFromHandle(GraphicHandle);
        setGraphicObjectRelationship(iCurrentFigure, iUicontrol);
    }
    else if (nbInputArgument(pvApiCtx) == 1)
    {
        /* Create a pushbutton in figure given as parameter */
        /* Or give focus to the uicontrol given as parameter */
        int* piAddr = NULL;
        int iType = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isHandleType(pvApiCtx, piAddr) == FALSE && isStringType(pvApiCtx, piAddr) == FALSE)
        {
            OverLoad(1);
            return FALSE;
        }
#if 0 // Allow XML loading
        else if (isStringType(pvApiCtx, piAddr))
        {
            char* pstXmlfile = NULL;
            char* pstExpandedPath = NULL;

            if (isScalar(pvApiCtx, piAddr) == 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
                return FALSE;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddr, &pstXmlfile))
            {
                freeAllocatedSingleString(pstXmlfile);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return FALSE;
            }

            pstExpandedPath = expandPathVariable(pstXmlfile);
            freeAllocatedSingleString(pstXmlfile);
            iUicontrol = xmlload(pstExpandedPath);
            if (iUicontrol < 1)
            {
                Scierror(999, _("%s: can not read file %s.\n"), fname, pstExpandedPath);
                FREE(pstExpandedPath);
                return 0;
            }
            FREE(pstExpandedPath);
            GraphicHandle = getHandle(iUicontrol);

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
#endif // Allow XML loading
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

            iParentUID = getObjectFromHandle((long)hParent);
            if (iParentUID != 0)
            {
                getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
                if (iParentType == __GO_UICONTROL__)  /* Focus management */
                {
                    GraphicHandle = (unsigned long)hParent;
                    requestFocus(iParentUID);
                }
                else if (iParentType == __GO_FIGURE__ || iParentType == __GO_UIMENU__)  /* PushButton creation */
                {
                    /* Create a new pushbutton */
                    GraphicHandle = getHandle(CreateUIControl(NULL));
                    iUicontrol = getObjectFromHandle(GraphicHandle);

                    /* First parameter is the parent */
                    setGraphicObjectRelationship(iParentUID, iUicontrol);
                    setStatus = callSetProperty(pvApiCtx, iUicontrol, &hParent, sci_handles, 1, 1, (char*)propertiesNames[parent_property]);
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
                        FREE(propertiesValuesIndices);
                        return 1;
                    }

                    if (isScalar(pvApiCtx, piAddr) == 0)
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
                        FREE(propertiesValuesIndices);
                        return FALSE;
                    }

                    if (getScalarDouble(pvApiCtx, piAddr, &dblValue))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                        FREE(propertiesValuesIndices);
                        return 1;
                    }

                    iParentUID = getFigureFromIndex((int)dblValue);
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    FREE(propertiesValuesIndices);
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
                    FREE(propertiesValuesIndices);
                    return 1;
                }

                if (isScalar(pvApiCtx, piAddr) == 0)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    FREE(propertiesValuesIndices);
                    return FALSE;
                }

                if (getScalarHandle(pvApiCtx, piAddr, &hParent))
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                    FREE(propertiesValuesIndices);
                    return 1;
                }

                iParentUID = getObjectFromHandle((long)hParent);
            }

            if (iParentUID == 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                         "Frame uicontrol");
                FREE(propertiesValuesIndices);
                return FALSE;
            }

            getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
            if (iParentType != __GO_FIGURE__)
            {
                getGraphicObjectProperty(iParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
                if (iParentType != __GO_UICONTROL__ ||
                        (iParentStyle != __GO_UI_FRAME__ && iParentStyle != __GO_UI_TAB__ && iParentStyle != __GO_UI_LAYER__))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"), fname, 1, "Figure",
                             "Frame uicontrol");
                    FREE(propertiesValuesIndices);
                    return FALSE;
                }
            }
            /* First parameter is the parent */
            propertiesValuesIndices[parent_property] = 1;
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
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, inputIndex);
                FREE(propertiesValuesIndices);
                return FALSE;
            }
            else
            {
                int* piAddr = NULL;
                sciErr = getVarAddressFromPosition(pvApiCtx, inputIndex, &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(propertiesValuesIndices);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddr, &propertyName))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, inputIndex);
                    FREE(propertiesValuesIndices);
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
                    freeAllocatedSingleString(propertyName);
                    FREE(propertiesValuesIndices);
                    return FALSE;
                }
                freeAllocatedSingleString(propertyName);
            }
        }

        if (propertiesValuesIndices[style_property] != NOT_FOUND)    /* Style found */
        {
            if ((checkInputArgumentType(pvApiCtx, propertiesValuesIndices[style_property], sci_strings)))
            {
                int* piAddr = NULL;
                sciErr = getVarAddressFromPosition(pvApiCtx, propertiesValuesIndices[style_property], &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(propertiesValuesIndices);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddr, &styleProperty))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, propertiesValuesIndices[style_property]);
                    FREE(propertiesValuesIndices);
                    return 1;
                }

                if (strcmp(styleProperty, "frame") == 0)
                {
                    //check scrollable property to create a scroll frame instead of normal frame
                    if (propertiesValuesIndices[scrollable_property] != NOT_FOUND)
                    {
                        char* pstScroll = NULL;
                        int iScroll = 0;
                        sciErr = getVarAddressFromPosition(pvApiCtx, propertiesValuesIndices[scrollable_property], &piAddr);
                        if (sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            FREE(propertiesValuesIndices);
                            freeAllocatedSingleString(styleProperty);
                            return 1;
                        }

                        if (isStringType(pvApiCtx, piAddr) == 0 && isBooleanType(pvApiCtx, piAddr) == 0 && isScalar(pvApiCtx, piAddr) == 0)
                        {
                            Scierror(202, _("%s: Wrong type for argument #%d: string or boolean expected.\n"), fname, propertiesValuesIndices[scrollable_property]);
                            FREE(propertiesValuesIndices);
                            freeAllocatedSingleString(styleProperty);
                            return 1;
                        }

                        if (isStringType(pvApiCtx, piAddr))
                        {
                            if (getAllocatedSingleString(pvApiCtx, piAddr, &pstScroll))
                            {
                                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, propertiesValuesIndices[scrollable_property]);
                                FREE(propertiesValuesIndices);
                                freeAllocatedSingleString(styleProperty);
                                return 1;
                            }

                            if (strcmp(pstScroll, "on") == 0)
                            {
                                iScroll = 1;
                            }

                            freeAllocatedSingleString(pstScroll);
                        }
                        else
                        {
                            if (getScalarBoolean(pvApiCtx, piAddr, &iScroll))
                            {
                                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, propertiesValuesIndices[scrollable_property]);
                                freeAllocatedSingleString(styleProperty);
                                FREE(propertiesValuesIndices);
                                return 1;
                            }
                        }

                        if (iScroll)
                        {
                            freeAllocatedSingleString(styleProperty);
                            styleProperty = os_strdup("framescrollable");
                        }

                        propertiesValuesIndices[scrollable_property] = NOT_FOUND;
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, propertiesValuesIndices[style_property]);
                FREE(propertiesValuesIndices);
                return FALSE;
            }
        }

        /* Create a new uicontrol */
        iUicontrol = CreateUIControl(styleProperty);
        freeAllocatedSingleString(styleProperty);
        if (iUicontrol == 0) /* Error in creation */
        {
            Scierror(999, _("%s: Could not create 'Uicontrol' handle.\n"), fname);
            FREE(propertiesValuesIndices);
            return FALSE;
        }
        GraphicHandle = getHandle(iUicontrol);

        /* If no parent given then the current figure is the parent */
        if (propertiesValuesIndices[parent_property] == NOT_FOUND)
        {
            /* Set the parent */
            iCurrentFigure = getCurrentFigure();

            if (iCurrentFigure == 0)
            {
                iCurrentFigure = createNewFigureWithAxes();
            }

            propertiesValuesIndices[parent_property] = PARENT_NOT_FOUND;
        }

        /* Read and set all properties */
        for (inputIndex = 1; inputIndex < iPropertiesCount; inputIndex++)   /* Style has already been set */
        {
            if (propertiesValuesIndices[inputIndex] == PARENT_NOT_FOUND)
            {
                //special case for not specified parent
                //but set relationship at the good moment.
                setGraphicObjectRelationship(iCurrentFigure, iUicontrol);
            }
            else if (propertiesValuesIndices[inputIndex] != NOT_FOUND)
            {
                int* piAddr = NULL;
                sciErr = getVarAddressFromPosition(pvApiCtx, propertiesValuesIndices[inputIndex], &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(propertiesValuesIndices);
                    return 1;
                }

                if (inputIndex == user_data_property || inputIndex == userdata_property)   /* User data settings */
                {
                    nbRow = -1;
                    nbCol = -1;
                    setStatus = callSetProperty(pvApiCtx, iUicontrol, piAddr, 0, 0, 0, (char*)propertiesNames[inputIndex]);
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
                                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                FREE(propertiesValuesIndices);
                                return 1;
                            }

                            setStatus = callSetProperty(pvApiCtx, iUicontrol, pdblValue, sci_matrix, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                            break;
                        }
                        case sci_strings:
                            /* Index for String & TooltipString properties: Can be more than one character string */
                            if ((inputIndex == string_property) || (inputIndex == tooltipstring_property))
                            {
                                char** pstValue = NULL;
                                if (getAllocatedMatrixOfString(pvApiCtx, piAddr, &nbRow, &nbCol, &pstValue))
                                {
                                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                    FREE(propertiesValuesIndices);
                                    return 1;
                                }

                                setStatus = callSetProperty(pvApiCtx, iUicontrol, pstValue, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                                freeAllocatedMatrixOfString(nbRow, nbCol, pstValue);
                            }
                            else
                            {
                                char* pstValue = NULL;
                                if (getAllocatedSingleString(pvApiCtx, piAddr, &pstValue))
                                {
                                    Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, propertiesValuesIndices[inputIndex]);
                                    FREE(propertiesValuesIndices);
                                    return 1;
                                }

                                nbRow = (int)strlen(pstValue);
                                nbCol = 1;
                                setStatus = callSetProperty(pvApiCtx, iUicontrol, pstValue, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
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
                                FREE(propertiesValuesIndices);
                                return 1;
                            }

                            setStatus = callSetProperty(pvApiCtx, iUicontrol, pHandles, sci_handles, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                            break;
                        }
                        case sci_tlist: //constraints and border
                        {
                            setStatus = callSetProperty(pvApiCtx, iUicontrol, piAddr, sci_tlist, 1, 1, (char*)propertiesNames[inputIndex]);
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
                    FREE(propertiesValuesIndices);
                    return FALSE;
                }
            }
        }
    }

    if (propertiesValuesIndices != NULL
            && (propertiesValuesIndices[sliderstep_property] == NOT_FOUND &&
                (propertiesValuesIndices[min_property] != NOT_FOUND || propertiesValuesIndices[max_property] != NOT_FOUND)))    /* SliderStep property not set */
    {
        /* Set SliderStep property to [1/100*(Max-Min) 1/10*(Max-Min)] */
        double maxValue = 0;
        double* pdblMaxValue = &maxValue;
        double minValue = 0;
        double* pdblMinValue = &minValue;
        double pdblStep[2];

        getGraphicObjectProperty(iUicontrol, __GO_UI_MIN__, jni_double, (void**) &pdblMinValue);
        getGraphicObjectProperty(iUicontrol, __GO_UI_MAX__, jni_double, (void**) &pdblMaxValue);

        pdblStep[0] = 0.01 * (maxValue - minValue);
        pdblStep[1] = 0.1 * (maxValue - minValue);

        setGraphicObjectProperty(iUicontrol, __GO_UI_SLIDERSTEP__, pdblStep, jni_double_vector, 2);
    }

    if ((nbInputArgument(pvApiCtx) < 2) || (propertiesValuesIndices[position_property] == NOT_FOUND))    /* Position property not set */
    {
        double* pdblPosition = NULL;

        getGraphicObjectProperty(iUicontrol, __GO_POSITION__, jni_double_vector, (void**) &pdblPosition);
        setGraphicObjectProperty(iUicontrol, __GO_POSITION__, pdblPosition, jni_double_vector, 4);
        releaseGraphicObjectProperty(__GO_POSITION__, pdblPosition, jni_double_vector, 4);
    }

    if ((nbInputArgument(pvApiCtx) < 2) || (propertiesValuesIndices[visible_property] == NOT_FOUND))    /* Visible property not set */
    {
        /* Force the uicontrol to be visible because is invisible by default in the model (See bug #10346) */
        int b = (int)TRUE;
        setGraphicObjectProperty(iUicontrol, __GO_VISIBLE__, &b, jni_bool, 1);
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
void init_property_index()
{
    if (style_property == -1)
    {
        int iPropertiesCount = sizeof(propertiesNames) / sizeof(char**);
        int i = 0;
        for (i = 0 ; i < iPropertiesCount ; i++)
        {
            if (style_property == -1 && strcmp(propertiesNames[i], "style") == 0)
            {
                style_property = i;
                continue;
            }

            if (parent_property == -1 && strcmp(propertiesNames[i], "parent") == 0)
            {
                parent_property = i;
                continue;
            }

            if (user_data_property == -1 && strcmp(propertiesNames[i], "user_data") == 0)
            {
                user_data_property = i;
                continue;
            }

            if (userdata_property == -1 && strcmp(propertiesNames[i], "userdata") == 0)
            {
                userdata_property = i;
                continue;
            }

            if (sliderstep_property == -1 && strcmp(propertiesNames[i], "sliderstep") == 0)
            {
                sliderstep_property = i;
                continue;
            }

            if (min_property == -1 && strcmp(propertiesNames[i], "min") == 0)
            {
                min_property = i;
                continue;
            }

            if (max_property == -1 && strcmp(propertiesNames[i], "max") == 0)
            {
                max_property = i;
                continue;
            }

            if (visible_property == -1 && strcmp(propertiesNames[i], "visible") == 0)
            {
                visible_property = i;
                continue;
            }

            if (position_property == -1 && strcmp(propertiesNames[i], "position") == 0)
            {
                position_property = i;
                continue;
            }

            if (scrollable_property == -1 && strcmp(propertiesNames[i], "scrollable") == 0)
            {
                scrollable_property = i;
                continue;
            }

            if (tooltipstring_property == -1 && strcmp(propertiesNames[i], "tooltipstring") == 0)
            {
                tooltipstring_property = i;
                continue;
            }

            if (string_property == -1 && strcmp(propertiesNames[i], "string") == 0)
            {
                string_property = i;
                continue;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
