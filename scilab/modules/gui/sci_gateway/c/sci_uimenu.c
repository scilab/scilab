/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan Cornet
* Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2006 - INRIA - Fabrice Leray
* Copyright (C) 2011 - DIGITEO - Allan CORNET
* desc : interface for sci_uimenu routine
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

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "BuildObjects.h"
#include "gw_gui.h"
#include "localization.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "localization.h"
#include "Scierror.h"
#include "os_string.h"
#include "CreateUimenu.h"
#include "createGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "api_scilab.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int sci_uimenu(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int nbRow = 0, nbCol = 0;
    int setStatus = SET_PROPERTY_SUCCEED;
    int inputIndex = 0, beginIndex = 0;
    char *propertyName = NULL;
    int iParentUID = 0;
    unsigned long GraphicHandle = 0;
    int parentDefined = FALSE;
    int iCurrentFigure = 0;
    int iParentType = -1;
    int *piParentType = &iParentType;

    /* Create a new menu */
    GraphicHandle = getHandle(CreateUimenu());

    /* If no nbInputArgument(pvApiCtx) -> current figure is the parent (Ascendant compatibility) */
    if (nbInputArgument(pvApiCtx) == 0)
    {
        // Set the parent property
        iCurrentFigure = getCurrentFigure();
        if (iCurrentFigure == 0)
        {
            iCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(iCurrentFigure, getObjectFromHandle(GraphicHandle));
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
            Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }
        else                    /* Get parent ID */
        {
            int *piAddr = NULL;
            long long hParent = 0;
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of handle at position 1.
            // YOU MUST REMOVE YOUR VARIABLE DECLARATION "int stkAdr".
            if (getScalarHandle(pvApiCtx, piAddr, &hParent))
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle expected.\n"), fname, 1);
                return 1;
            }

            iParentUID = getObjectFromHandle((long)hParent);
            if (iParentUID != 0)
            {
                getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
                if (iParentType != __GO_FIGURE__ && iParentType != __GO_UIMENU__)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Figure", "Uimenu");
                    return FALSE;
                }

                // Set the parent property
                callSetProperty(pvApiCtx, getObjectFromHandle(GraphicHandle), &hParent, sci_handles, 1, 1, "parent");

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
    for (inputIndex = beginIndex; inputIndex < nbInputArgument(pvApiCtx); inputIndex = inputIndex + 2)
    {
        int* piAddrValue = NULL;
        int* piAddrProperty = NULL;

        int isUserDataProperty = 0;
        int iPropertyValuePositionIndex = inputIndex + 1;
        size_t posStackOrAdr = 0;

        /* Read property name */
        if ((!checkInputArgumentType(pvApiCtx, inputIndex, sci_strings)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, inputIndex);
            return FALSE;
        }
        else
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, inputIndex, &piAddrProperty);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddrProperty, &propertyName))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, inputIndex);
                return 1;
            }

            if (stricmp(propertyName, "parent") == 0)
            {
                parentDefined = TRUE;
            }

            isUserDataProperty = (stricmp(propertyName, "user_data") == 0) || (stricmp(propertyName, "userdata") == 0);
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, iPropertyValuePositionIndex, &piAddrValue);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(propertyName);
            return 1;
        }

        if (isUserDataProperty)
        {
            nbRow = -1;
            nbCol = -1;
            setStatus = callSetProperty(pvApiCtx, getObjectFromHandle(GraphicHandle), piAddrValue, 0, 0, 0, propertyName);
        }
        else
        {
            /* Read property value */
            switch (getInputArgumentType(pvApiCtx, iPropertyValuePositionIndex))
            {
                case sci_matrix:
                {
                    double* pdblValue = NULL;
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddrValue, &nbRow, &nbCol, &pdblValue);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, iPropertyValuePositionIndex);
                        freeAllocatedSingleString(propertyName);
                        return 1;
                    }

                    setStatus = callSetProperty(pvApiCtx, getObjectFromHandle(GraphicHandle), pdblValue, sci_matrix, nbRow, nbCol, propertyName);
                    break;
                }
                case sci_strings:
                {
                    char* pstValue = NULL;
                    if (getAllocatedSingleString(pvApiCtx, piAddrValue, &pstValue))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, iPropertyValuePositionIndex);
                        freeAllocatedSingleString(propertyName);
                        return 1;
                    }

                    nbRow = (int)strlen(pstValue);
                    nbCol = 1;
                    setStatus = callSetProperty(pvApiCtx, getObjectFromHandle(GraphicHandle), pstValue, sci_strings, nbRow, nbCol, propertyName);
                    freeAllocatedSingleString(pstValue);
                    break;
                }
                case sci_handles:
                {
                    long long* phValues = NULL;
                    sciErr = getMatrixOfHandle(pvApiCtx, piAddrValue, &nbRow, &nbCol, &phValues);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, iPropertyValuePositionIndex);
                        freeAllocatedSingleString(propertyName);
                        return 1;
                    }
                    setStatus = callSetProperty(pvApiCtx, getObjectFromHandle(GraphicHandle), phValues, sci_handles, nbRow, nbCol, propertyName);
                    break;
                }
                case sci_list:
                {
                    getListItemNumber(pvApiCtx, piAddrValue, &nbRow);
                    nbCol = 1;
                    setStatus = callSetProperty(pvApiCtx, getObjectFromHandle(GraphicHandle), piAddrValue, sci_list, nbRow, nbCol, propertyName);
                    break;
                }
                default:
                {
                    setStatus = SET_PROPERTY_ERROR;
                    break;
                }
            }
        }

        if (setStatus == SET_PROPERTY_ERROR)
        {
            Scierror(999, _("%s: Could not set property '%s'.\n"), fname, propertyName);
            freeAllocatedSingleString(propertyName);
            return FALSE;
        }

        freeAllocatedSingleString(propertyName);
    }

    /* If the parent is not given, the current figure is set as parent */
    if (!parentDefined && (nbInputArgument(pvApiCtx) != 0))
    {
        // Set the parent property
        iCurrentFigure = getCurrentFigure();
        if (iCurrentFigure == 0)
        {
            iCurrentFigure = createNewFigureWithAxes();
        }
        setGraphicObjectRelationship(iCurrentFigure, getObjectFromHandle(GraphicHandle));
    }

    /* Create return variable */
    nbRow = 1;
    nbCol = 1;
    // YOU MUST REMOVE YOUR VARIABLE DECLARATION "int stkAdr".
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
