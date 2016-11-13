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

#include "DecompositionUtils.hxx"
#include "ColorComputer.hxx"
#include "Fac3DColorComputer.hxx"
#include "Fac3DDecomposer.hxx"
#include "Triangulator.hxx"

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int Fac3DDecomposer::getDataSize(int id)
{
    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    return numVerticesPerGon * numGons;
}

void Fac3DDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    int i = 0;

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


    for (i = 0; i < numVerticesPerGon * numGons; i++)
    {
        if (coordinateMask & 0x1)
        {
            double xi = x[i];
            if (logMask & 0x1)
            {
                xi = DecompositionUtils::getLog10Value(x[i]);
            }
            buffer[bufferOffset] = (float)(xi * scale[0] + translation[0]);
        }

        if (coordinateMask & 0x2)
        {
            double yi = y[i];
            if (logMask & 0x2)
            {
                yi = DecompositionUtils::getLog10Value(y[i]);
            }
            buffer[bufferOffset + 1] = (float)(yi * scale[1] + translation[1]);
        }

        if (coordinateMask & 0x4)
        {
            double zi = z[i];
            if (logMask & 0x4)
            {
                zi = DecompositionUtils::getLog10Value(z[i]);
            }
            buffer[bufferOffset + 2] = (float)(zi * scale[2] + translation[2]);
        }

        if (elementsSize == 4 && (coordinateMask & 0x8))
        {
            buffer[bufferOffset + 3] = 1.0;
        }

        bufferOffset += 4;
    }

}

void Fac3DDecomposer::fillTextureCoordinates(int id, float* buffer, int bufferLength)
{
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;
    int parent = 0;
    int* pparent = &parent;

    double* colors = NULL;
    double* colormap = NULL;
    double* z = NULL;

    double color = 0.;

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


    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_COLORS__, jni_int, (void**) &piNumColors);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COLORS__, jni_double_vector, (void**) &colors);


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

    getGraphicObjectProperty(id, __GO_COLOR_FLAG__, jni_int, (void**) &piColorFlag);
    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    /* Do not fill */
    if (colorFlag == 0)
    {
        return;
    }

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    if (numColors == numGons * numVerticesPerGon)
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
        fillNormalizedZColorsTextureCoordinates(buffer, bufferLength, colormap, colormapSize, z, numGons, numVerticesPerGon);
    }
    else if (colorFlag > 1 && numColors == 0)
    {
        /*
         * The color buffer must be filled with the color_mode value.
         * To do: correctly take into account Nan and infinite values.
         */
        int colorMode = 0;
        int* piColorMode = &colorMode;

        getGraphicObjectProperty(id, __GO_COLOR_MODE__, jni_int, (void**) &piColorMode);

        color = (double) colorMode;
        color = DecompositionUtils::getAbsoluteValue(color);

        fillConstantColorsTextureCoordinates(buffer, bufferLength, colormap, colormapSize,
                                             color, numGons, numVerticesPerGon);
    }
    else
    {
        fillDataColorsTextureCoordinates(buffer, bufferLength, colormap, colormapSize,
                                         colors, colorFlag, perVertex, dataMapping, numGons, numVerticesPerGon);
    }

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

void Fac3DDecomposer::fillNormalizedZColorsTextureCoordinates(float* buffer, int bufferLength, double* colormap, int colormapSize,
        double* z, int numGons, int numVerticesPerGon)
{
    double zavg = 0.;
    double zMin = 0.;
    double zMax = 0.;
    double zRange = 0.;
    double minDoubleValue = 0.;
    float index = 0.;

    int i = 0;
    int j = 0;
    int bufferOffset = 0;

    computeMinMaxValues(z, numGons * numVerticesPerGon, numGons, numVerticesPerGon, ALL_VALUES, &zMin, &zMax);

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
        zavg = computeAverageValue(&z[i * numVerticesPerGon], numVerticesPerGon);
        index = (float)((ColorComputer::getIndex(zavg, zMin, zRange, Z_COLOR_OFFSET, 0, colormapSize - 1) + 2.0 + COLOR_TEXTURE_OFFSET) / (float) (colormapSize + 2));

        for (j = 0; j < numVerticesPerGon; j++)
        {
            buffer[bufferOffset++] = index;
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 1.0;
        }
    }

}

