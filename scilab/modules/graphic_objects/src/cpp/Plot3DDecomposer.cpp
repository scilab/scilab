/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#include <cstring>

#include "ColorComputer.hxx"
#include "DecompositionUtils.hxx"
#include "Plot3DDecomposer.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

Plot3DDecomposer* Plot3DDecomposer::decomposer = NULL;

void Plot3DDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    Plot3DDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    decomposer->fillGridVertices(buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, x, y, z, numX, numY);
}

double Plot3DDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j)
{
    double zij = 0.;

    zij = z[numX * j + i];

    return zij;
}

double Plot3DDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed)
{
    double zij = 0.;

    zij = z[numX * j + i];

    if (logUsed)
    {
        zij = DecompositionUtils::getLog10Value(zij);
    }

    return zij;
}

void Plot3DDecomposer::getFacetTriangles(double* x, double* y, double* z, int numX, int numY, int i, int j,
        int* facetVertexIndices, int* triangleVertexIndices)
{
    double vertices[4][3];

    /* Gets the facet's vertices: (i,j), (i+1,j), (i+1,j+1), (i,j+1) */
    getFacetCoordinates(x, y, z, numX, numY, i, j, vertices);

    /* Decomposes the facet into two triangles and outputs their indices */
    DecompositionUtils::getDecomposedQuadTriangleIndices(vertices, facetVertexIndices, triangleVertexIndices);
}

/*
 * To do: compute and return color indices instead of directly looking
 * up colors from the colormap.
 */
void Plot3DDecomposer::fillColors(int id, float* buffer, int bufferLength, int elementsSize)
{
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;
    int parent = 0;
    int* pparent = &parent;

    double* z = NULL;
    double* colormap = NULL;


    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    Plot3DDecomposer* decomposer = get();

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

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    decomposer->fillNormalizedZGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

int Plot3DDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int numberIndices = 0;

    Plot3DDecomposer* decomposer = get();

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

    /* The per-node value flag is set to 1, as grid values amount to z coordinates and are not relevant anyway. */
    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, z, z, 1, numX, numY);

    return numberIndices;
}

int Plot3DDecomposer::getWireIndicesSize(int id)
{
    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    return 2 * 2 * (numX - 1) * (numY - 1) + 2 * (numX - 1) + 2 * (numY - 1);
}

/*
 * To be optimized:
 * -a lot of work performed redundantly with NgonGridDataDecomposer::fillIndices, ought to be merged
 *  with it.
 */
