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

int getTextureWidth(char* id)
{
    int iType = 0;
    int *piType = &iType;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);
    if (iType == __GO_MATPLOT__)
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
    int iType = 0;
    int *piType = &iType;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);
    if (iType == __GO_MATPLOT__)
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
    int iType = 0;
    int *piType = &iType;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);
    if (iType == __GO_MATPLOT__)
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
    int iType = 0;
    int *piType = &iType;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);
    if (iType == __GO_MATPLOT__)
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
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        return Fac3DDecomposer::getDataSize(id);
    case __GO_FEC__ :
        return TriangleMeshFecDataDecomposer::getDataSize(id);
    case __GO_GRAYPLOT__ :
        return NgonGridGrayplotDataDecomposer::getDataSize(id);
    case __GO_MATPLOT__ :
        return NgonGridMatplotDataDecomposer::getDataSize(id);
    case __GO_PLOT3D__ :
        return Plot3DDecomposer::getDataSize(id);
    case __GO_POLYLINE__ :
        return PolylineDecomposer::getDataSize(id);
    default :
        return 0;
    }

}

void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        Fac3DDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        break;
    case __GO_FEC__ :
        TriangleMeshFecDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        break;
    case __GO_GRAYPLOT__ :
        NgonGridGrayplotDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        break;
    case __GO_MATPLOT__ :
        NgonGridMatplotDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        break;
    case __GO_PLOT3D__ :
        Plot3DDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        break;
    case __GO_POLYLINE__ :
        PolylineDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
        break;
    }
}

void fillTextureCoordinates(char* id, float* BUFF, int bufferLength)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        Fac3DDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
        break;
    case __GO_FEC__ :
        TriangleMeshFecDataDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
        break;
    case __GO_POLYLINE__ :
        PolylineDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
        break;
    }
}

void fillColors(char* id, float* BUFF, int bufferLength, int elementsSize)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FEC__ :
        TriangleMeshFecDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
        break;
    case __GO_GRAYPLOT__ :
        NgonGridGrayplotDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
        break;
    case __GO_MATPLOT__ :
        NgonGridMatplotDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
        break;
    case __GO_PLOT3D__ :
        Plot3DDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
        break;
    case __GO_POLYLINE__ :
        PolylineDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
        break;
    }
}


int getIndicesSize(char* id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        return Fac3DDecomposer::getIndicesSize(id);
    case __GO_FEC__ :
        return TriangleMeshFecDataDecomposer::getIndicesSize(id);
    case __GO_GRAYPLOT__ :
        return NgonGridGrayplotDataDecomposer::getIndicesSize(id);
    case __GO_MATPLOT__ :
        return NgonGridMatplotDataDecomposer::getIndicesSize(id);
    case __GO_PLOT3D__ :
        return Plot3DDecomposer::getIndicesSize(id);
    case __GO_POLYLINE__ :
        return PolylineDecomposer::getIndicesSize(id);
    default :
        return 0;
    }
}


int fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        return Fac3DDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    case __GO_FEC__ :
        return TriangleMeshFecDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    case __GO_GRAYPLOT__ :
        return NgonGridGrayplotDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    case __GO_MATPLOT__ :
        return NgonGridMatplotDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    case __GO_PLOT3D__ :
        return Plot3DDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    case __GO_POLYLINE__ :
        return PolylineDecomposer::fillIndices(id, buffer, bufferLength, logMask);
    default :
        return 0;
    }
}

int getWireIndicesSize(char* id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        return Fac3DDecomposer::getWireIndicesSize(id);
    case __GO_FEC__ :
        return TriangleMeshFecDataDecomposer::getWireIndicesSize(id);
    case __GO_PLOT3D__ :
        return Plot3DDecomposer::getWireIndicesSize(id);
    case __GO_POLYLINE__ :
        return PolylineDecomposer::getWireIndicesSize(id);
    default :
        return 0;
    }
}

int fillWireIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
    case __GO_FAC3D__ :
        return Fac3DDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    case __GO_FEC__ :
        return TriangleMeshFecDataDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    case __GO_PLOT3D__ :
        return Plot3DDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    case __GO_POLYLINE__ :
        return PolylineDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
    default :
        return 0;
    }
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
