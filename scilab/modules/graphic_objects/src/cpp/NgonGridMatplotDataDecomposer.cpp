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
#include "NgonGridMatplotDataDecomposer.hxx"

#include <iostream>

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

NgonGridMatplotDataDecomposer* NgonGridMatplotDataDecomposer::decomposer = NULL;

void NgonGridMatplotDataDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
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
    bufferOffset = 0;

    for (int j = 0; j < numY - 1; j++)
    {
        double ycoords[4];
        int yindices[4];

        yj = (double) j * y[1] + y[0];
        yjp1 = (double) (j + 1) * y[1] + y[0];

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

            xi = (double) i * x[1] + x[0];
            xip1 = (double) (i + 1) * x[1] + x[0];

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

                /*if (coordinateMask & 0x4)
                    zij = getZCoordinate(z, numX, numY, xindices[k], yindices[k], logMask & 0x4);
                    buffer[bufferOffset +2] = (float)(zij * scale[2] + translation[2]);
                }*/

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

/*
 * To do:
 * -clean-up: replace explicitely computed z indices by getPointIndex calls
 * -remove the per-vertex color fill code
 */
void NgonGridMatplotDataDecomposer::fillColors(int id, float* buffer, int bufferLength, int elementsSize)
{
    int parent = 0;
    int* pparent = &parent;
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;

    void * data = NULL;
    double* colormap = NULL;
    double currentZ = 0.;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    int datatype = -1;
    int * pidataType = &datatype;
    int imagetype = 0;
    int * piImagetype = &imagetype;
    int gltype = 0;
    int * piGltype = &gltype;

    parent = getParentObject(id);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parent == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &pparentFigure);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, jni_double_vector, &data);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int, (void**) &pidataType);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int, (void **)&piImagetype);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_GL_TYPE__, jni_int, (void **)&piGltype);

    /* In order not to access invalid data when reading the last line's last element */
    if (numX < 2)
    {
        return;
    }

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

#if PER_VERTEX_VALUES
    for (int j = 0; j < numY - 1; j++)
    {
        for (int i = 0; i < numX - 1; i++)
        {
            currentZ = z[i * (numY - 1) + (numY - 2 - j)];

            ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset + 3] = 1.0;
            }

            bufferOffset += elementsSize;
        }

        /* Last element (same as the above loop's last) */
        currentZ  = z[(numX - 2) * (numY - 1) + (numY - 2 - j)];

        ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

        if (elementsSize == 4)
        {
            buffer[bufferOffset + 3] = 1.0;
        }

        bufferOffset += elementsSize;
    }

    /* Last line */
    for (int i = 0; i < numX - 1; i++)
    {
        currentZ = z[(numY - 1) * i + 0];

        ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

        bufferOffset += elementsSize;

        if (elementsSize == 4)
        {
            buffer[bufferOffset + 3] = 1.0;
        }
    }

    /* Last element (same as the above loop's last) */
    currentZ  = z[(numX - 2) * (numY - 1) + 0];

    ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

    if (elementsSize == 4)
    {
        buffer[bufferOffset + 3] = 1.0;
    }

#else

    getRGBAData((ImageType)imagetype, (DataType)datatype, (GLType)gltype, data, buffer, elementsSize, numY - 1, numX - 1, colormap, colormapSize);

    /*for (int j = 0; j < numY-1; j++)
    {
        for (int i = 0; i < numX - 1; i++)
        {
            float facetColor[3];

            currentZ = z[i * (numY - 1) + (numY - 2 - j)];
            ColorComputer::getDirectColor((double) currentZ - 1.0, colormap, colormapSize, facetColor);
            writeFacetColorToBuffer(buffer, bufferOffset, facetColor, elementsSize);

            bufferOffset += 4 * elementsSize;
        }
    }*/

#endif

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

/*
 * To do: merge with NgonGridDataDecomposer::fillIndices as these functions perform a lot of work in
 * a redundant way.
 */
int NgonGridMatplotDataDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
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
    int facetValid = 1;

    /*zij = getZCoordinate(z, numX, numY, i, j, logUsed);

    facetValid = DecompositionUtils::isValid(zij);

    if (logUsed)
    {
        facetValid &= DecompositionUtils::isLogValid(zij);
    }*/

    /* Transposed relative to Grayplot */
    //facetValid &= DecompositionUtils::isValid(values[getPointIndex(numY-1, numX-1, numY-2-j, i)]);

    /* Edge validity is always 1 since it is not used at all to determine facet validity for Matplot decomposition */
    *nextEdgeValid = 1;

    return facetValid;
}

int NgonGridMatplotDataDecomposer::isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed)
{
    /* Always considered valid since not used at all to determine facet validity */
    return 1;
}

