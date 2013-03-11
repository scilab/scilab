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

void NgonGridMatplotDataDecomposer::fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* matplotScale = NULL;
    double* matplotTranslate = NULL;
    double zShift = 0.;
    double* pdZShift = &zShift;

    double xTrans[2];
    double yTrans[2];

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    NgonGridMatplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_MATPLOT_SCALE__, jni_double_vector, (void**) &matplotScale);
    getGraphicObjectProperty(id, __GO_MATPLOT_TRANSLATE__, jni_double_vector, (void**) &matplotTranslate);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double, (void**) &pdZShift);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* The position of the lower-left corner and the distance between two adjacent vertices i and i+1 (respectively) along the x-axis */
    xTrans[0] = matplotTranslate[0];
    xTrans[1] = matplotScale[0];

    /* The position of the lower-left corner and the distance between two adjacent vertices j and j+1 (respectively) along the y-axis */
    yTrans[0] = matplotTranslate[1];
    yTrans[1] = matplotScale[1];

    /*
     * We pass the scale and translate values (for both the x and y axes) as the x and y coordinate arrays,
     * because Matplot vertex coordinates are directly computed from these values.
     */
    decomposer->fillGridVertices(buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, (double*) xTrans, (double*) yTrans, &zShift, numX, numY);

    releaseGraphicObjectProperty(__GO_MATPLOT_SCALE__, matplotScale, jni_double_vector, 0);
    releaseGraphicObjectProperty(__GO_MATPLOT_TRANSLATE__, matplotTranslate, jni_double_vector, 0);
}

/* To do: refactor with its parent class' same method */
void NgonGridMatplotDataDecomposer::fillGridVertices(float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask,
                                                     double* x, double* y, double* z, int numX, int numY)
{
    double xi = 0.;
    double yj = 0.;
    double zij = 0.;
    double yjp1 = 0.;
    double xip1 = 0.;

    int bufferOffset = 0;

#if PER_VERTEX_VALUES
    for (int j = 0; j < numY; j++)
    {
        yj = (double) j * y[1] + y[0];

        if (coordinateMask  & 0x2)
        {
            if (logMask & 0x2)
            {
                yj = DecompositionUtils::getLog10Value(yj);
            }
        }

        for (int i = 0; i < numX; i++)
        {
            xi = (double) i * x[1] + x[0];
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
#else
    bufferOffset = 0;

    for (int j = 0; j < numY-1; j++)
    {
        double ycoords[4];
        int yindices[4];

        yj = (double) j * y[1] + y[0];
        yjp1 = (double) (j+1) * y[1] + y[0];

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
        yindices[2] = j+1;
        yindices[3] = j+1;

        for (int i = 0; i < numX-1; i++)
        {
            double xcoords[4];
            int xindices[4];

            xi = (double) i * x[1] + x[0];
            xip1 = (double) (i+1) * x[1] + x[0];

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
            xindices[1] = i+1;
            xindices[2] = i;
            xindices[3] = i+1;

            for (int k = 0; k < 4; k++)
            {
                if (coordinateMask & 0x1)
                {
                    buffer[bufferOffset] = (float)(xcoords[k] * scale[0] + translation[0]);
                }

                if (coordinateMask  & 0x2)
                {
                    buffer[bufferOffset +1] = (float)(ycoords[k] * scale[1] + translation[1]);
                }

                if (coordinateMask & 0x4)
                {
                    zij = getZCoordinate(z, numX, numY, xindices[k], yindices[k], logMask & 0x4);
                    buffer[bufferOffset +2] = (float)(zij * scale[2] + translation[2]);
                }

                if (elementsSize == 4 && (coordinateMask & 0x8))
                {
                    buffer[bufferOffset +3] = 1.0;
                }

                bufferOffset += elementsSize;
            }

        }
    }

#endif
}

/*
 * To do:
 * -clean-up: replace explicitely computed z indices by getPointIndex calls
 * -remove the per-vertex color fill code
 */
void NgonGridMatplotDataDecomposer::fillColors(char* id, float* buffer, int bufferLength, int elementsSize)
{
    char* parent = NULL;
    char* parentFigure = NULL;

    double* z = NULL;
    double* colormap = NULL;
    double currentZ = 0.;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_PARENT__, jni_string, (void**) &parent);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (strcmp(parent, "") == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    /* In order not to access invalid data when reading the last line's last element */
    if (numX < 2)
    {
        return;
    }

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

#if PER_VERTEX_VALUES
    for (int j = 0; j < numY-1; j++)
    {
        for (int i = 0; i < numX-1; i++)
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
    for (int i = 0; i < numX-1; i++)
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

    for (int j = 0; j < numY-1; j++)
    {
        for (int i = 0; i < numX-1; i++)
        {
            float facetColor[3];

            currentZ = z[i*(numY-1)+(numY-2-j)];

            ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, facetColor);

            writeFacetColorToBuffer(buffer, bufferOffset, facetColor, elementsSize);

            bufferOffset += 4*elementsSize;
        }
    }

#endif

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

/*
 * To do: merge with NgonGridDataDecomposer::fillIndices as these functions perform a lot of work in
 * a redundant way.
 */
int NgonGridMatplotDataDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
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

    /*
     * The per-node value flag is set to 0 as for Matplot objects grid values are specified per-facet.
     * However, it not used at all when determining facet validity and hence is only informative.
     */
    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, &zShift, z, 0, numX, numY);

    return numberIndices;
}

int NgonGridMatplotDataDecomposer::isFacetValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid)
{
    double zij = 0.;
    int facetValid = 0;

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

int NgonGridMatplotDataDecomposer::isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed)
{
    /* Always considered valid since not used at all to determine facet validity */
    return 1;
}

