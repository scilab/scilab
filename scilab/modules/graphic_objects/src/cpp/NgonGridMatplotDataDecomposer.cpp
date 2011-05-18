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
#include "NgonGridMatplotDataDecomposer.hxx"

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

/*
 * To do:
 * -per-facet colors
 * -clean-up (replace explicitely computed z indices by getPointIndex calls)
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

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int i;
    int j;
    int bufferOffset = 0;

    int currentRowValid;
    int nextRowValid;

    int currentColumnValid;
    int nextColumnValid;

    int ij;
    int ip1j;
    int ip1jp1;
    int ijp1;

    int facetValid;

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

    /* First line */
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

        for (i = 0; i < numX-1; i++)
        {
            nextColumnValid = DecompositionUtils::isValid(x[i+1]);

            if (logMask & 0x1)
            {
                nextColumnValid &= DecompositionUtils::isLogValid(x[i+1]);
            }

            ip1j = getPointIndex(numX, numY, i+1, j);
            ip1jp1 = getPointIndex(numX, numY, i+1, j+1);

            /* Transposed relative to Grayplot */
            facetValid = DecompositionUtils::isValid(z[getPointIndex(numY-1, numX-1, numY-2-j, i)]);

            if (currentColumnValid && nextColumnValid && facetValid)
            {
                /* All facets are decomposed the same way */
                buffer[bufferOffset] = ij;
                buffer[bufferOffset+1] = ip1j;
                buffer[bufferOffset+2] = ip1jp1;
                buffer[bufferOffset+3] = ij;
                buffer[bufferOffset+4] = ip1jp1;
                buffer[bufferOffset+5] = ijp1;

                bufferOffset += 6;
            }

            currentColumnValid = nextColumnValid;

            ij = ip1j;
            ijp1 = ip1jp1;
        }
    }

    return bufferOffset;
}

