/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <map>
#include "DataLoader.hxx"

#include "PolylineDecomposer.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include <string.h>

// TODO: switch using the Type

int getDataSize(char* id)
{
    char* type;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::getDataSize(id);
    }
    else
    {
        return 0;
    }

}

void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation)
{
    char* type;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        PolylineDecomposer::fillVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation);
    }
}

void fillColors(char* id, float* BUFF, int bufferLength, int elementsSize)
{
  // TODO.
}


int getIndicesSize(char* id)
{
    char* type;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::getIndicesSize(id);
    }

    return 0;
}


int fillIndices(char* id, int* buffer, int bufferLength)
{
    char* type;
    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &type);

    if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        return PolylineDecomposer::fillIndices(id, buffer, bufferLength);
    }

    return 0;
}

int getWireIndicesSize(char* id)
{
    return 0;
}

int fillWireIndices(char* id, int* BUFF, int bufferLength)
{
  // TODO.
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

} // extern "C"