void NgonGridMatplotDataDecomposer::getRGBAData(ImageType imagetype, DataType datatype, GLType gltype, void * data, float * buffer, int elementsSize, const int nbRow, const int nbCol, double * colormap, const int colormapSize)
{
    if (imagetype == MATPLOT_INDEX)
    {
        switch (datatype)
        {
            case MATPLOT_Char :
            {
                fillColorsByIndex((char *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            case MATPLOT_UChar :
            {
                fillColorsByIndex((unsigned char *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            case MATPLOT_Int :
            {
                fillColorsByIndex((int *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            case MATPLOT_UInt :
            {
                fillColorsByIndex((unsigned int *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            case MATPLOT_Short :
            {
                fillColorsByIndex((short *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            case MATPLOT_UShort :
            {
                fillColorsByIndex((unsigned short *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            case MATPLOT_Double :
            {
                fillColorsByIndex((double *)data, buffer, elementsSize, nbRow, nbCol, colormap, colormapSize);
                break;
            }
            default:
                break;
        }
    }
    else
    {
        float colors[4];

        switch (gltype)
        {
            case MATPLOT_GL_RGBA :
            {
                unsigned int k = 0;
                unsigned int * ui = (unsigned int *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        const unsigned int c = ui[nbRow - 1 + i * nbRow - j];
                        colors[0] = ((c >> 24) & 0xFF) / 255.0f;
                        colors[1] = ((c >> 16) & 0xFF) / 255.0f;
                        colors[2] = ((c >> 8) & 0xFF) / 255.0f;
                        colors[3] = (c & 0xFF) / 255.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, true);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGB_FLOAT :
            {
                unsigned int k = 0;
                float * f = (float *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        writeFacetColorToBuffer(buffer, k, &f[3 * (nbRow - 1 + i * nbRow - j)], elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGB :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        const unsigned int ind = 3 * (nbRow - 1 + i * nbRow - j);
                        colors[0] = uc[ind] / 255.0f;
                        colors[1] = uc[ind + 1] / 255.0f;
                        colors[2] = uc[ind + 2] / 255.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGBA_BYTE :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        const unsigned int ind = 4 * (nbRow - 1 + i * nbRow - j);
                        colors[0] = uc[ind] / 255.0f;
                        colors[1] = uc[ind + 1] / 255.0f;
                        colors[2] = uc[ind + 2] / 255.0f;
                        colors[3] = uc[ind + 3] / 255.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, true);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGBA_FLOAT :
            {
                unsigned int k = 0;
                float * f = (float *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        writeFacetColorToBuffer(buffer, k, &f[4 * (nbRow - 1 + i * nbRow - j)], elementsSize, true);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_GRAY :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = uc[nbRow - 1 + i * nbRow - j] / 255.0f;
                        colors[1] = colors[0];
                        colors[2] = colors[0];
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RED :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = uc[nbRow - 1 + i * nbRow - j] / 255.0f;
                        colors[1] = 0;
                        colors[2] = 0;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_GREEN :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = 0;
                        colors[1] = uc[nbRow - 1 + i * nbRow - j] / 255.0f;
                        colors[2] = 0;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_BLUE :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = 0;
                        colors[1] = 0;
                        colors[2] = uc[nbRow - 1 + i * nbRow - j] / 255.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_GRAY_FLOAT :
            {
                unsigned int k = 0;
                float * f = (float *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = f[nbRow - 1 + i * nbRow - j];
                        colors[1] = colors[0];
                        colors[2] = colors[0];
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RED_FLOAT :
            {
                unsigned int k = 0;
                float * f = (float *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = f[nbRow - 1 + i * nbRow - j];
                        colors[1] = 0;
                        colors[2] = 0;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_GREEN_FLOAT :
            {
                unsigned int k = 0;
                float * f = (float *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = 0;
                        colors[1] = f[nbRow - 1 + i * nbRow - j];
                        colors[2] = 0;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_BLUE_FLOAT :
            {
                unsigned int k = 0;
                float * f = (float *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = 0;
                        colors[1] = 0;
                        colors[2] = f[nbRow - 1 + i * nbRow - j];
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGBA_4444 :
            {
                unsigned int k = 0;
                unsigned short * us = (unsigned short *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        const unsigned short c = us[nbRow - 1 + i * nbRow - j];
                        colors[0] = ((c >> 12) & 0xFF) / 15.0f;
                        colors[1] = ((c >> 8) & 0xFF) / 15.0f;
                        colors[2] = ((c >> 4) & 0xFF) / 15.0f;
                        colors[3] = (c & 0xFF) / 15.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, true);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGBA_5551 :
            {
                unsigned int k = 0;
                unsigned short * us = (unsigned short *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        const unsigned short c = us[nbRow - 1 + i * nbRow - j];
                        colors[0] = ((c >> 11) & 0xFF) / 31.0f;
                        colors[1] = ((c >> 6) & 0xFF) / 31.0f;
                        colors[2] = ((c >> 1) & 0xFF) / 31.0f;
                        colors[3] = (float)(c & 0x1);
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, true);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RGB_332 :
            {
                unsigned int k = 0;
                unsigned char * uc = (unsigned char *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        const unsigned char c = uc[nbRow - 1 + i * nbRow - j];
                        colors[0] = ((c >> 5) & 0x7) / 7.0f;
                        colors[1] = ((c >> 2) & 0x7) / 7.0f;
                        colors[2] = (c & 0x3) / 3.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_GRAY_16 :
            {
                unsigned int k = 0;
                unsigned short * us = (unsigned short *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = us[nbRow - 1 + i * nbRow - j] / 65535.0f;
                        colors[1] = colors[0];
                        colors[2] = colors[0];
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_RED_16 :
            {
                unsigned int k = 0;
                unsigned short * us = (unsigned short *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = us[nbRow - 1 + i * nbRow - j] / 65535.0f;
                        colors[1] = 0;
                        colors[2] = 0;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_GREEN_16 :
            {
                unsigned int k = 0;
                unsigned short * us = (unsigned short *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = 0;
                        colors[1] = us[nbRow - 1 + i * nbRow - j] / 65535.0f;
                        colors[2] = 0;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            case MATPLOT_GL_BLUE_16 :
            {
                unsigned int k = 0;
                unsigned short * us = (unsigned short *)data;

                for (int j = 0; j < nbRow; j++)
                {
                    for (int i = 0; i < nbCol; i++)
                    {
                        colors[0] = 0;
                        colors[1] = 0;
                        colors[2] = us[nbRow - 1 + i * nbRow - j] / 65535.0f;
                        writeFacetColorToBuffer(buffer, k, colors, elementsSize, false);
                        k += 4 * elementsSize;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}
