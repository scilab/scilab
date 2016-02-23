/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
#include "MeshFecDataDecomposer.hxx"

#include <iostream>

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int MeshFecDataDecomposer::getDataSize(int id)
{
    int numVertices = 0;
    int* piNumVertices = &numVertices;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**) &piNumVertices);

    return numVertices;
}

void MeshFecDataDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* coordinates = NULL;

    int numVertices = 0;
    int* piNumVertices = &numVertices;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &coordinates);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**) &piNumVertices);

    for (int i = 0; i < numVertices; i++)
    {
        if (coordinateMask & 0x1)
        {
            double xi = coordinates[3 * i];
            if (logMask & 0x1)
            {
                xi = DecompositionUtils::getLog10Value(xi);
            }
            buffer[elementsSize * i] = (float)(xi * scale[0] + translation[0]);
        }

        if (coordinateMask & 0x2)
        {
            double yi = coordinates[3 * i + 1];
            if (logMask & 0x2)
            {
                yi = DecompositionUtils::getLog10Value(yi);
            }
            buffer[elementsSize * i + 1] = (float)(yi * scale[1] + translation[1]);
        }

        if (coordinateMask & 0x4)
        {
            double zi = coordinates[3 * i + 2];
            if (logMask & 0x4)
            {
                zi = DecompositionUtils::getLog10Value(zi);
            }
            buffer[elementsSize * i + 2] = (float)(zi * scale[2] + translation[2]);
        }

        if (elementsSize == 4 && (coordinateMask & 0x8))
        {
            buffer[elementsSize * i + 3] = 1.0;
        }
    }

}

void MeshFecDataDecomposer::fillTextureCoordinates(int id, float* buffer, int bufferLength)
{
    int parentFigure = 0;
    int * piParentFigure = &parentFigure;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    int* colorRange = NULL;

    double colorsNumber = 0.;
    double scale = 0.;
    double t = 0.;

    double* values = NULL;
    double* zBounds = NULL;

    double minValue = 0.;
    double maxValue = 0.;

    int numVertices = 0;
    int* piNumVertices = &numVertices;

    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &piParentFigure);
    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parentFigure == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_COLOR_RANGE__, jni_int_vector, (void**) &colorRange);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    if (colorRange[0] != 0 || colorRange[1] != 0)
    {
        colorsNumber = (double) (1 + colorRange[1] - colorRange[0]);
    }
    else
    {
        colorsNumber = (double) colormapSize;
    }

    releaseGraphicObjectProperty(__GO_COLOR_RANGE__, colorRange, jni_int_vector, 0);

    /** To take into account the presence of exterior colors:
     *  - We add 2 to the number of colors.
     *  - We skip the first color.
     */
    t = 3. / (2. * (colorsNumber + 2.));
    scale = (colorsNumber - 1.) / (colorsNumber + 2);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**) &piNumVertices);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_VALUES__, jni_double_vector, (void**) &values);
    getGraphicObjectProperty(id, __GO_Z_BOUNDS__, jni_double_vector, (void**) &zBounds);

    /* Z-bounds are not taken into account if either of them is invalid */
    if ((zBounds[0] != 0.0 || zBounds[1] != 0.0) && (DecompositionUtils::isValid(zBounds[0]) && DecompositionUtils::isValid(zBounds[1])) && (zBounds[0] != zBounds[1]))
    {
        minValue = zBounds[0];
        maxValue = zBounds[1];
    }
    else
    {
        computeMinMaxValues(values, numVertices, &minValue, &maxValue);
    }

    releaseGraphicObjectProperty(__GO_Z_BOUNDS__, zBounds, jni_double_vector, 0);

    if (maxValue == minValue)
    {
        for (int i = 0; i < numVertices; i++)
        {
            buffer[bufferOffset++] = (float)minValue;
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 1;
        }
    }
    else
    {
        for (int i = 0; i < numVertices; i++)
        {
            buffer[bufferOffset++] = (float)(t + scale * (values[i] - minValue) / (maxValue - minValue));
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 1;
        }
    }
}

