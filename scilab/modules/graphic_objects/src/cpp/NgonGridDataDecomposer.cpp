/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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

extern "C"
{
#include <stdlib.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

NgonGridDataDecomposer* NgonGridDataDecomposer::decomposer = NULL;

int NgonGridDataDecomposer::getDataSize(char* id)
{
    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    return numX*numY;
}

void NgonGridDataDecomposer::fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* x;
    double* y;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    NgonGridDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    decomposer->fillGridVertices(buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, x, y, NULL, numX, numY);
}

void NgonGridDataDecomposer::fillGridVertices(float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask,
    double* x, double* y, double* z, int numX, int numY)
{
    double xi;
    double yj;
    double zij;

    int i;
    int j;

    int bufferOffset;

    for (j = 0; j < numY; j++)
    {
        yj = y[j];

        if (coordinateMask  & 0x2)
        {
            if (logMask & 0x2)
            {
                yj = DecompositionUtils::getLog10Value(yj);
            }
        }

        for (i = 0; i < numX; i++)
        {
            xi = x[i];
            bufferOffset = elementsSize*(numX*j + i);

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
                buffer[bufferOffset +1] = yj * scale[1] + translation[1];
            }

            if (coordinateMask & 0x4)
            {
                zij = getZCoordinate(z, numX, numY, i, j, logMask & 0x4);

                buffer[bufferOffset +2] = zij * scale[2] + translation[2];
            }

            if (elementsSize == 4 && (coordinateMask & 0x8))
            {
                buffer[bufferOffset +3] = 1.0;
            }
        }
    }

}

void NgonGridDataDecomposer::fillNormalizedZGridColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
    double* z, int numX, int numY)
{
    double zMin;
    double zMax;
    double zRange;
    double minDoubleValue;

    int i;
    int j;

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

    for (j = 0; j < numY; j++)
    {
        for (i = 0; i < numX; i++)
        {
            int currentPointIndex = getPointIndex(numX, numY, i, j);

            ColorComputer::getColor(z[currentPointIndex], zMin, zRange, Z_COLOR_OFFSET, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset +3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }

}

void NgonGridDataDecomposer::fillDirectGridColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
    double* z, int numX, int numY)
{
    int i;
    int j;

    int bufferOffset = 0;

    for (j = 0; j < numY; j++)
    {
        for (i = 0; i < numX; i++)
        {
            int currentPointIndex = getPointIndex(numX, numY, i, j);

            ColorComputer::getDirectColor(z[currentPointIndex], colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset +3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }

}

double NgonGridDataDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j)
{
    return 0.0;
}

double NgonGridDataDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed)
{
    double zij;

    if (logUsed)
    {
        zij = 1.0;
    }
    else
    {
        zij = 0.0;
    }

    return zij;
}

double NgonGridDataDecomposer::getZValue(double* z, int numX, int numY, int i, int j)
{
    return z[numX*j +i];
}

int NgonGridDataDecomposer::getIndicesSize(char* id)
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

    return 6*(numX-1)*(numY-1);
}


int NgonGridDataDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x;
    double* y;
    double* z;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int numberIndices;

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

    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, z, numX, numY);

    return numberIndices;
}

/*
 * To do: merge with Plot3DDecomposer::fillWireIndices, as these functions perform a lot of redundant work.
 */
int NgonGridDataDecomposer::fillTriangleIndices(int* buffer, int bufferLength, int logMask, double* x, double* y, double* z, int numX, int numY)
{
    int i;
    int j;
    int bufferOffset = 0;

    int currentRowValid;
    int nextRowValid;

    int currentColumnValid;
    int nextColumnValid;

    int currentEdgeValid;
    int nextEdgeValid;

    int ij;
    int ip1j;
    int ip1jp1;
    int ijp1;

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
    for (j = 0; j < numY-1; j++)
    {
        nextRowValid = DecompositionUtils::isValid(y[j+1]);

        if (logMask & 0x2)
        {
            nextRowValid &= DecompositionUtils::isLogValid(y[j+1]);
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
        ijp1 = getPointIndex(numX, numY, 0, j+1);

        currentEdgeValid = isFacetEdgeValid(z, numX, numY, 0, j, logMask & 0x4);

        for (i = 0; i < numX-1; i++)
        {
            nextColumnValid = DecompositionUtils::isValid(x[i+1]);

            if (logMask & 0x1)
            {
                nextColumnValid &= DecompositionUtils::isLogValid(x[i+1]);
            }

            ip1j = getPointIndex(numX, numY, i+1, j);
            ip1jp1 = getPointIndex(numX, numY, i+1, j+1);

            nextEdgeValid = isFacetEdgeValid(z, numX, numY, i+1, j, logMask & 0x4);

            if (currentColumnValid && nextColumnValid && (currentEdgeValid && nextEdgeValid))
            {
                /*
                 * All facets are currently decomposed the same way.
                 * To be adapted to the particular case of Plot3D objects.
                 */
                buffer[bufferOffset] = ij;
                buffer[bufferOffset+1] = ip1j;
                buffer[bufferOffset+2] = ip1jp1;
                buffer[bufferOffset+3] = ij;
                buffer[bufferOffset+4] = ip1jp1;
                buffer[bufferOffset+5] = ijp1;

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

int NgonGridDataDecomposer::isFacetEdgeValid(double* z, int numX, int numY, int i, int j, int logUsed)
{
    double zij;
    double zijp1;

    int lowerZValid;
    int upperZValid;

    zij = getZCoordinate(z, numX, numY, i, j);
    zijp1 = getZCoordinate(z, numX, numY, i, j+1);

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
    return numX*j + i;
}

void NgonGridDataDecomposer::computeMinMaxZValues(double* z, int numX, int numY, double* zMin, double* zMax)
{
    double maxDouble;
    double tmpZMin;
    double tmpZMax;
    double zi;

    int i;
    int j;
    int currentPoint;

    maxDouble = DecompositionUtils::getMaxDoubleValue();

    tmpZMin = maxDouble;
    tmpZMax = -maxDouble;

    for (j = 0; j < numY; j++)
    {
        for (i = 0; i < numX; i++)
        {
            currentPoint = getPointIndex(numX, numY, i, j);
            zi = z[currentPoint];

            if(zi < tmpZMin)
            {
                tmpZMin = zi;
            }

            if (zi > tmpZMax)
            {
                tmpZMax = zi;
            }
        }
    }

    *zMin = tmpZMin;
    *zMax = tmpZMax;
}