int Plot3DDecomposer::fillWireIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int previousRowValid = 0;
    int currentRowValid = 0;
    int nextRowValid = 0;

    int previousColumnValid = 0;
    int currentColumnValid = 0;
    int nextColumnValid = 0;

    int lowerLeftZValid = 0;
    int lowerRightZValid = 0;
    int upperLeftZValid = 0;
    int upperRightZValid = 0;

    int jm1HorizontalEdgeZValid = 0;
    int im1VerticalEdgeZValid = 0;
    int jHorizontalEdgeZValid = 0;
    int iVerticalEdgeZValid = 0;
    int jp1HorizontalEdgeZValid = 0;
    int ip1VerticalEdgeZValid = 0;

    int ij = 0;
    int ip1j = 0;
    int ijp1 = 0;
    int ip1jp1 = 0;
    int ijm1 = 0;
    int ip1jm1 = 0;

    int firstVertexIndex = 0;

    int bufferOffset = 0;

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

    /* First row */
    previousRowValid = 0;
    currentRowValid = DecompositionUtils::isValid(y[0]);

    if (logMask & 0x2)
    {
        currentRowValid &= DecompositionUtils::isLogValid(y[0]);
    }

    /* Set to 0 as it is not relevant for the first row iteration */
    jm1HorizontalEdgeZValid = 0;

    /* x-axis and y-axis indices (respectively) */
    for (int j = 0; j < numY - 1; j++)
    {
        nextRowValid = DecompositionUtils::isValid(y[j + 1]);

        if (logMask & 0x2)
        {
            nextRowValid &= DecompositionUtils::isLogValid(y[j + 1]);
        }

        if (!currentRowValid)
        {
            previousRowValid = currentRowValid;
            currentRowValid = nextRowValid;
            continue;
        }

        previousColumnValid = 0;
        currentColumnValid = DecompositionUtils::isValid(x[0]);

        if (logMask & 0x1)
        {
            currentColumnValid &= DecompositionUtils::isLogValid(x[0]);
        }

        ij = getPointIndex(numX, numY, 0, j);
        ijp1 = getPointIndex(numX, numY, 0, j + 1);

        lowerLeftZValid = DecompositionUtils::isValid(z[ij]);
        upperLeftZValid = DecompositionUtils::isValid(z[ijp1]);

        if (logMask & 0x4)
        {
            lowerLeftZValid &= DecompositionUtils::isLogValid(z[ij]);
            upperLeftZValid &= DecompositionUtils::isLogValid(z[ijp1]);
        }

        iVerticalEdgeZValid = lowerLeftZValid && upperLeftZValid;

        /* Set to 0 as not relevant for the first column iteration */
        im1VerticalEdgeZValid = 0;

        for (int i = 0; i < numX - 1; i++)
        {
#if !PER_VERTEX_VALUES
            firstVertexIndex = getFirstVertexIndex(numX, numY, i, j);
#endif

            ip1j = getPointIndex(numX, numY, i + 1, j);
            ip1jp1 = getPointIndex(numX, numY, i + 1, j + 1);

            nextColumnValid = DecompositionUtils::isValid(x[i + 1]);

            if (logMask & 0x1)
            {
                nextColumnValid &= DecompositionUtils::isLogValid(x[i + 1]);
            }

            lowerRightZValid = DecompositionUtils::isValid(z[ip1j]);
            upperRightZValid = DecompositionUtils::isValid(z[ip1jp1]);

            if (logMask & 0x4)
            {
                lowerRightZValid &= DecompositionUtils::isLogValid(z[ip1j]);
                upperRightZValid &= DecompositionUtils::isLogValid(z[ip1jp1]);
            }

            if (j > 0)
            {
                ijm1 = getPointIndex(numX, numY, i, j - 1);
                ip1jm1 = getPointIndex(numX, numY, i + 1, j - 1);

                jm1HorizontalEdgeZValid = DecompositionUtils::isValid(z[ijm1]) && DecompositionUtils::isValid(z[ip1jm1]);

                if (logMask & 0x4)
                {
                    jm1HorizontalEdgeZValid &= (DecompositionUtils::isLogValid(z[ijm1]) && DecompositionUtils::isLogValid(z[ip1jm1]));
                }
            }

            jHorizontalEdgeZValid = lowerLeftZValid && lowerRightZValid;

            jp1HorizontalEdgeZValid = upperLeftZValid && upperRightZValid;
            ip1VerticalEdgeZValid = lowerRightZValid && upperRightZValid;

            /*
             * Two segments: between points (i,j) and (i+1,j)
             * and points (i,j) and (i,j+1) .
             */
            if ((currentColumnValid && nextColumnValid) && jHorizontalEdgeZValid && ((previousRowValid && jm1HorizontalEdgeZValid) || (nextRowValid && jp1HorizontalEdgeZValid)))
            {
#if PER_VERTEX_VALUES
                buffer[bufferOffset] = ij;
                buffer[bufferOffset + 1] = ip1j;
#else
                buffer[bufferOffset] = firstVertexIndex;
                buffer[bufferOffset + 1] = firstVertexIndex + 1;
#endif

                bufferOffset += 2;
            }

            if (currentColumnValid && nextRowValid && iVerticalEdgeZValid && ((previousColumnValid && im1VerticalEdgeZValid) || (nextColumnValid && ip1VerticalEdgeZValid)))
            {
#if PER_VERTEX_VALUES
                buffer[bufferOffset] = ij;
                buffer[bufferOffset + 1] = ijp1;
#else
                buffer[bufferOffset] = firstVertexIndex;
                buffer[bufferOffset + 1] = firstVertexIndex + 2;
#endif

                bufferOffset += 2;
            }

            previousColumnValid = currentColumnValid;
            currentColumnValid = nextColumnValid;
            lowerLeftZValid = lowerRightZValid;
            upperLeftZValid = upperRightZValid;

            im1VerticalEdgeZValid = iVerticalEdgeZValid;
            iVerticalEdgeZValid = ip1VerticalEdgeZValid;

#if PER_VERTEX_VALUES
            ij = ip1j;
            ijp1 = ip1jp1;
#endif
        }

        /* Rightmost vertical line */
        if (currentColumnValid && nextRowValid && iVerticalEdgeZValid && (previousColumnValid && im1VerticalEdgeZValid))
        {
#if PER_VERTEX_VALUES
            buffer[bufferOffset] = ij;
            buffer[bufferOffset + 1] = ijp1;
#else
            firstVertexIndex = getFirstVertexIndex(numX, numY, numX - 2, j);

            buffer[bufferOffset] = firstVertexIndex + 1;
            buffer[bufferOffset + 1] = firstVertexIndex + 3;
#endif

            bufferOffset += 2;
        }

        previousRowValid = currentRowValid;
        currentRowValid = nextRowValid;
    }

    /* Topmost horizontal lines */
    currentRowValid = DecompositionUtils::isValid(y[numY - 1]);

    if (logMask & 0x2)
    {
        currentRowValid &= DecompositionUtils::isLogValid(y[numY - 1]);
    }

    currentColumnValid = DecompositionUtils::isValid(x[0]);

    if (logMask & 0x1)
    {
        currentColumnValid = DecompositionUtils::isValid(x[0]);
    }

    ij = getPointIndex(numX, numY, 0, numY - 1);

    lowerLeftZValid = DecompositionUtils::isValid(z[ij]);

    if (logMask & 0x4)
    {
        lowerLeftZValid &= DecompositionUtils::isValid(z[ij]);
    }

    ij = getPointIndex(numX, numY, 0, numY - 1);

    for (int i = 0; i < numX - 1; i++)
    {
        nextColumnValid = DecompositionUtils::isValid(x[i + 1]);

        if (logMask & 0x1)
        {
            nextColumnValid &= DecompositionUtils::isLogValid(x[i + 1]);
        }

        ip1j = getPointIndex(numX, numY, i + 1, numY - 1);

        lowerRightZValid = DecompositionUtils::isValid(z[ip1j]);

        if (logMask & 0x4)
        {
            lowerRightZValid &= DecompositionUtils::isLogValid(z[ip1j]);
        }

        ijm1 = getPointIndex(numX, numY, i, numY - 2);
        ip1jm1 = getPointIndex(numX, numY, i + 1, numY - 2);

        jm1HorizontalEdgeZValid = DecompositionUtils::isValid(z[ijm1]) && DecompositionUtils::isValid(z[ip1jm1]);

        if (logMask & 0x4)
        {
            jm1HorizontalEdgeZValid &= (DecompositionUtils::isLogValid(z[ijm1]) && DecompositionUtils::isLogValid(z[ip1jm1]));
        }

        jHorizontalEdgeZValid = lowerLeftZValid && lowerRightZValid;

        if (currentRowValid && (currentColumnValid && nextColumnValid) && jHorizontalEdgeZValid && (previousRowValid && jm1HorizontalEdgeZValid))
        {
#if PER_VERTEX_VALUES
            buffer[bufferOffset] = ij;
            buffer[bufferOffset + 1] = ip1j;
#else
            firstVertexIndex = getFirstVertexIndex(numX, numY, i, numY - 2);

            buffer[bufferOffset] = firstVertexIndex + 2;
            buffer[bufferOffset + 1] = firstVertexIndex + 3;
#endif

            bufferOffset += 2;
        }

        currentColumnValid = nextColumnValid;
        lowerLeftZValid = lowerRightZValid;

        ij = ip1j;
    }

    return bufferOffset;
}

