/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <cstring>

#include "DataLoader.hxx"

#include "MatPlotDecomposer.hxx"
#include "Fac3DDecomposer.hxx"
#include "NgonGridGrayplotDataDecomposer.hxx"
#include "NgonGridMatplotDataDecomposer.hxx"
#include "Plot3DDecomposer.hxx"
#include "PolylineDecomposer.hxx"
#include "TriangleMeshFecDataDecomposer.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

// TODO: switch using the Type


int getTextureWidth(char* id)
{
  char* type = NULL;
  getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);
  if (strcmp(type, __GO_MATPLOT__) == 0)
  {
    return MatPlotDecomposer::getTextureWidth(id);
  }
  else
  {
    return 0;
  }
}

int getTextureHeight(char* id)
{
  char* type = NULL;
  getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);
  if (strcmp(type, __GO_MATPLOT__) == 0)
  {
    return MatPlotDecomposer::getTextureHeight(id);
  }
  else
  {
    return 0;
  }
}

int fillTextureData(char* id, unsigned char* buffer, int bufferLength)
{
  char* type = NULL;
  getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);
  if (strcmp(type, __GO_MATPLOT__) == 0)
  {
    return MatPlotDecomposer::fillTextureData(id, buffer, bufferLength);
  }
  else
  {
    return 0;
  }
}

int fillSubTextureData(char* id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height)
{
  char* type = NULL;
  getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);
  if (strcmp(type, __GO_MATPLOT__) == 0)
  {
    return MatPlotDecomposer::fillTextureData(id, buffer, bufferLength, x, y, width, height);
  }
  else
  {
    return 0;
  }
}

int getDataSize(char* id)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        return Fac3DDecomposer::getDataSize(id);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        return TriangleMeshFecDataDecomposer::getDataSize(id);
    }
    else if (strcmp(type, __GO_GRAYPLOT__) == 0)
    {
        return NgonGridGrayplotDataDecomposer::getDataSize(id);
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
    {
        return NgonGridMatplotDataDecomposer::getDataSize(id);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        return Plot3DDecomposer::getDataSize(id);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::getDataSize(id);
    }
    else
    {
        return 0;
    }

}

void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        Fac3DDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        TriangleMeshFecDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
    }
    else if (strcmp(type, __GO_GRAYPLOT__) == 0)
    {
        NgonGridGrayplotDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
    {
        NgonGridMatplotDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        Plot3DDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        PolylineDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
    }
}

void fillTextureCoordinates(char* id, float* BUFF, int bufferLength)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        Fac3DDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        TriangleMeshFecDataDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        PolylineDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
    }
}

void fillColors(char* id, float* BUFF, int bufferLength, int elementsSize)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FEC__) == 0)
    {
        TriangleMeshFecDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
    }
    else if (strcmp(type, __GO_GRAYPLOT__) == 0)
    {
        NgonGridGrayplotDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
    {
        NgonGridMatplotDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        Plot3DDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        PolylineDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
    }
}


int getIndicesSize(char* id)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        return Fac3DDecomposer::getIndicesSize(id);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        return TriangleMeshFecDataDecomposer::getIndicesSize(id);
    }
    else if (strcmp(type, __GO_GRAYPLOT__) == 0)
    {
        return NgonGridGrayplotDataDecomposer::getIndicesSize(id);
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
    {
        return NgonGridMatplotDataDecomposer::getIndicesSize(id);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        return Plot3DDecomposer::getIndicesSize(id);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::getIndicesSize(id);
    }

    return 0;
}


int fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        return Fac3DDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        return TriangleMeshFecDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_GRAYPLOT__) == 0)
    {
        return NgonGridGrayplotDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
    {
        return NgonGridMatplotDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        return Plot3DDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    }

    return 0;
}

int getWireIndicesSize(char* id)
{
    char* type = NULL;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        return Fac3DDecomposer::getWireIndicesSize(id);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        return TriangleMeshFecDataDecomposer::getWireIndicesSize(id);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        return Plot3DDecomposer::getWireIndicesSize(id);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::getWireIndicesSize(id);
    }

    return 0;
}

int fillWireIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    char* type = NULL;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_FAC3D__) == 0)
    {
        return Fac3DDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        return TriangleMeshFecDataDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        return Plot3DDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    }

    return 0;
}

int getMarkIndicesSize(char* id)
{
    return 0;
}

int fillMarkIndices(char* id, int* BUFF, int bufferLength)
{
    // TODO.
    return 0;
}