void MeshFecDataDecomposer::fillColors(int id, float* buffer, int bufferLength, int elementsSize)
{
    int parent = 0;
    int parentFigure = 0;
    int * piParentFigure = &parentFigure;

    double* values = NULL;
    double* zBounds = NULL;
    double* colormap = NULL;

    double minValue = 0.;
    double maxValue = 0.;
    double valueRange = 0.;

    float minColor[3];
    float maxColor[3];

    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    int numVertices = 0;
    int* piNumVertices = &numVertices;

    int minColorIndex = 0;
    int maxColorIndex = 0;

    int* colorRange = NULL;
    int useOutsideColors = 0;
    int bufferOffset = 0;

    parent = getParentObject(id);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parent == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &piParentFigure);

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**) &piNumVertices);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_VALUES__, jni_double_vector, (void**) &values);

    getGraphicObjectProperty(id, __GO_Z_BOUNDS__, jni_double_vector, (void**) &zBounds);
    getGraphicObjectProperty(id, __GO_COLOR_RANGE__, jni_int_vector, (void**) &colorRange);

    if (colorRange[0] != 0 || colorRange[1] != 0)
    {
        /* To do: use a scilab index to colormap index conversion function */
        minColorIndex = colorRange[0] - 1;
        maxColorIndex = colorRange[1] - 1;

        if (minColorIndex < 0)
        {
            minColorIndex = 0;
        }
        if (maxColorIndex < 0)
        {
            maxColorIndex = 0;
        }

        if (maxColorIndex > colormapSize - 1)
        {
            maxColorIndex = colormapSize - 1;
        }
        if (minColorIndex > colormapSize - 1)
        {
            minColorIndex = colormapSize - 1;
        }

    }
    else
    {
        minColorIndex = 0;
        maxColorIndex = colormapSize - 1;
    }

    releaseGraphicObjectProperty(__GO_COLOR_RANGE__, colorRange, jni_int_vector, 0);

    computeMinMaxValues(values, numVertices, &minValue, &maxValue);

    /* Z-bounds are not taken into account if either of them is invalid */
    if ((zBounds[0] != 0.0 || zBounds[1] != 0.0) && (DecompositionUtils::isValid(zBounds[0]) && DecompositionUtils::isValid(zBounds[1])) && (zBounds[0] != zBounds[1]))
    {
        int* outsideColors;

        minValue = zBounds[0];
        maxValue = zBounds[1];

        getGraphicObjectProperty(id, __GO_OUTSIDE_COLOR__, jni_int_vector, (void**) &outsideColors);

        if (outsideColors[0] != 0 || outsideColors[1] != 0)
        {
            useOutsideColors = 1;

            ColorComputer::getDirectColor((double) outsideColors[0] - 1.0, colormap, colormapSize, minColor);
            ColorComputer::getDirectColor((double) outsideColors[1] - 1.0, colormap, colormapSize, maxColor);
        }
    }

    releaseGraphicObjectProperty(__GO_Z_BOUNDS__, zBounds, jni_double_vector, 0);

    /* To be verified (when reverse z bounds are specified) */
    if (DecompositionUtils::getAbsoluteValue(maxValue - minValue) < DecompositionUtils::getMinDoubleValue())
    {
        valueRange = 1.0;
    }
    else
    {
        valueRange = maxValue - minValue;
    }

    for (int i = 0; i < numVertices; i++)
    {
        bufferOffset = elementsSize * i;

        if (useOutsideColors == 1)
        {
            if (values[i] < minValue)
            {
                buffer[bufferOffset] = minColor[0];
                buffer[bufferOffset + 1] = minColor[1];
                buffer[bufferOffset + 2] = minColor[2];
            }
            else if (values[i] > maxValue)
            {
                buffer[bufferOffset] = maxColor[0];
                buffer[bufferOffset + 1] = maxColor[1];
                buffer[bufferOffset + 2] = maxColor[2];
            }
            else
            {
                /* To do: replace 0.5 by a macro-definition */
                ColorComputer::getColor(values[i], minValue, valueRange, 0.5, colormap, minColorIndex, maxColorIndex, colormapSize, &buffer[bufferOffset]);
            }
        }
        else
        {
            /* To do: replace 0.5 by a macro-definition */
            ColorComputer::getColor(values[i], minValue, valueRange, 0.5, colormap, minColorIndex, maxColorIndex, colormapSize, &buffer[bufferOffset]);
        }

        if (elementsSize == 4)
        {
            buffer[bufferOffset + 3] = 1.0;
        }
    }

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

