/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_zoom_rect.c                                                  */
/* desc : interface for zoom_rect routine                                 */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "getPropertyAssignedValue.h"
#include "axesScale.h"
#include "localization.h"
#include "Scierror.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "BuildObjects.h"

#include "JavaInteraction.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
static char * getZoomedObject(const char * fname);
static BOOL getZoomRect(const char * fname, int attribPos, double rect[4]);
/*--------------------------------------------------------------------------*/
/**
 * Get the [xmin, ymin, xmax, ymax] vector specified as input argument
 * @param fname name of the calling function for error messages
 * @param attribPos position of the argument within the rhs (1 or 2)
 * @param[out] rect retrieved rectangle
 * @return TRUE if the rect could be retrieved, false otherwise
 */
static BOOL getZoomRect(const char * fname, int attribPos, double rect[4])
{
    int nbRow = 0;
    int nbCol = 0;
    size_t stackPointer = 0;
    int i = 0;
    double * rectVect = NULL;
    GetRhsVar(attribPos, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stackPointer);

    if (nbRow * nbCol != 4)
    {
        if (attribPos == 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d expected.\n"), fname, 1, 4);
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d expected.\n"), fname, 2, 4);
        }
        return FALSE;
    }

    rectVect = stk(stackPointer);
    for (i = 0; i < 4; i++)
    {
        rect[i] = rectVect[i];
    }

    return TRUE;

}
/*--------------------------------------------------------------------------*/
/**
 * Return the handle passed as input argument if one exists
 * @param fname of the function for errors
 * @return NULL if the input argument is not correct,
 *              the object to zoom otherwise
 */
static char * getZoomedObject(const char * fname)
{
    int nbRow  = 0;
    int nbCol = 0;
    size_t stackPointer = 0;
    char *res = NULL;
    int iType = -1;
    int *piType = &iType;
    /* if a handle is specified it must be the first input argument */
    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer);

    /* check that there is only a single Figre or subwin */
    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Single handle expected.\n"), fname, 1);
        return NULL;
    }

    res = (char*)getObjectFromHandle(getHandleFromStack(stackPointer));

    if (res == NULL)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Figure or Axes handle expected.\n"), fname, 1);
        return NULL;
    }

    getGraphicObjectProperty(res, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_FIGURE__ && iType != __GO_AXES__)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Figure or Axes handle expected.\n"), fname, 1);
        return NULL;
    }

    return res;


}
/*--------------------------------------------------------------------------*/
int sci_zoom_rect(char *fname, unsigned long fname_len)
{
    char* pFigureUID = NULL;
    char** childrenUID = NULL;
    int iChildrenCount = 0;
    int* childrencount = &iChildrenCount;
    int iHidden = 0;
    int *piHidden = &iHidden;
    int i = 0;

    CheckRhs(0, 2) ;
    CheckLhs(0, 1) ;
    if (Rhs == 0)
    {
        /* zoom_rect() */
        pFigureUID = (char*)getCurrentFigure();
        if (pFigureUID == NULL)
        {
            pFigureUID = createNewFigureWithAxes();
        }
        startInteractiveZoom(pFigureUID);
    }
    else if (Rhs == 1)
    {
        /* zoom_rect([xmin,ymin,xmax,ymax]) or zoom_rect(handle) */
        /* with handle a figure or subwindow */
        if (GetType(1) == sci_handles)
        {
            char * pstZoomedObject = getZoomedObject(fname);
            if (pstZoomedObject == NULL)
            {
                return -1;
            }
            startInteractiveZoom(pstZoomedObject);
        }
        else if (GetType(1) == sci_matrix)
        {
            double rect[4];
            if (getZoomRect(fname, 1, rect))
            {
                /* rectangle found */
                //int status = sciZoom2D(getCurrentSubWin(), rect);
                int status = 0;
                pFigureUID = (char*)getCurrentFigure();

                getGraphicObjectProperty(pFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&childrencount);

                getGraphicObjectProperty(pFigureUID, __GO_CHILDREN__, jni_string_vector, (void **)&childrenUID);

                for (i = 0; i < childrencount[0]; ++i)
                {
                    getGraphicObjectProperty(childrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
                    if (iHidden == 0)
                    {
                        status = sciZoom2D(childrenUID[i], rect);
                    }
                }
                if (status == SET_PROPERTY_ERROR)
                {
                    /* error on rectangle bounds */
                    Scierror(999, _("%s: Wrong value for input argument #%d: Specified bounds are not correct.\n"), fname, 1);
                    return -1;
                }
            }
            else
            {
                /* error on rectagle definition */
                return -1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Handle or vector of double expected.\n"), fname, 1);
            return 0;
        }
    }
    else if (Rhs == 2)
    {
        /* zoom_rect(handle, [xmin,ymin,xmax,ymax]) */

        double rect[4];
        char *zoomedObject = NULL;

        if (GetType(1) != sci_handles || GetType(2) != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for input arguments: Handle or vector of double expected.\n"), fname);
            return -1;
        }

        zoomedObject = getZoomedObject(fname);
        if (zoomedObject == NULL || !getZoomRect(fname, 2, rect))
        {
            return -1;
        }

        if (sciZoomRect(zoomedObject, rect) == SET_PROPERTY_ERROR)
        {
            /* error on rectangle bounds */
            Scierror(999, _("%s: Error on input argument #%d: Specified bounds are not correct.\n"), fname, 1);
            return -1;
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/

