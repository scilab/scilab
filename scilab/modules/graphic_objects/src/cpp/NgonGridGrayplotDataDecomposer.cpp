/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
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
 * fillVertices: override to support both vector and matrix grid modes.
 * Matrix mode is signalled by xDimensions[1] > 1 (gridSize[1] > 1).
 * In matrix mode, x and y hold full (numX x numY) coordinate matrices and
 * the non-parent fillGridVerticesMatrix is used.
 * In vector mode, delegates to the base class fillVertices.
 */
void NgonGridGrayplotDataDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize,
        int coordinateMask, double* scale, double* translation, int logMask)
{
    /* Detect matrix mode from xDimensions[1] > 1 */
    int* xDims = NULL;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_DIMENSIONS__, jni_int_vector, (void**) &xDims);

    if (xDims == NULL || xDims[1] <= 1)
    {
        /* Vector (rectangular) mode: use parent implementation */
        NgonGridDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        return;
    }

    /* Matrix (non-rectangular) mode: numX = xDims[0], numY = xDims[1] */
    int numX = xDims[0];
    int numY = xDims[1];

    double* xMatrix = NULL;
    double* yMatrix = NULL;
    double zShift = 0.;
    double* pdZShift = &zShift;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &xMatrix);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &yMatrix);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double, (void**) &pdZShift);

    NgonGridGrayplotDataDecomposer* decomp = get();
    decomp->fillGridVerticesMatrix(buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask,
                                   xMatrix, yMatrix, &zShift, numX, numY);
}

/*
 * fillGridVerticesMatrix: fills the vertex buffer for a non-rectangular grid.
 * x[i + j*numX] and y[i + j*numX] are the coordinates of node (i,j).
 * For PER_VERTEX_VALUES=0 (per-facet mode), each quad (i,j) contributes 4 vertices.
 */
void NgonGridGrayplotDataDecomposer::fillGridVerticesMatrix(float* buffer, int bufferLength, int elementsSize,
        int coordinateMask, double* scale, double* translation, int logMask,
        double* x, double* y, double* z, int numX, int numY)
{
    int bufferOffset = 0;

    for (int j = 0; j < numY - 1; j++)
    {
        for (int i = 0; i < numX - 1; i++)
        {
            /*
             * 4 corners of quad (i,j) in column-major order:
             *   vertex 0: (i,   j  )  → index  i    + j   *numX
             *   vertex 1: (i+1, j  )  → index (i+1) + j   *numX
             *   vertex 2: (i,   j+1)  → index  i    +(j+1)*numX
             *   vertex 3: (i+1, j+1)  → index (i+1)+(j+1)*numX
             */
            int idx[4];
            idx[0] = i       + j       * numX;
            idx[1] = (i + 1) + j       * numX;
            idx[2] = i       + (j + 1) * numX;
            idx[3] = (i + 1) + (j + 1) * numX;

            int xindices[4] = {i, i + 1, i,     i + 1};
            int yindices[4] = {j, j,     j + 1, j + 1};

            for (int k = 0; k < 4; k++)
            {
                double xi = x[idx[k]];
                double yj = y[idx[k]];

                if (coordinateMask & 0x1)
                {
                    if (logMask & 0x1)
                    {
                        xi = DecompositionUtils::getLog10Value(xi);
                    }
                    buffer[bufferOffset] = (float)(xi * scale[0] + translation[0]);
                }

                if (coordinateMask & 0x2)
                {
                    if (logMask & 0x2)
                    {
                        yj = DecompositionUtils::getLog10Value(yj);
                    }
                    buffer[bufferOffset + 1] = (float)(yj * scale[1] + translation[1]);
                }

                if (coordinateMask & 0x4)
                {
                    double zij = getZCoordinate(z, numX, numY, xindices[k], yindices[k], logMask & 0x4);
                    buffer[bufferOffset + 2] = (float)(zij * scale[2] + translation[2]);
                }

                if (elementsSize == 4 && (coordinateMask & 0x8))
                {
                    buffer[bufferOffset + 3] = 1.0;
                }

                bufferOffset += elementsSize;
            }
        }
    }
}

/*
 * To do: allowing either per-vertex or per-facet colors
 */
void NgonGridGrayplotDataDecomposer::fillColors(int id, float* buffer, int bufferLength, int elementsSize)
{
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;
    int parent = 0;

    double* z = NULL;
    double* colormap = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int dataMapping = 0;
    int* piDataMapping = &dataMapping;

    NgonGridGrayplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    parent = getParentObject(id);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parent == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &pparentFigure);

    if (parentFigure == 0)
    {
        return;
    }

    colormapSize = ColorComputer::getClosestColormap(id, &colormap);

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    if (dataMapping == 0)
    {
        decomposer->fillNormalizedZGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);
    }
    else if (dataMapping == 1)
    {
        decomposer->fillDirectGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);
    }

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

int NgonGridGrayplotDataDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
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
    int dataMapping = 0;
    int* piDataMapping = &dataMapping;

    int perNodeValues = 1;

    int numberIndices = 0;

    NgonGridGrayplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* 0 indices if 0 points */
    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double, (void**) &pdZShift);

    /*
     * Data mapping values 0 and 1 respectively correspond to scaled and direct colors, for which per-node
     * and per-facet values are respectively used.
     */
    perNodeValues = !dataMapping;

    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, &zShift, z, perNodeValues, numX, numY);

    return numberIndices;
}

int NgonGridGrayplotDataDecomposer::isFacetValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid)
{
    *nextEdgeValid = isFacetEdgeValid(z, values, perNodeValues, numX, numY, i + 1, j, logUsed);

    if (!perNodeValues)
    {
        /*
         * Grid values are defined per facet.
         * In addition to edge validity, which in this case is determined only by z coordinates, the
         * facet's (i,j) value must also be read in order to determine its overall validity.
         */
        double zij = getValue(values, numX, numY, i, j);

        if (DecompositionUtils::isValid(zij) && currentEdgeValid && *nextEdgeValid)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        /*
         * Grid values are defined per node, and have therefore been already used
         * to determine edge validity
         */
        if (currentEdgeValid && *nextEdgeValid)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/*
 * To be merged with its parent's isFacetEdgeValid function.
 */
int NgonGridGrayplotDataDecomposer::isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed)
{
    double zij = 0.;
    double zijp1 = 0.;

    int lowerZValid = 0;
    int upperZValid = 0;

    /* First, z-coordinate values are tested */
    zij = getZCoordinate(z, numX, numY, i, j, logUsed);
    zijp1 = getZCoordinate(z, numX, numY, i, j + 1, logUsed);

    lowerZValid = DecompositionUtils::isValid(zij);
    upperZValid = DecompositionUtils::isValid(zijp1);

    if (logUsed)
    {
        lowerZValid &= DecompositionUtils::isLogValid(zij);
        upperZValid &= DecompositionUtils::isLogValid(zijp1);
    }

    /*
     * If values are defined per node, edge validity must also
     * take into account grid values at the edge's nodes.
     */
    if (perNodeValues)
    {
        zij = getValue(values, numX, numY, i, j);
        zijp1 = getValue(values, numX, numY, i, j + 1);

        lowerZValid &= DecompositionUtils::isValid(zij);
        upperZValid &= DecompositionUtils::isValid(zijp1);
    }

    if (lowerZValid && upperZValid)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

