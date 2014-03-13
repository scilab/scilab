/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "addColor.h"
#include "string.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "MALLOC.h"

int addColor(int _iFig, double* _pdblNewColor)
{
    int iColorIndex = 0;
    int i = 0, j = 0;
    int iColorMapSize = 0;
    int* piColorMapSize = &iColorMapSize;
    double* pdblColorMap = NULL;
    //for new figure, we have to set figure and axes background to [0.8 0.8 0.8]
    //to do that, we have to update figure.colormap to add new color if not exist.
    //or get index of color in current color_map

    //first get figure.color_map
    getGraphicObjectProperty(_iFig, __GO_COLORMAP_SIZE__, jni_int, (void**)&piColorMapSize);
    getGraphicObjectProperty(_iFig, __GO_COLORMAP__, jni_double_vector, (void **)&pdblColorMap);

    //check if newColor already in coloMap
    for (i = 0 ; i < iColorMapSize ; i++)
    {
        BOOL bFound = TRUE;
        double* pdblCurrentColor = pdblColorMap + i;
        for (j = 0 ; j < COLOR_COMPONENT ; j++)
        {
            if (*(pdblCurrentColor + j * iColorMapSize) != _pdblNewColor[j])
            {
                bFound = FALSE;
                break;
            }
        }

        if (bFound)
        {
            iColorIndex = i + 1;
            break;
        }
    }

    //not found in current color map
    if (iColorIndex == 0)
    {
        int iNewColorMapSize = (iColorMapSize + 1) * COLOR_COMPONENT;
        double* pdblNewColorMap = (double*)MALLOC(sizeof(double) * iNewColorMapSize);
        //we have to add the new color at the end of the current color map
        for (i = 0 ; i < COLOR_COMPONENT ; i++)
        {
            memcpy(pdblNewColorMap + i * (iColorMapSize + 1),
                   pdblColorMap + (i * iColorMapSize),
                   iColorMapSize * sizeof(double));

            pdblNewColorMap[i * (iColorMapSize + 1) + iColorMapSize] = _pdblNewColor[i];
        }

        setGraphicObjectProperty(_iFig, __GO_COLORMAP__, pdblNewColorMap, jni_double_vector, iNewColorMapSize);

        iColorIndex = iColorMapSize + 1;
    }
    return iColorIndex;
}
