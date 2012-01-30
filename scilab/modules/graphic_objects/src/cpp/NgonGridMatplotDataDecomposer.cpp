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
#include "NgonGridDataDecomposer.hxx"
#include "NgonGridMatplotDataDecomposer.hxx"

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

NgonGridMatplotDataDecomposer* NgonGridMatplotDataDecomposer::decomposer = NULL;

/*
 * To do:
 * -clean-up: replace explicitely computed z indices by getPointIndex calls
 * -remove the per-vertex color fill code
 */
void NgonGridMatplotDataDecomposer::fillColors(char* id, float* buffer, int bufferLength, int elementsSize)
{
    char* parent;
    char* parentFigure;

    double* z;
    double* colormap;
    double currentZ;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    int i;
    int j;
    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_PARENT__, jni_string, (void**) &parent);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (strcmp(parent, "") == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    /* In order not to access invalid data when reading the last line's last element */
    if (numX < 2)
    {
        return;
    }

#if PER_VERTEX_VALUES
    for (j = 0; j < numY-1; j++)
    {
        for (i = 0; i < numX-1; i++)
        {
            currentZ = z[i*(numY-1)+(numY-2-j)];

            ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset+3] = 1.0;
            }

            bufferOffset += elementsSize;
        }

        /* Last element (same as the above loop's last) */
        currentZ  = z[(numX-2)*(numY-1)+(numY-2-j)];

        ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

        if (elementsSize == 4)
        {
            buffer[bufferOffset+3] = 1.0;
        }

        bufferOffset += elementsSize;
    }

    /* Last line */
    for (i = 0; i < numX-1; i++)
    {
        currentZ = z[(numY-1)*i+0];

        ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

        bufferOffset += elementsSize;

        if (elementsSize == 4)
        {
            buffer[bufferOffset+3] = 1.0;
        }
    }

    /* Last element (same as the above loop's last) */
    currentZ  = z[(numX-2)*(numY-1)+0];

    ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

    if (elementsSize == 4)
    {
        buffer[bufferOffset+3] = 1.0;
    }

#else

    for (j = 0; j < numY-1; j++)
    {
        for (i = 0; i < numX-1; i++)
        {
            float facetColor[3];

            currentZ = z[i*(numY-1)+(numY-2-j)];

            ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, facetColor);

            writeFacetColorToBuffer(buffer, bufferOffset, facetColor, elementsSize);

            bufferOffset += 4*elementsSize;
        }
    }

#endif
}

/*
 * To do: merge with NgonGridDataDecomposer::fillIndices as these functions perform a lot of work in
 * a redundant way.
 */
int NgonGridMatplotDataDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x;
    double* y;
    double* z;
    double zShift = 0.0;
    double* pdZShift = &zShift;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int numberIndices;

    NgonGridMatplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* 0 indices if less than 2 points along either dimension */
    if (numX < 2 || numY < 2)
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

int NgonGridMatplotDataDecomposer::isFacetValid(double* z, double* values, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid)
{
    double zij;
    int facetValid;

    zij = getZCoordinate(z, numX, numY, i, j, logUsed);

    facetValid = DecompositionUtils::isValid(zij);

    if (logUsed)
    {
        facetValid &= DecompositionUtils::isLogValid(zij);
    }

    /* Transposed relative to Grayplot */
    facetValid &= DecompositionUtils::isValid(values[getPointIndex(numY-1, numX-1, numY-2-j, i)]);

    /* Edge validity is always 1 since it is not used at all to determine facet validity for Matplot decomposition */
    *nextEdgeValid = 1;

    return facetValid;
}

int NgonGridMatplotDataDecomposer::isFacetEdgeValid(double* z, double* values, int numX, int numY, int i, int j, int logUsed)
{
    /* Always considered valid since not used at all to determine facet validity */
    return 1;
}

