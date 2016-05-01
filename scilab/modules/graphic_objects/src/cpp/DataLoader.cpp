/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
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

#include <cstring>

#include "DataLoader.hxx"

#include "MatPlotDecomposer.hxx"
#include "Fac3DDecomposer.hxx"
#include "NgonGridMatplotData.hxx"
#include "NgonGridGrayplotDataDecomposer.hxx"
#include "NgonGridMatplotDataDecomposer.hxx"
#include "Plot3DDecomposer.hxx"
#include "PolylineDecomposer.hxx"
#include "MeshFecDataDecomposer.hxx"
#include "NormalGenerator.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int getTextureWidth(int id)
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

int getTextureHeight(int id)
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

int fillTextureData(int id, unsigned char* buffer, int bufferLength)
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

int fillSubTextureData(int id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height)
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

int getDataSize(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FAC3D__ :
            return Fac3DDecomposer::getDataSize(id);
        case __GO_FEC__ :
            return MeshFecDataDecomposer::getDataSize(id);
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

void fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
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
            MeshFecDataDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask);
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


void fillNormals(int id, float* position, float* buffer, int bufferLength, int elementsSize)
{

    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FAC3D__ :
        {
            int numVerticesPerGon = 0;
            int* piNumVerticesPerGon = &numVerticesPerGon;
            getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**) &piNumVerticesPerGon);
            CalculatePolygonNormalFlat(position, buffer, bufferLength, elementsSize, numVerticesPerGon);
        }
        break;
        case __GO_FEC__ :
            break;
        case __GO_GRAYPLOT__ :
            break;
        case __GO_MATPLOT__ :
            break;
        case __GO_PLOT3D__ :
        {
            //CalculateGridNormalFlat(position, buffer, bufferLength, elementsSize);
            int numX = 0;
            int* piNumX = &numX;
            int numY = 0;
            int* piNumY = &numY;

            getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
            getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);
            CalculateGridNormalSmooth(position, buffer, bufferLength, elementsSize, numX, numY);
        }
        break;
        case __GO_POLYLINE__ :
            break;
    }
}

void fillTextureCoordinates(int id, float* BUFF, int bufferLength)
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
            MeshFecDataDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
            break;
        case __GO_POLYLINE__ :
            PolylineDecomposer::fillTextureCoordinates(id, BUFF, bufferLength);
            break;
    }
}

void fillColors(int id, float* BUFF, int bufferLength, int elementsSize)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FEC__ :
            MeshFecDataDecomposer::fillColors(id, BUFF, bufferLength, elementsSize);
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


int getIndicesSize(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FAC3D__ :
            return Fac3DDecomposer::getIndicesSize(id);
        case __GO_FEC__ :
            return MeshFecDataDecomposer::getIndicesSize(id);
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


int fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FAC3D__ :
            return Fac3DDecomposer::fillIndices(id, buffer, bufferLength, logMask);
        case __GO_FEC__ :
            return MeshFecDataDecomposer::fillIndices(id, buffer, bufferLength, logMask);
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

int getWireIndicesSize(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FAC3D__ :
            return Fac3DDecomposer::getWireIndicesSize(id);
        case __GO_FEC__ :
            return MeshFecDataDecomposer::getWireIndicesSize(id);
        case __GO_PLOT3D__ :
            return Plot3DDecomposer::getWireIndicesSize(id);
        case __GO_POLYLINE__ :
            return PolylineDecomposer::getWireIndicesSize(id);
        default :
            return 0;
    }
}

int fillWireIndices(int id, int* buffer, int bufferLength, int logMask)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    switch (iType)
    {
        case __GO_FAC3D__ :
            return Fac3DDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
        case __GO_FEC__ :
            return MeshFecDataDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
        case __GO_PLOT3D__ :
            return Plot3DDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
        case __GO_POLYLINE__ :
            return PolylineDecomposer::fillWireIndices(id, buffer, bufferLength, logMask);
        default :
            return 0;
    }
}

int getMarkIndicesSize(int id)
{
    return 0;
}

int fillMarkIndices(int id, int* BUFF, int bufferLength)
{
    // TODO.
    return 0;
}

JavaDirectBuffer getTextureData(int id)
{
    JavaDirectBuffer info;
    int iType = 0;
    int *piType = &iType;

    info.address = NULL;
    info.size = 0;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    if (iType == __GO_MATPLOT__)
    {
        void * address = NULL;
        unsigned int size = 0;

        if (!MatPlotDecomposer::getTextureData(id, &address, &size))
        {
            return info;
        }
        info.address = address;
        info.size = size;
    }

    return info;
}

int getTextureImageType(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    if (iType == __GO_MATPLOT__)
    {
        return MatPlotDecomposer::getTextureImageType(id);
    }

    return -1;
}

int getTextureDataType(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    if (iType == __GO_MATPLOT__)
    {
        return MatPlotDecomposer::getTextureDataType(id);
    }

    return -1;
}

int getTextureGLType(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    if (iType == __GO_MATPLOT__)
    {
        return MatPlotDecomposer::getTextureGLType(id);
    }

    return -1;
}

void disposeTextureData(int id, unsigned char * buffer)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    if (iType == __GO_MATPLOT__)
    {
        MatPlotDecomposer::disposeTextureData(id, buffer);
    }
}

int isTextureRowOrder(int id)
{
    int iType = 0;
    int *piType = &iType;

    getGraphicObjectProperty(id, __GO_TYPE__, jni_int, (void**) &piType);

    if (iType == __GO_MATPLOT__)
    {
        return MatPlotDecomposer::isTextureRowOrder(id);
    }

    return 0;
}

void setABGRExt(int isAvailable)
{
    NgonGridMatplotData::setABGRSupported(isAvailable != 0);
}
