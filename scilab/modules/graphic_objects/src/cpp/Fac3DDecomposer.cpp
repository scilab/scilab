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

#include "DecompositionUtils.hxx"
#include "ColorComputer.hxx"
#include "Fac3DDecomposer.hxx"

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int Fac3DDecomposer::getDataSize(char* id)
{
    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    return numVerticesPerGon*numGons;
}

void Fac3DDecomposer::fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* x;
    double* y;
    double* z;

    int i;

    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_int, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_int, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_int, (void**) &z);


    for (i = 0; i < numVerticesPerGon*numGons; i++)
    {
        if (coordinateMask & 0x1)
        {
            buffer[bufferOffset] = x[i] * scale[0] + translation[0];
        }

        if (coordinateMask & 0x2)
        {
            buffer[bufferOffset +1] = y[i] * scale[1] + translation[1];
        }

        if (coordinateMask & 0x4)
        {
            buffer[bufferOffset +2] = z[i] * scale[2] + translation[2];
        }

        if (elementsSize == 4 && (coordinateMask & 0x8))
        {
            buffer[bufferOffset +3] = 1.0;
        }

        bufferOffset += 4;
    }

}

void Fac3DDecomposer::fillColors(char* id, float* buffer, int bufferLength, int elementsSize)
{
    char* parentFigure;
    char* parent;

    double* colors;
    double* colormap;
    double* z;

    double color;

    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;
    int numColors = 0;
    int* piNumColors = &numColors;

    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    int colorFlag = 0;
    int* piColorFlag = &colorFlag;

    int dataMapping =  0;
    int* piDataMapping = &dataMapping;

    int perVertex = 0;
    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_COLORS__, jni_int, (void**) &piNumColors);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COLORS__, jni_double_vector, (void**) &colors);

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

    getGraphicObjectProperty(id, __GO_COLOR_FLAG__, jni_int, (void**) &piColorFlag);

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    /* Do not fill */
    if (colorFlag == 0)
    {
        return;
    }

    if (numColors == numGons*numVerticesPerGon)
    {
        perVertex = 1;
    }
    else if (numColors == numGons)
    {
        perVertex = 0;
    }


    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_int, (void**) &z);

    if (colorFlag == 1)
    {
        fillNormalizedZColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numGons, numVerticesPerGon);
    }
    else if (colorFlag > 1 && numColors == 0)
    {
        /* The color buffer must be filled with the color_mode value. */
        int colorMode = 0;
        int* piColorMode = &colorMode;

        getGraphicObjectProperty(id, __GO_COLOR_MODE__, jni_int, (void**) &piColorMode);

        color = (double) colorMode;
        color = DecompositionUtils::getAbsoluteValue(color);

        fillConstantColors(buffer, bufferLength, elementsSize, colormap, colormapSize,
            color, numGons, numVerticesPerGon);
    }
    else
    {
        fillDataColors(buffer, bufferLength, elementsSize, colormap, colormapSize,
            colors, colorFlag, perVertex, dataMapping, numGons, numVerticesPerGon);
    }

}