void Fac3DDecomposer::fillConstantColorsTextureCoordinates(float* buffer, int bufferLength, double* colormap, int colormapSize,
        double colorValue, int numGons, int numVerticesPerGon)
{
    int bufferOffset = 0;

    double index = (ColorComputer::getClampedDirectIndex(colorValue - 1.0, colormapSize) + 2.0 + COLOR_TEXTURE_OFFSET) / (float) (colormapSize + 2);

    for (int i = 0; i < numGons * numVerticesPerGon; i++)
    {
        buffer[bufferOffset++] = (float)index;
        buffer[bufferOffset++] = 0;
        buffer[bufferOffset++] = 0;
        buffer[bufferOffset++] = 1.0;
    }

}

void Fac3DDecomposer::fillDataColorsTextureCoordinates(float* buffer, int bufferLength, double* colormap, int colormapSize,
        double* colors, int colorFlag, int perVertex, int dataMapping, int numGons, int numVerticesPerGon)
{
    double colMin = 0.;
    double colRange = 0.;
    double color = 0.;
    double colorTextureOffset = 0.;
    double index = 0.;

    int bufferOffset = 0;

    int numColors = 0;

    Fac3DColorComputer colorComputer;

    if (perVertex)
    {
        numColors = numGons * numVerticesPerGon;
    }
    else
    {
        numColors = numGons;
    }

    colorComputer = Fac3DColorComputer(colors, numColors, colorFlag, dataMapping, numGons, numVerticesPerGon);

    /* 0: colors are scaled */
    if (dataMapping == 0)
    {
        colorComputer.getColorRangeValue(&colMin, &colRange);
    }

    /*
     * The color texture offset value is used to center color sub-intervals
     * on integer index values when interpolated shading is used or to be sure
     * to fetch the correct color value when flat shading is used.
     */
    colorTextureOffset = COLOR_TEXTURE_OFFSET;

    for (int i = 0; i < numGons; i++)
    {
        for (int j = 0; j < numVerticesPerGon; j++)
        {
            color = colorComputer.getOutputFacetColor(i, j);

            if (dataMapping == 1)
            {
                color = DecompositionUtils::getAbsoluteValue(color);
                index = ColorComputer::getClampedDirectIndex(color - 1.0 , colormapSize);
            }
            else if (dataMapping == 0)
            {
                index = ColorComputer::getIndex(color, colMin, colRange, COLOR_OFFSET, 0, colormapSize - 1);
            }

            /* The offset corresponding to the black and white colors must added to the index and the colormap size. */
            buffer[bufferOffset++] = (float) ((index + colorTextureOffset + 2.0) / (double) (colormapSize + 2));
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 0;
            buffer[bufferOffset++] = 1.0;
        }
    }
}

double Fac3DDecomposer::computeAverageValue(double* values, int numVertices)
{
    double averageValue = 0.0;

    for (int i = 0; i < numVertices; i++)
    {
        averageValue += values[i];
    }

    averageValue /= (double) numVertices;

    return averageValue;
}

void Fac3DDecomposer::computeMinMaxValues(double* values, int numValues, int numGons, int numVerticesPerGon, int minMaxComputation,
        double* valueMin, double* valueMax)
{
    double maxDouble = DecompositionUtils::getMaxDoubleValue();
    double tmpValueMin = maxDouble;
    double tmpValueMax = -maxDouble;
    double value = 0.;

    int numIterations = 0;

    if (minMaxComputation != ALL_VALUES)
    {
        numIterations = numGons;
    }
    else
    {
        numIterations = numValues;
    }

    for (int i = 0; i < numIterations; i++)
    {
        if (minMaxComputation == FIRST_VERTEX_VALUE)
        {
            value = values[i * numVerticesPerGon];
        }
        else if (minMaxComputation == FACE_AVERAGE)
        {
            value = computeAverageValue(&values[i * numVerticesPerGon], numVerticesPerGon);
        }
        else
        {
            value = values[i];
        }

        if (DecompositionUtils::isValid(value))
        {
            if (value < tmpValueMin)
            {
                tmpValueMin = value;
            }

            if (value > tmpValueMax)
            {
                tmpValueMax = value;
            }
        }
    }

    *valueMin = tmpValueMin;
    *valueMax = tmpValueMax;
}