void MeshFecDataDecomposer::computeMinMaxValues(double* values, int numValues, double* valueMin, double* valueMax)
{
    double maxDouble = DecompositionUtils::getMaxDoubleValue();
    double tmpValueMin = maxDouble;
    double tmpValueMax = -maxDouble;
    double value = 0.;

    for (int i = 0; i < numValues; i++)
    {
        value = values[i];

        if (value < tmpValueMin)
        {
            tmpValueMin = value;
        }

        if (value > tmpValueMax)
        {
            tmpValueMax = value;
        }
    }

    *valueMin = tmpValueMin;
    *valueMax = tmpValueMax;
}

int MeshFecDataDecomposer::getIndicesSize(int id)
{
    int numIndices = 0;
    int* piNumIndices = &numIndices;
    int nVertex = 0;
    int* piNVertex = &nVertex;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_INDICES__, jni_int, (void**) &piNumIndices);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, jni_int, (void**) &piNVertex);

    return 3 * (nVertex - 2) * numIndices;
}

int MeshFecDataDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* coordinates = NULL;
    double* values = NULL;

    int numIndices = 0;
    int* piNumIndices = &numIndices;
    int numVertices = 0;
    int* piNumVertices = &numVertices;
    int nVertex = 0;
    int* piNVertex = &nVertex;

    int* triangleIndices = NULL;

    int v0 = 0;
    int v1 = 0;
    int v2 = 0;
    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_INDICES__, jni_int, (void**) &piNumIndices);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**) &piNumVertices);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, jni_int, (void**) &piNVertex);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &coordinates);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_VALUES__, jni_double_vector, (void**) &values);

    /* 0 facets */
    if (numIndices == 0 || numVertices < 3)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_INDICES__, jni_int_vector, (void**) &triangleIndices);

    for (int i = 0; i < numIndices; i++)
    {
        v0 = triangleIndices[nVertex * i];
        for (unsigned int j = 1; j < nVertex - 1; ++j)
        {
            v1 = triangleIndices[nVertex * i + j];
            v2 = triangleIndices[nVertex * i + j + 1];

            if (areFaceIndicesValid(numVertices, v0, v1, v2) &&
                    areFaceVerticesValid(coordinates, v0, v1, v2, logMask) &&
                    areFaceValuesValid(values, v0, v1, v2))
            {
                buffer[bufferOffset] = v0;
                buffer[bufferOffset + 1] = v1;
                buffer[bufferOffset + 2] = v2;
                bufferOffset += 3;
            }
        }
    }

    return bufferOffset;
}

int MeshFecDataDecomposer::areFaceVerticesValid(double* coordinates, int v0, int v1, int v2, int logMask)
{
    double vertex0[3];
    double vertex1[3];
    double vertex2[3];

    getVertexCoordinates(coordinates, v0, vertex0);
    getVertexCoordinates(coordinates, v1, vertex1);
    getVertexCoordinates(coordinates, v2, vertex2);

    if (DecompositionUtils::isValid(vertex0[0], vertex0[1], vertex0[2]) &&
            DecompositionUtils::isLogValid(vertex0[0], vertex0[1], vertex0[2], logMask) &&
            DecompositionUtils::isValid(vertex1[0], vertex1[1], vertex1[2]) &&
            DecompositionUtils::isLogValid(vertex1[0], vertex1[1], vertex1[2], logMask) &&
            DecompositionUtils::isValid(vertex2[0], vertex2[1], vertex2[2]) &&
            DecompositionUtils::isLogValid(vertex2[0], vertex2[1], vertex2[2], logMask))
    {
        return 1;
    }

    return 0;
}

