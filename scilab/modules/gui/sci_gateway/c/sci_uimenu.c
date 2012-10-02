/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 * desc : interface for sci_uimenu routine
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "BuildObjects.h"
#include "gw_gui.h"
#include "localization.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "localization.h"
#include "Scierror.h"
#include "stricmp.h"
#include "CreateUimenu.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "api_scilab.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int sci_uimenu(char *fname, unsigned long fname_len)
{
    int nbRow = 0, nbCol = 0, stkAdr = 0;

    int setStatus = SET_PROPERTY_SUCCEED;

    int inputIndex = 0, beginIndex = 0;

    char *propertyName = NULL;

    char *pParentUID = NULL;

    unsigned long GraphicHandle = 0;

    int parentDefined = FALSE;

    char *pstCurrentFigure = NULL;

    int iParentType = -1;
    int *piParentType = &iParentType;

    /* Create a new menu */
    GraphicHandle = getHandle(CreateUimenu());

    /* If no Rhs -> current figure is the parent (Ascendant compatibility) */
    if (Rhs == 0)
    {
        // Set the parent property
        pstCurrentFigure = (char*)getCurrentFigure();
        if (pstCurrentFigure == NULL)
        {
            pstCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(pstCurrentFigure, getObjectFromHandle(GraphicHandle));
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
            Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
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
                if (iParentType == __GO_FIGURE__ && iParentType == __GO_UIMENU__)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Figure", "Uimenu");
                    return FALSE;
                }

                // Set the parent property
                callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), stkAdr, sci_handles, nbRow, nbCol, "parent");

                // Set the flag to avoid setting the parent two times
                parentDefined = TRUE;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Figure", "Uimenu");
                return FALSE;
            }

            // First input parameter which is a property name
            beginIndex = 2;
        }
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

    /* Read and set all properties */
    for (inputIndex = beginIndex; inputIndex < Rhs; inputIndex = inputIndex + 2)
    {
        int isUserDataProperty = 0;
        int iPropertyValuePositionIndex = inputIndex + 1;
        size_t posStackOrAdr = 0;

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

            if (stricmp(propertyName, "parent") == 0)
            {
                parentDefined = TRUE;
            }

            isUserDataProperty = (stricmp(propertyName, "user_data") == 0) || (stricmp(propertyName, "userdata") == 0);
        }

        /* Read property value */
        switch (VarType(iPropertyValuePositionIndex))
        {
            case sci_matrix:
                if (isUserDataProperty)
                {
                    nbRow = -1;
                    nbCol = -1;
                    posStackOrAdr = iPropertyValuePositionIndex;
                }
                else
                {
                    GetRhsVar(iPropertyValuePositionIndex, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
                    posStackOrAdr = stkAdr;
                }
                setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), posStackOrAdr, sci_matrix, nbRow, nbCol, propertyName);
                break;
            case sci_strings:
                if (isUserDataProperty)
                {
                    nbRow = -1;
                    nbCol = -1;
                    posStackOrAdr = iPropertyValuePositionIndex;
                }
                else
                {
                    GetRhsVar(iPropertyValuePositionIndex, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
                    posStackOrAdr = stkAdr;
                }
                setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), posStackOrAdr, sci_strings, nbRow, nbCol, propertyName);
                break;
            case sci_handles:
                if (isUserDataProperty)
                {
                    nbRow = -1;
                    nbCol = -1;
                    posStackOrAdr = iPropertyValuePositionIndex;
                }
                else
                {
                    GetRhsVar(iPropertyValuePositionIndex, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
                    posStackOrAdr = stkAdr;
                }
                setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), posStackOrAdr, sci_handles, nbRow, nbCol, propertyName);
                break;
            case sci_list:
                if (isUserDataProperty)
                {
                    nbRow = -1;
                    nbCol = -1;
                    posStackOrAdr = iPropertyValuePositionIndex;
                }
                else
                {
                    GetRhsVar(iPropertyValuePositionIndex, LIST_DATATYPE, &nbRow, &nbCol, &stkAdr);
                    posStackOrAdr = iPropertyValuePositionIndex;
                }
                setStatus = callSetProperty(pvApiCtx, (char*)getObjectFromHandle(GraphicHandle), posStackOrAdr, sci_list, nbRow, nbCol, propertyName);
                break;
            default:
                setStatus = SET_PROPERTY_ERROR;
                break;
        }
        if (setStatus == SET_PROPERTY_ERROR)
        {
            Scierror(999, _("%s: Could not set property '%s'.\n"), fname, propertyName);
            return FALSE;
        }
    }

    /* If the parent is not given, the current figure is set as parent */
    if (!parentDefined && (Rhs != 0))
    {
        // Set the parent property
        pstCurrentFigure = (char*)getCurrentFigure();
        if (pstCurrentFigure == NULL)
        {
            pstCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(pstCurrentFigure, getObjectFromHandle(GraphicHandle));
    }

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