int Fac3DDecomposer::getIndicesSize(int id)
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

    return 3 * (numVerticesPerGon - 2) * numGons;
}

/*
 * To do: use a polygon triangulation algorithm, as the fan decomposition used may produce
 * overlapping triangles for non-convex polygons.
 */
int Fac3DDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    double xc = 0.;
    double yc = 0.;
    double zc = 0.;

    double* colors = NULL;

    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    int numColors = 0;
    int* piNumColors = &numColors;

    int colorFlag = 0;
    int* piColorFlag = &colorFlag;

    int dataMapping =  0;
    int* piDataMapping = &dataMapping;

    int bufferOffset = 0;
    int vertexOffset = 0;

    Fac3DColorComputer colorComputer;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_COLORS__, jni_int, (void**) &piNumColors);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_COLORS__, jni_double_vector, (void**) &colors);

    getGraphicObjectProperty(id, __GO_COLOR_FLAG__, jni_int, (void**) &piColorFlag);

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    /* At least 3 vertices per N-gon are required to output triangles. */
    if (numVerticesPerGon < 3)
    {
        return 0;
    }

    colorComputer = Fac3DColorComputer(colors, numColors, colorFlag, dataMapping, numGons, numVerticesPerGon);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    for (int i = 0; i < numGons; i++)
    {
        int isValid = 1;
        Triangulator triangulator;

        for (int j = 0; j < numVerticesPerGon; j++)
        {
            xc = x[vertexOffset + j];
            yc = y[vertexOffset + j];
            zc = z[vertexOffset + j];

            if (!DecompositionUtils::isValid(xc, yc, zc) || !DecompositionUtils::isLogValid(xc, yc, zc, logMask))
            {
                isValid = 0;
                break;
            }
            triangulator.addPoint(xc, yc, zc);
        }

        if (isValid == 0 || colorComputer.isFacetColorValid(i) == 0)
        {
            vertexOffset += numVerticesPerGon;
            continue;
        }

        triangulator.initialize();
        triangulator.triangulate();
        int numTriangles = triangulator.getNumberTriangles();

        //Failed to triangulate, should not happen, unless the points are colinear
        if (numTriangles < 1)
        {
            vertexOffset += numVerticesPerGon;
            continue;
        }

        int * indices = triangulator.getIndices();


        for (int j = 0; j < numTriangles; j++)
        {
            buffer[bufferOffset] = vertexOffset + indices[3 * j];
            buffer[bufferOffset + 1] = vertexOffset + indices[3 * j + 1];
            buffer[bufferOffset + 2] = vertexOffset + indices[3 * j + 2];

            bufferOffset += 3;
        }
        triangulator.clear();

        vertexOffset += numVerticesPerGon;
    }

    return bufferOffset;
}

int Fac3DDecomposer::getWireIndicesSize(int id)
{
    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**) &piNumGons);

    return 2 * numVerticesPerGon * numGons;
}

int Fac3DDecomposer::fillWireIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;

    double xc = 0.;
    double yc = 0.;
    double zc = 0.;

    int numVerticesPerGon = 0;
    int* piNumVerticesPerGon = &numVerticesPerGon;
    int numGons = 0;
    int* piNumGons = &numGons;

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

    for (int i = 0; i < numGons; i++)
    {
        int isValid = 1;

        for (int j = 0; j < numVerticesPerGon; j++)
        {
            xc = x[vertexOffset + j];
            yc = y[vertexOffset + j];
            zc = z[vertexOffset + j];

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

        for (int j = 0; j < numVerticesPerGon; j++)
        {
            buffer[bufferOffset] = vertexOffset + j;
            buffer[bufferOffset + 1] = vertexOffset + (j + 1) % numVerticesPerGon;

            bufferOffset += 2;
        }

        vertexOffset += numVerticesPerGon;
    }

    return bufferOffset;
}