int MeshFecDataDecomposer::areSegmentVerticesValid(double* coordinates, int v0, int v1, int logMask)
{
    double vertex0[3];
    double vertex1[3];

    getVertexCoordinates(coordinates, v0, vertex0);
    getVertexCoordinates(coordinates, v1, vertex1);

    if (DecompositionUtils::isValid(vertex0[0], vertex0[1], vertex0[2]) &&
            DecompositionUtils::isLogValid(vertex0[0], vertex0[1], vertex0[2], logMask) &&
            DecompositionUtils::isValid(vertex1[0], vertex1[1], vertex1[2]) &&
            DecompositionUtils::isLogValid(vertex1[0], vertex1[1], vertex1[2], logMask))
    {
        return 1;
    }

    return 0;
}

int MeshFecDataDecomposer::areFaceValuesValid(double* values, int v0, int v1, int v2)
{
    if (DecompositionUtils::isValid(values[v0], values[v1], values[v2]))
    {
        return 1;
    }

    return 0;
}

int MeshFecDataDecomposer::areSegmentValuesValid(double* values, int v0, int v1)
{
    if (DecompositionUtils::isValid(values[v0], values[v1]))
    {
        return 1;
    }

    return 0;
}

int MeshFecDataDecomposer::areFaceIndicesValid(int numVertices, int v0, int v1, int v2)
{
    if (v0 < 0 || v0 >= numVertices || v1 < 0 || v1 >= numVertices || v2 < 0 || v2 >= numVertices)
    {
        return 0;
    }

    return 1;
}

int MeshFecDataDecomposer::areSegmentIndicesValid(int numVertices, int v0, int v1)
{
    if (v0 < 0 || v0 >= numVertices || v1 < 0 || v1 >= numVertices)
    {
        return 0;
    }

    return 1;
}


void MeshFecDataDecomposer::getVertexCoordinates(double* coordinates, int index, double* vertexCoordinates)
{
    vertexCoordinates[0] = coordinates[3 * index];
    vertexCoordinates[1] = coordinates[3 * index + 1];
    vertexCoordinates[2] = coordinates[3 * index + 2];
}

int MeshFecDataDecomposer::getWireIndicesSize(int id)
{
    int numTriangles = 0;
    int* piNumTriangles = &numTriangles;
    int nVertex = 0;
    int* piNVertex = &nVertex;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_INDICES__, jni_int, (void**) &piNumTriangles);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, jni_int, (void**) &piNVertex);

    return 2 * nVertex * numTriangles;
}

/*
 * To do: output shared edges once instead of twice (once per adjacent face).
 */
int MeshFecDataDecomposer::fillWireIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* coordinates = NULL;
    double* values = NULL;

    int numVertices = 0;
    int* piNumVertices = &numVertices;
    int numIndices = 0;
    int* piNumIndices = &numIndices;
    int* triangleIndices = NULL;
    int nVertex = 0;
    int* piNVertex = &nVertex;

    int v0 = 0;
    int v1 = 0;
    int v2 = 0;
    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_INDICES__, jni_int, (void**) &piNumIndices);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**) &piNumVertices);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &coordinates);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_VALUES__, jni_double_vector, (void**) &values);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, jni_int, (void**) &piNVertex);

    /* 0 segments */
    if (numIndices == 0 || numVertices < 3)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_INDICES__, jni_int_vector, (void**) &triangleIndices);

    for (int i = 0; i < numIndices; i++)
    {
        for (unsigned int j = 0; j < nVertex - 1; ++j)
        {
            v0 = triangleIndices[nVertex * i + j];
            v1 = triangleIndices[nVertex * i + j + 1];
            if (areSegmentIndicesValid(numVertices, v0, v1) &&
                    areSegmentVerticesValid(coordinates, v0, v1, logMask) &&
                    areSegmentValuesValid(values, v0, v1))
            {
                buffer[bufferOffset] = v0;
                buffer[bufferOffset + 1] = v1;
                bufferOffset += 2;
            }
        }

        v0 = triangleIndices[nVertex * i + nVertex - 1];
        v1 = triangleIndices[nVertex * i];
        if (areSegmentIndicesValid(numVertices, v0, v1) &&
                areSegmentVerticesValid(coordinates, v0, v1, logMask) &&
                areSegmentValuesValid(values, v0, v1))
        {
            buffer[bufferOffset] = v0;
            buffer[bufferOffset + 1] = v1;
            bufferOffset += 2;
        }
    }

    return bufferOffset;
}

