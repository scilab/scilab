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

#include "ColorComputer.hxx"
#include "DecompositionUtils.hxx"
#include "NgonGridDataDecomposer.hxx"

extern "C"
{
#include <stdlib.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

NgonGridDataDecomposer* NgonGridDataDecomposer::decomposer = NULL;

int NgonGridDataDecomposer::getDataSize(int id)
{
    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

#if PER_VERTEX_VALUES
    return numX * numY;
#else
    return 4 * (numX - 1) * (numY - 1);
#endif
}

void NgonGridDataDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double zShift = 0.;
    double* pdZShift = &zShift;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    NgonGridDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double, (void**) &pdZShift);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    decomposer->fillGridVertices(buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, x, y, &zShift, numX, numY);
}

void NgonGridDataDecomposer::fillGridVertices(float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask,
        double* x, double* y, double* z, int numX, int numY)
{
    double xi = 0.;
    double yj = 0.;
    double zij = 0.;

    int bufferOffset = 0;

#if PER_VERTEX_VALUES
    for (int j = 0; j < numY; j++)
    {
        yj = y[j];

        if (coordinateMask  & 0x2)
        {
            if (logMask & 0x2)
            {
                yj = DecompositionUtils::getLog10Value(yj);
            }
        }

        for (int i = 0; i < numX; i++)
        {
            xi = x[i];
            bufferOffset = elementsSize * (numX * j + i);

            if (coordinateMask & 0x1)
            {
                if (logMask & 0x1)
                {
                    xi = DecompositionUtils::getLog10Value(xi);
                }

                buffer[bufferOffset] = xi * scale[0] + translation[0];
            }

            if (coordinateMask  & 0x2)
            {
                buffer[bufferOffset + 1] = yj * scale[1] + translation[1];
            }

            if (coordinateMask & 0x4)
            {
                zij = getZCoordinate(z, numX, numY, i, j, logMask & 0x4);

                buffer[bufferOffset + 2] = zij * scale[2] + translation[2];
            }

            if (elementsSize == 4 && (coordinateMask & 0x8))
            {
                buffer[bufferOffset + 3] = 1.0;
            }
        }
    }
#else

    double yjp1 = 0.;
    double xip1 = 0.;

    bufferOffset = 0;

    for (int j = 0; j < numY - 1; j++)
    {
        double ycoords[4];
        int yindices[4];

        yj = y[j];
        yjp1 = y[j + 1];

        if (coordinateMask  & 0x2)
        {
            if (logMask & 0x2)
            {
                yj = DecompositionUtils::getLog10Value(yj);
                yjp1 = DecompositionUtils::getLog10Value(yjp1);
            }
        }

        ycoords[0] = yj;
        ycoords[1] = yj;
        ycoords[2] = yjp1;
        ycoords[3] = yjp1;

        yindices[0] = j;
        yindices[1] = j;
        yindices[2] = j + 1;
        yindices[3] = j + 1;

        for (int i = 0; i < numX - 1; i++)
        {
            double xcoords[4];
            int xindices[4];

            xi = x[i];
            xip1 = x[i + 1];

            if (logMask & 0x1)
            {
                xi = DecompositionUtils::getLog10Value(xi);
                xip1 = DecompositionUtils::getLog10Value(xip1);
            }

            xcoords[0] = xi;
            xcoords[1] = xip1;
            xcoords[2] = xi;
            xcoords[3] = xip1;

            xindices[0] = i;
            xindices[1] = i + 1;
            xindices[2] = i;
            xindices[3] = i + 1;

            /*
             * If color values are defined per facet, we must duplicate shared vertices in order
             * to be able to render flat-shading facets, as the renderer uses smooth shading as a default.
             * Reducing duplication would require being able to enable flat shading at render time.
             */
            for (int k = 0; k < 4; k++)
            {
                if (coordinateMask & 0x1)
                {
                    buffer[bufferOffset] = (float)(xcoords[k] * scale[0] + translation[0]);
                }

                if (coordinateMask  & 0x2)
                {
                    buffer[bufferOffset + 1] = (float)(ycoords[k] * scale[1] + translation[1]);
                }

                if (coordinateMask & 0x4)
                {
                    zij = getZCoordinate(z, numX, numY, xindices[k], yindices[k], logMask & 0x4);

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

#endif

}

void NgonGridDataDecomposer::fillNormalizedZGridColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
        double* z, int numX, int numY)
{
    double zMin = 0.;
    double zMax = 0.;
    double zRange = 0.;
    double minDoubleValue = 0.;

    int bufferOffset = 0;

    computeMinMaxZValues(z, numX, numY, &zMin, &zMax);

    minDoubleValue = DecompositionUtils::getMinDoubleValue();

    /* To be verified */
    if ((zMax - zMin) < minDoubleValue)
    {
        zRange = 1.0;
    }
    else
    {
        zRange = zMax - zMin;
    }

#if PER_VERTEX_VALUES
    for (int j = 0; j < numY; j++)
    {
        for (int i = 0; i < numX; i++)
        {
            int currentPointIndex = getPointIndex(numX, numY, i, j);

            ColorComputer::getColor(z[currentPointIndex], zMin, zRange, Z_COLOR_OFFSET, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset + 3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }
#else
    for (int j = 0; j < numY - 1; j++)
    {
        for (int i = 0; i < numX - 1; i++)
        {
            double avgz;
            float facetColor[3];

            avgz = computeFacetAverageZValue(z, numX, numY, i, j);

            ColorComputer::getColor(avgz, zMin, zRange, Z_COLOR_OFFSET, colormap, colormapSize, facetColor);

            writeFacetColorToBuffer(buffer, bufferOffset, facetColor, elementsSize);

            bufferOffset += 4 * elementsSize;
        }
    }
#endif

}

void NgonGridDataDecomposer::fillDirectGridColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
        double* z, int numX, int numY)
{
    int currentPointIndex = 0;
    int bufferOffset = 0;

#if PER_VERTEX_VALUES
    for (int j = 0; j < numY; j++)
    {
        for (int i = 0; i < numX; i++)
        {
            currentPointIndex = getPointIndex(numX, numY, i, j);

            ColorComputer::getDirectColor(z[currentPointIndex] - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset + 3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }
#else
    for (int j = 0; j < numY - 1; j++)
    {
        for (int i = 0; i < numX - 1; i++)
        {
            float facetColor[3];

            currentPointIndex = getPointIndex(numX, numY, i, j);

            ColorComputer::getDirectColor(z[currentPointIndex] - 1.0, colormap, colormapSize, facetColor);

            writeFacetColorToBuffer(buffer, bufferOffset, facetColor, elementsSize);

            bufferOffset += 4 * elementsSize;
        }
    }
#endif

}

void NgonGridDataDecomposer::getFacetCoordinates(double* x, double* y, double*z, int numX, int numY, int i, int j,
        double vertices[4][3])
{
    vertices[0][0] = x[i];
    vertices[0][1] = y[j];
    vertices[0][2] = getZCoordinate(z, numX, numY, i, j);;

    vertices[1][0] = x[i + 1];
    vertices[1][1] = y[j];
    vertices[1][2] = getZCoordinate(z, numX, numY, i + 1, j);

    vertices[2][0] = x[i + 1];
    vertices[2][1] = y[j + 1];
    vertices[2][2] = getZCoordinate(z, numX, numY, i + 1, j + 1);

    vertices[3][0] = x[i];
    vertices[3][1] = y[j + 1];
    vertices[3][2] = getZCoordinate(z, numX, numY, i, j + 1);
}

double NgonGridDataDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j)
{
    return *z;
}

double NgonGridDataDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed)
{
    double zij = 0.;

    if (logUsed)
    {
        /* If the logarithmic scale is used, add the default offset */
        zij = *z + DEFAULT_LOG_COORD_Z;
    }
    else
    {
        zij = *z;
    }

    return zij;
}

double NgonGridDataDecomposer::getValue(double* values, int numX, int numY, int i, int j)
{
    return values[numX * j + i];
}

int NgonGridDataDecomposer::getIndicesSize(int id)
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

    return 6 * (numX - 1) * (numY - 1);
}


int NgonGridDataDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int numberIndices = 0;

    NgonGridDataDecomposer* decomposer = get();

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

    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, z, z, 1, numX, numY);

    return numberIndices;
}

/*
 * To do: merge with Plot3DDecomposer::fillWireIndices, as these functions perform a lot of redundant work.
 */
int NgonGridDataDecomposer::fillTriangleIndices(int* buffer, int bufferLength, int logMask, double* x, double* y, double* z, double* values, int perNodeValues, int numX, int numY)
{
    int bufferOffset = 0;

    int currentRowValid = 0;
    int nextRowValid = 0;

    int currentColumnValid = 0;
    int nextColumnValid = 0;

    int currentEdgeValid = 0;
    int nextEdgeValid = 0;

    int currentFacetValid = 0;

    int ij = 0;
    int ip1j = 0;
    int ip1jp1 = 0;
    int ijp1 = 0;

    /* 0 indices if less than 2 points along either dimension */
    if (numX < 2 || numY < 2)
    {
        return 0;
    }

    /* First row */
    currentRowValid = DecompositionUtils::isValid(y[0]);

    if (logMask & 0x2)
    {
        currentRowValid &= DecompositionUtils::isLogValid(y[0]);
    }

    /* To do: optimize */
    for (int j = 0; j < numY - 1; j++)
    {
        nextRowValid = DecompositionUtils::isValid(y[j + 1]);

        if (logMask & 0x2)
        {
            nextRowValid &= DecompositionUtils::isLogValid(y[j + 1]);
        }

        if (!currentRowValid || !nextRowValid)
        {
            currentRowValid = nextRowValid;
            continue;
        }
        else
        {
            currentRowValid = nextRowValid;
        }

        currentColumnValid = DecompositionUtils::isValid(x[0]);

        if (logMask & 0x1)
        {
            currentColumnValid &= DecompositionUtils::isLogValid(x[0]);
        }

        ij = getPointIndex(numX, numY, 0, j);
        ijp1 = getPointIndex(numX, numY, 0, j + 1);

        currentEdgeValid = isFacetEdgeValid(z, values, perNodeValues, numX, numY, 0, j, logMask & 0x4);

        for (int i = 0; i < numX - 1; i++)
        {
            nextColumnValid = DecompositionUtils::isValid(x[i + 1]);

            if (logMask & 0x1)
            {
                nextColumnValid &= DecompositionUtils::isLogValid(x[i + 1]);
            }

            ip1j = getPointIndex(numX, numY, i + 1, j);
            ip1jp1 = getPointIndex(numX, numY, i + 1, j + 1);

            currentFacetValid = isFacetValid(z, values, perNodeValues, numX, numY, i, j, logMask & 0x4, currentEdgeValid, &nextEdgeValid);

            if (currentColumnValid && nextColumnValid && (currentFacetValid))
            {
                int facetVertexIndices[4];
                int triangleVertexIndices[6];
                int firstVertexIndex;

#if PER_VERTEX_VALUES
                facetVertexIndices[0] = ij;
                facetVertexIndices[1] = ip1j;
                facetVertexIndices[2] = ip1jp1;
                facetVertexIndices[3] = ijp1;
#else
                firstVertexIndex = getFirstVertexIndex(numX, numY, i, j);

                facetVertexIndices[0] = firstVertexIndex;
                facetVertexIndices[1] = firstVertexIndex + 1;
                facetVertexIndices[2] = firstVertexIndex + 3;
                facetVertexIndices[3] = firstVertexIndex + 2;
#endif

                getFacetTriangles(x, y, z, numX, numY, i, j, facetVertexIndices, triangleVertexIndices);

                buffer[bufferOffset] = triangleVertexIndices[0];
                buffer[bufferOffset + 1] = triangleVertexIndices[1];
                buffer[bufferOffset + 2] = triangleVertexIndices[2];
                buffer[bufferOffset + 3] = triangleVertexIndices[3];
                buffer[bufferOffset + 4] = triangleVertexIndices[4];
                buffer[bufferOffset + 5] = triangleVertexIndices[5];

                bufferOffset += 6;
            }

            currentColumnValid = nextColumnValid;

            currentEdgeValid = nextEdgeValid;

            ij = ip1j;
            ijp1 = ip1jp1;
        }
    }

    return bufferOffset;
}

void NgonGridDataDecomposer::getFacetTriangles(double* x, double* y, double* z, int numX, int numY, int i, int j,
        int* facetVertexIndices, int* triangleVertexIndices)
{
    /* Facets are plane: they are all decomposed the same way */
    triangleVertexIndices[0] = facetVertexIndices[0];
    triangleVertexIndices[1] = facetVertexIndices[1];
    triangleVertexIndices[2] = facetVertexIndices[2];
    triangleVertexIndices[3] = facetVertexIndices[0];
    triangleVertexIndices[4] = facetVertexIndices[2];
    triangleVertexIndices[5] = facetVertexIndices[3];
}

int NgonGridDataDecomposer::isFacetValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid)
{
    *nextEdgeValid = isFacetEdgeValid(z, values, perNodeValues, numX, numY, i + 1, j, logUsed);

    if (currentEdgeValid && *nextEdgeValid)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int NgonGridDataDecomposer::isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed)
{
    double zij = 0.;
    double zijp1 = 0.;

    int lowerZValid = 0;
    int upperZValid = 0;

    zij = getZCoordinate(z, numX, numY, i, j);
    zijp1 = getZCoordinate(z, numX, numY, i, j + 1);

    lowerZValid = DecompositionUtils::isValid(zij);
    upperZValid = DecompositionUtils::isValid(zijp1);

    if (logUsed)
    {
        lowerZValid &= DecompositionUtils::isLogValid(zij);
        upperZValid &= DecompositionUtils::isLogValid(zijp1);
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

int NgonGridDataDecomposer::getPointIndex(int numX, int numY, int i, int j)
{
    return numX * j + i;
}

void NgonGridDataDecomposer::computeMinMaxZValues(double* z, int numX, int numY, double* zMin, double* zMax)
{
    double maxDouble = 0;
    double tmpZMin = 0;
    double tmpZMax = 0;
    double zi = 0;

    int currentPoint = 0;

    maxDouble = DecompositionUtils::getMaxDoubleValue();

    tmpZMin = maxDouble;
    tmpZMax = -maxDouble;

    for (int j = 0; j < numY; j++)
    {
        for (int i = 0; i < numX; i++)
        {
            currentPoint = getPointIndex(numX, numY, i, j);
            zi = z[currentPoint];

            if (DecompositionUtils::isValid(zi))
            {
                if (zi < tmpZMin)
                {
                    tmpZMin = zi;
                }

                if (zi > tmpZMax)
                {
                    tmpZMax = zi;
                }
            }
        }
    }

    *zMin = tmpZMin;
    *zMax = tmpZMax;
}

double NgonGridDataDecomposer::computeFacetAverageZValue(double* z, int numX, int numY, int i, int j)
{
    double avgz = 0.;
    int pointIndex = 0;

    avgz = 0.0;
    pointIndex = getPointIndex(numX, numY, i, j);
    avgz += z[pointIndex];
    pointIndex = getPointIndex(numX, numY, i + 1, j);
    avgz += z[pointIndex];
    pointIndex = getPointIndex(numX, numY, i, j + 1);
    avgz += z[pointIndex];
    pointIndex = getPointIndex(numX, numY, i + 1, j + 1);
    avgz += z[pointIndex];

    avgz *= 0.25;

    return avgz;
}

void NgonGridDataDecomposer::writeFacetColorToBuffer(float* buffer, int bufferOffset, float* color, int elementsSize, bool hasTransparency)
{
    for (int k = 0; k < 4; k++)
    {
        buffer[bufferOffset] = color[0];
        buffer[bufferOffset + 1] = color[1];
        buffer[bufferOffset + 2] = color[2];

        if (elementsSize == 4)
        {
            if (hasTransparency)
            {
                buffer[bufferOffset + 3] = color[3];
            }
            else
            {
                buffer[bufferOffset + 3] = 1.0;
            }
        }

        bufferOffset += elementsSize;
    }
}

int NgonGridDataDecomposer::getFirstVertexIndex(int numX, int numY, int i, int j)
{
    return 4 * j * (numX - 1) + 4 * i;
}

