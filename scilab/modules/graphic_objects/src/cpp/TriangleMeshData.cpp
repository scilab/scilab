/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TriangleMeshData.hxx"
#include "DataProperties.hxx"

extern "C" {
#include <string.h>
#include "BOOL.h"

#include "graphicObjectProperties.h"
}

TriangleMeshData::TriangleMeshData(void)
{
    vertices = NULL;
    indices = NULL;
    values = NULL;

    numberVertices = 0;
    numberTriangles = 0;
}

/* To be correctly implemented */
TriangleMeshData::TriangleMeshData(unsigned int numberVertices, unsigned int numberTriangles)
{
    vertices = new double[3 * numberVertices];

    indices = new unsigned int[3 * numberTriangles];

    this->numberVertices = numberVertices;
    this->numberTriangles = numberTriangles;
}

/* To be correctly implemented */
TriangleMeshData::~TriangleMeshData(void)
{
    if (numberVertices > 0)
    {
        delete [] vertices;
        delete [] values;
    }

    if (numberTriangles > 0)
    {
        delete [] indices;
    }

}

int TriangleMeshData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
    case __GO_DATA_MODEL_NUM_VERTICES__ :
        return NUM_VERTICES;
    case __GO_DATA_MODEL_NUM_INDICES__ :
        return NUM_INDICES;
    case __GO_DATA_MODEL_X__ :
        return X_COORDINATES;
    case __GO_DATA_MODEL_Y__ :
        return Y_COORDINATES;
    case __GO_DATA_MODEL_Z__ :
        return Z_COORDINATES;
    case __GO_DATA_MODEL_COORDINATES__ :
        return COORDINATES;
    case __GO_DATA_MODEL_INDICES__ :
        return INDICES;
    case __GO_DATA_MODEL_VALUES__ :
        return VALUES;
    default :
        return Data3D::getPropertyFromName(propertyName);
    }

}


int TriangleMeshData::setDataProperty(int property, void const* value, int numElements)
{
    if (property == NUM_VERTICES)
    {
        return setNumVertices(*((unsigned int const*) value));
    }
    else if (property == NUM_INDICES)
    {
        return setNumIndices(*((unsigned int const*) value));
    }
    else if (property == X_COORDINATES)
    {
        setDataX((double const*) value, numElements);
    }
    else if (property == Y_COORDINATES)
    {
        setDataY((double const*) value, numElements);
    }
    else if (property == Z_COORDINATES)
    {
        setDataZ((double const*) value, numElements);
    }
    else if (property == COORDINATES)
    {
        setVertices((double const*) value, numElements);
    }
    else if (property == INDICES)
    {
        setIndices((unsigned int const*) value, numElements);
    }
    else if (property == VALUES)
    {
        setValues((double const*) value, numElements);
    }
    else
    {
        return Data3D::setDataProperty(property, value, numElements);
    }

    return 1;
}

void TriangleMeshData::getDataProperty(int property, void **_pvData)
{
    if (property == NUM_VERTICES)
    {
        ((int *)*_pvData)[0] = getNumVertices();
    }
    else if (property == NUM_INDICES)
    {
        ((int *)*_pvData)[0] = getNumIndices();
    }
    else if (property == COORDINATES)
    {
        *_pvData = getVertices();
    }
    else if (property == INDICES)
    {
        *_pvData = getIndices();
    }
    else if (property == VALUES)
    {
        *_pvData = getValues();
    }
    else
    {
        Data3D::getDataProperty(property, _pvData);
    }
}

unsigned int TriangleMeshData::getNumVertices(void)
{
    return numberVertices;
}

/*
 * Values are considered as being specified per-vertex for now
 * To be corrected
 */
int TriangleMeshData::setNumVertices(unsigned int numVertices)
{
    int result = 1;

    if (numVertices == 0 && numberVertices > 0)
    {
        numberVertices = 0;

        delete [] vertices;
        delete [] values;

        return 1;
    }

    if (numVertices != this->numberVertices)
    {
        double* newVertices = NULL;
        double* newValues = NULL;

        try
        {
            newVertices = new double[3 * numVertices];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }

        try
        {
            newValues = new double[numVertices];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }

        if (result)
        {
            if (this->numberVertices > 0)
            {
                delete [] vertices;
                delete [] values;
            }

            vertices = newVertices;
            values = newValues;

            this->numberVertices =  numVertices;

            resetCoordinates();
        }
        else
        {
            /* Failed allocation, nothing is set */
            if (newVertices != NULL)
            {
                delete [] newVertices;
            }

            if (newValues != NULL)
            {
                delete [] newValues;
            }
        }

    }

    return result;
}

unsigned int TriangleMeshData::getNumIndices(void)
{
    return numberTriangles;
}

int TriangleMeshData::setNumIndices(unsigned int numIndices)
{
    int result = 1;

    if (numIndices != this->numberTriangles)
    {
        unsigned int* newIndices = NULL;

        try
        {
            newIndices = new unsigned int[3 * numIndices];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }

        if (result)
        {
            if (this->numberTriangles > 0)
            {
                delete [] indices;
            }

            indices = newIndices;

            this->numberTriangles =  numIndices;
        }
        else
        {
            /* Failed allocation, nothing is set */
            if (newIndices != NULL)
            {
                delete [] newIndices;
            }
        }

    }

    return result;
}

double* TriangleMeshData::getVertices(void)
{
    return vertices;
}

void TriangleMeshData::setVertices(double const* vertices, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        memcpy(this->vertices, vertices, numElements * 3 * sizeof(double));
    }
}

unsigned int* TriangleMeshData::getIndices(void)
{
    return indices;
}

void TriangleMeshData::setIndices(unsigned int const* indices, unsigned int numElements)
{
    if (numElements <= numberTriangles)
    {
        memcpy(this->indices, indices, numElements * 3 * sizeof(unsigned int));
    }
}

void TriangleMeshData::setDataX(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        for (unsigned int i = 0 ; i < numElements ; i++)
        {
            vertices[3 * i] = data[i];
        }
    }
}

void TriangleMeshData::setDataY(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        for (unsigned int i = 0 ; i < numElements ; i++)
        {
            vertices[3 * i + 1] = data[i];
        }
    }
}

void TriangleMeshData::setDataZ(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        for (unsigned int i = 0; i < numElements; i++)
        {
            vertices[3 * i + 2] = data[i];
        }
    }
}

void TriangleMeshData::setValues(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        memcpy(this->values, data, numElements * sizeof(double));
    }
}

double* TriangleMeshData::getValues(void)
{
    return values;
}

unsigned int TriangleMeshData::scilabIndexToIndex(unsigned int scilabIndex)
{
    return (scilabIndex - 1);
}

void TriangleMeshData::resetCoordinates(void)
{
    for (unsigned int i = 0; i < numberVertices; i++)
    {
        vertices[3 * i] = 0.0;
        vertices[3 * i + 1] = 0.0;
        vertices[3 * i + 2] = 0.0;
    }
}

