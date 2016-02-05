/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
#include "MatPlotDecomposer.hxx"
#include "DataProperties.hxx"
#include "ColorComputer.hxx"
#include <iostream>
#include <cstring>

extern "C" {
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "Matplot.h"
}

int MatPlotDecomposer::getTextureWidth(int id)
{
    int width = 0;
    int* piWidth = &width;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piWidth);

    return width - 1;
}


int MatPlotDecomposer::getTextureHeight(int id)
{
    int height = 0;
    int* piHeight = &height;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piHeight);

    return height - 1;
}


int MatPlotDecomposer::fillTextureData(int id, unsigned char* buffer, int bufferLength)
{
    // Indexed colors
    void * data = NULL;
    int parentFigure = 0;
    int * piParentFigure = &parentFigure;
    double * colormap = NULL;
    int colormapSize = 0;
    int * piColormapSize = &colormapSize;
    int datatype = -1;
    int * pidataType = &datatype;
    const int h = getTextureHeight(id);
    const int w = getTextureWidth(id);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, &data);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int, (void**) &pidataType);
    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &piParentFigure);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    switch ((DataType)datatype)
    {
        case MATPLOT_HM1_Char :
        case MATPLOT_Char :
        {
            char * index = (char *)data;
            for (int i = 0 ; i < w * h; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }

            break;
        }
        case MATPLOT_HM1_UChar :
        case MATPLOT_UChar :
        {
            unsigned char * index = (unsigned char *)data;
            for (int i = 0 ; i < w * h; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }

            break;
        }
        case MATPLOT_Int :
        {
            int * index = (int *)data;
            for (int i = 0 ; i < w * h; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }

            break;
        }
        case MATPLOT_UInt :
        {
            unsigned int * index = (unsigned int *)data;
            for (int i = 0 ; i < w * h; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }

            break;
        }
        case MATPLOT_Short :
        {
            short * index = (short *)data;
            for (int i = 0 ; i < w * h; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }

            break;
        }
        case MATPLOT_UShort :
        {
            unsigned short * index = (unsigned short *)data;
            for (int i = 0 ; i < w * h; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }

            break;
        }
        case MATPLOT_HM1_Double :
        case MATPLOT_Double :
        {
            double * index = (double *)data;
            for (int i = 0 ; i < w * h ; i++)
            {
                ColorComputer::getDirectByteColor(index[i] - 1, colormap, colormapSize, &buffer[4 * i], false);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);

    return bufferLength;
}


int MatPlotDecomposer::fillTextureData(int id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height)
{
    double* value = NULL;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &value);
    if (width * height * 4 == bufferLength)
    {
        int parentFigure = 0;
        int * piParentFigure = &parentFigure;
        double* colormap = NULL;
        int colormapSize = 0;
        int* piColormapSize = &colormapSize;
        getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &piParentFigure);
        getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
        getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);
        int textureHeight = getTextureHeight(id);
        int k = 0;
        for (int j = y ; j < y + height ; j++)
        {
            for (int i = x ; i < x + width ; i++)
            {
                ColorComputer::getDirectByteColor(value[j + i * textureHeight] - 1, colormap, colormapSize, &buffer[k]);
                buffer[k + 3] = 255;
                k += 4;
            }
        }

        releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);

        return bufferLength;
    }
    else
    {
        return 0;
    }
}

int MatPlotDecomposer::getTextureImageType(int id)
{
    int type = -1;
    int * piType = &type;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int, (void**) &piType);

    return type;
}

int MatPlotDecomposer::getTextureDataType(int id)
{
    int type = -1;
    int * piType = &type;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int, (void**) &piType);

    return type;
}

int MatPlotDecomposer::getTextureGLType(int id)
{
    int type = -1;
    int * piType = &type;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_GL_TYPE__, jni_int, (void**) &piType);

    return type;
}

int MatPlotDecomposer::getTextureData(int id, void ** address, unsigned int * size)
{
    int type = getTextureImageType(id);
    if (type == MATPLOT_INDEX)
    {
        // Indexed colors
        const int h = getTextureHeight(id);
        const int w = getTextureWidth(id);
        const int bsize = w * h * sizeof(int);
        unsigned char * buffer = new unsigned char[bsize];
        fillTextureData(id, buffer, bsize);

        *address = buffer;
        *size = bsize;

        return 1;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, jni_double_vector, address);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_IMAGE_DATASIZE__, jni_int, (void **)&size);

    return 1;
}

void MatPlotDecomposer::disposeTextureData(int id, unsigned char * buffer)
{
    if (buffer)
    {
        delete[] buffer;
    }
}

int MatPlotDecomposer::isTextureRowOrder(int id)
{
    int rowOrder;
    int * piRowOrder = &rowOrder;
    getGraphicObjectProperty(id, __GO_DATA_MODEL_MATPLOT_DATA_ORDER__, jni_int, (void**) &piRowOrder);

    return rowOrder;
}

int buildMatplotType(DataType datatype, DataOrder dataorder, ImageType imagetype)
{
    return ((int)datatype) | ((int)dataorder << 8) | ((int)imagetype << 16);
}
