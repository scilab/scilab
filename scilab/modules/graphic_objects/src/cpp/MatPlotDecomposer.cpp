/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DecompositionUtils.hxx"
#include "MatPlotDecomposer.hxx"
#include "DataProperties.hxx"
#include "ColorComputer.hxx"
#include <iostream>

extern "C" {
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int MatPlotDecomposer::getTextureWidth(char* id)
{
  int width = 0;
  int* piWidth = &width;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piWidth);
  return width - 1;
}


int MatPlotDecomposer::getTextureHeight(char* id)
{
  int height = 0;
  int* piHeight = &height;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piHeight);
  return height - 1;
}


int MatPlotDecomposer::fillTextureData(char* id, unsigned char* buffer, int bufferLength)
{
  double* value = NULL;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &value);
  int textureWidth = getTextureWidth(id);
  int textureHeight = getTextureHeight(id);
  int dataSize = textureWidth * textureHeight;
  if (dataSize * 4 == bufferLength)
  {

    char* parentFigure = NULL;
    double* colormap = NULL;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    for (int i = 0 ; i < textureWidth ; i++)
    {
      for (int j = 0 ; j < textureHeight ; j++)
      {
        ColorComputer::getDirectByteColor(value[j + i * textureHeight] - 1, colormap, colormapSize, &buffer[4 * (i + j * textureWidth)]);
        buffer[4 * (i + j * textureWidth) + 3] = 255;
      }
    }

    return bufferLength;
  }
  else
  {
    return 0;
  }
}


int MatPlotDecomposer::fillTextureData(char* id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height)
{
  double* value = NULL;
  getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &value);
  if (width * height * 4 == bufferLength)
  {
    char* parentFigure = NULL;
    double* colormap = NULL;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);
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
    return bufferLength;
  }
  else
  {
    return 0;
  }
}