void Fac3DDecomposer::fillNormalizedZColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
    double* z, int numGons, int numVerticesPerGon)
{
    double zMin;
    double zMax;
    double zRange;
    double minDoubleValue;

    int i;
    int j;
    int bufferOffset = 0;

    computeMinMaxValues(z, numGons*numVerticesPerGon, numGons, numVerticesPerGon, ALL_VALUES, &zMin, &zMax);

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

    for (i = 0; i < numGons; i++)
    {
        /* Per-face average */
        double zavg = 0.0;

        float colorret[3];

        zavg = computeAverageValue(&z[i*numVerticesPerGon], numVerticesPerGon);

        ColorComputer::getColor(zavg, zMin, zRange, Z_COLOR_OFFSET, colormap, colormapSize, colorret);

        for (j = 0; j < numVerticesPerGon; j++)
        {
            buffer[bufferOffset] = colorret[0];
            buffer[bufferOffset+1] = colorret[1];
            buffer[bufferOffset+2] = colorret[2];

            if (elementsSize == 4)
            {
                buffer[bufferOffset +3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }

}

void Fac3DDecomposer::fillConstantColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
    double colorValue, int numGons, int numVerticesPerGon)
{
    int i;
    int bufferOffset = 0;

    float color[3];

    ColorComputer::getDirectColor(colorValue - 1.0, colormap, colormapSize, color);

    for (i = 0; i < numGons*numVerticesPerGon; i++)
    {
        buffer[bufferOffset] = color[0];
        buffer[bufferOffset+1] = color[1];
        buffer[bufferOffset+2] = color[2];

        if (elementsSize == 4)
        {
            buffer[bufferOffset+3] = 1.0;
        }

        bufferOffset += elementsSize;
    }

}

void Fac3DDecomposer::fillDataColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
    double* colors, int colorFlag, int perVertex, int dataMapping, int numGons, int numVerticesPerGon)
{
    double colMin;
    double colMax;
    double colRange;
    double color;

    int i;
    int j;
    int bufferOffset = 0;

    /* 0: colors are scaled */
    if (dataMapping == 0)
    {
        int minMaxComputation;
        int numColors;

        minMaxComputation = ALL_VALUES;

        if (perVertex)
        {
            numColors = numGons*numVerticesPerGon;

            if (colorFlag == 2)
            {
                minMaxComputation = FACE_AVERAGE;
            }
            else if (colorFlag == 4)
            {
                minMaxComputation = FIRST_VERTEX_VALUE;
            }
        }
        else
        {
            numColors = numGons;
        }

        computeMinMaxValues(colors, numColors, numGons, numVerticesPerGon, minMaxComputation, &colMin, &colMax);

        colRange = colMax - colMin;

        colMax -= 1.0;
        colMin -= 1.0;
    }

    for (i = 0; i < numGons; i++)
    {
        /* Per-face average */
        if (perVertex == 1 && colorFlag == 2)
        {
            color = computeAverageValue(&colors[i*numVerticesPerGon], numVerticesPerGon);
            color -= 1.0;
        }
        else if (perVertex == 0)
        {
            color = colors[i] - 1.0;
        }

        for (j = 0; j < numVerticesPerGon; j++)
        {
            if (perVertex == 1)
            {
                if (colorFlag == 3)
                {
                    color = colors[i*numVerticesPerGon+j] - 1.0;
                }
                else if (colorFlag == 4)
                {
                    color = colors[i*numVerticesPerGon] - 1.0;
                }
            }

            if (dataMapping == 1)
            {
                ColorComputer::getDirectColor(color, colormap, colormapSize, &buffer[bufferOffset]);
            }
            else if (dataMapping == 0)
            {
                ColorComputer::getColor(color, colMin, colRange, COLOR_OFFSET, colormap, colormapSize, &buffer[bufferOffset]);
            }

            if (elementsSize == 4)
            {
                buffer[bufferOffset +3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }

}

double Fac3DDecomposer::computeAverageValue(double* values, int numVertices)
{
    double averageValue;
    int i;

    averageValue = 0.0;

    for (i = 0; i < numVertices; i++)
    {
        averageValue += values[i];
    }

    averageValue /= (double) numVertices;

    return averageValue;
}

void Fac3DDecomposer::computeMinMaxValues(double* values, int numValues, int numGons, int numVerticesPerGon, int minMaxComputation,
    double* valueMin, double* valueMax)
{
    double maxDouble;
    double tmpValueMin;
    double tmpValueMax;
    double value;

    int i;
    int numIterations;

    maxDouble = DecompositionUtils::getMaxDoubleValue();

    tmpValueMin = maxDouble;
    tmpValueMax = -maxDouble;

    if (minMaxComputation != ALL_VALUES)
    {
        numIterations = numGons;
    }
    else
    {
        numIterations = numValues;
    }

    for (i = 0; i < numIterations; i++)
    {
        if (minMaxComputation == FIRST_VERTEX_VALUE)
        {
            value = values[i*numVerticesPerGon];
        }
        else if (minMaxComputation == FACE_AVERAGE)
        {
            value = computeAverageValue(&values[i*numVerticesPerGon], numVerticesPerGon);
        }
        else
        {
            value = values[i];
        }

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

int Fac3DDecomposer::getIndicesSize(char* id)
{
    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    if (numVerticesPerGon < 2)
    {
        return 0;
    }

    return 3*(numVerticesPerGon-2)*numGons;
}

/*
 * To do: use a polygon triangulation algorithm, as the fan decomposition used may produce
 * overlapping triangles for non-convex polygons.
 */
int Fac3DDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x;
    double* y;
    double* z;

    double xc;
    double yc;
    double zc;

    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    int i;
    int j;

    int bufferOffset = 0;
    int vertexOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    /* At least 3 vertices per N-gon are required to output triangles. */
    if (numVerticesPerGon < 3)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    for (i = 0; i < numGons; i++)
    {
        int isValid = 1;

        for (j = 0; j < numVerticesPerGon; j++)
        {
            xc = x[vertexOffset+j];
            yc = y[vertexOffset+j];
            zc = z[vertexOffset+j];

            if (!DecompositionUtils::isValid(xc, yc, zc) || !DecompositionUtils::isLogValid(xc, yc, zc, logMask))
            {
                isValid = 0;
                break;
            }
        }

        if (isValid == 0)
        {
            vertexOffset += numVerticesPerGon;
            continue;
        }

        /* Performs a fan decomposition */
        for (j = 0; j < numVerticesPerGon-2; j++)
        {
            buffer[bufferOffset] = vertexOffset;
            buffer[bufferOffset +1] = vertexOffset + j+1;
            buffer[bufferOffset +2] = vertexOffset + j+2;

            bufferOffset += 3;
        }

        vertexOffset += numVerticesPerGon;
    }

    return bufferOffset;
}

int Fac3DDecomposer::getWireIndicesSize(char* id)
{
    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    return 2*numVerticesPerGon*numGons;
}

int Fac3DDecomposer::fillWireIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x;
    double* y;
    double* z;

    double xc;
    double yc;
    double zc;

    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    int i;
    int j;

    int bufferOffset = 0;
    int vertexOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    /* At least 2 vertices per N-gon are required to output segments. */
    if (numVerticesPerGon < 2)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    for (i = 0; i < numGons; i++)
    {
        int isValid = 1;

        for (j = 0; j < numVerticesPerGon; j++)
        {
            xc = x[vertexOffset+j];
            yc = y[vertexOffset+j];
            zc = z[vertexOffset+j];

            if (!DecompositionUtils::isValid(xc, yc, zc) || !DecompositionUtils::isLogValid(xc, yc, zc, logMask))
            {
                isValid = 0;
                break;
            }
        }

        if (isValid == 0)
        {
            vertexOffset += numVerticesPerGon;
            continue;
        }

        for (j = 0; j < numVerticesPerGon; j++)
        {
            buffer[bufferOffset] = vertexOffset + j;
            buffer[bufferOffset+1] = vertexOffset + (j+1) % numVerticesPerGon;

            bufferOffset += 2;
        }

        vertexOffset += numVerticesPerGon;
    }

    return bufferOffset;
}

