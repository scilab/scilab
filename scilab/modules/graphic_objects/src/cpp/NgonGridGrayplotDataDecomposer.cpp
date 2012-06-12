/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ColorComputer.hxx"
#include "DecompositionUtils.hxx"
#include "NgonGridGrayplotDataDecomposer.hxx"

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

NgonGridGrayplotDataDecomposer* NgonGridGrayplotDataDecomposer::decomposer = NULL;

/*
 * To do: allowing either per-vertex or per-facet colors
 */
void NgonGridGrayplotDataDecomposer::fillColors(char* id, float* buffer, int bufferLength, int elementsSize)
{
    char* parentFigure = NULL;
    char* parent = NULL;

    double* z = NULL;
    double* colormap = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    int dataMapping = 0;
    int* piDataMapping = &dataMapping;

    NgonGridGrayplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &parent);
    getGraphicObjectProperty(id, __GO_PARENT__, jni_string, (void**) &parent);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (strcmp(parent, "") == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);

    if (parentFigure == NULL)
    {
        return;
    }

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    if (dataMapping == 0)
    {
        decomposer->fillNormalizedZGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);
    }
    else if (dataMapping == 1)
    {
        decomposer->fillDirectGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);
    }
}

int NgonGridGrayplotDataDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;
    double zShift = 0.0;
    double* pdZShift = &zShift;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int numberIndices = 0;

    NgonGridGrayplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* 0 indices if 0 points */
    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double, (void**) &pdZShift);

    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, &zShift, z, numX, numY);

    return numberIndices;
}

/*
 * To be merged with its parent's isFacetEdgeValid function.
 */
int NgonGridGrayplotDataDecomposer::isFacetEdgeValid(double* z, double* values, int numX, int numY, int i, int j, int logUsed)
{
    double zij = 0.;
    double zijp1 = 0.;

    int lowerZValid = 0;
    int upperZValid = 0;

    /* First, z-coordinate values are tested */
    zij = getZCoordinate(z, numX, numY, i, j, logUsed);
    zijp1 = getZCoordinate(z, numX, numY, i, j+1, logUsed);

    lowerZValid = DecompositionUtils::isValid(zij);
    upperZValid = DecompositionUtils::isValid(zijp1);

    if (logUsed)
    {
        lowerZValid &= DecompositionUtils::isLogValid(zij);
        upperZValid &= DecompositionUtils::isLogValid(zijp1);
    }

    /* Then the actual grid values */
    zij = getValue(values, numX, numY, i, j);
    zijp1 = getValue(values, numX, numY, i, j+1);

    lowerZValid &= DecompositionUtils::isValid(zij);
    upperZValid &= DecompositionUtils::isValid(zijp1);

    if (lowerZValid && upperZValid)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

