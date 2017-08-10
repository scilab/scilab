/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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

#include "MeshData.hxx"
#include "DataProperties.hxx"

extern "C" {
#include <string.h>
#include "BOOL.h"

#include "graphicObjectProperties.h"
}

MeshData::MeshData(void)
{
    vertices = NULL;
    indices = NULL;
    values = NULL;

    numberVertices = 0;
    numberElements = 0;
    numberVerticesByElem = 3;
}

/* To be correctly implemented */
MeshData::MeshData(unsigned int numberVertices, unsigned int numberElements, unsigned int numberVerticesByElem)
{
    vertices = new double[numberVerticesByElem * numberVertices];
    indices = new unsigned int[numberVerticesByElem * numberElements];

    this->numberVertices = numberVertices;
    this->numberElements = numberElements;
    this->numberVerticesByElem = numberVerticesByElem;
}

/* To be correctly implemented */
MeshData::~MeshData(void)
{
    if (numberVertices > 0)
    {
        delete [] vertices;
        delete [] values;
    }

    if (numberElements > 0)
    {
        delete [] indices;
    }

}

int MeshData::getPropertyFromName(int propertyName)
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
        case __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__ :
            return NUM_VERTICES_BY_ELEM;
        default :
            return Data3D::getPropertyFromName(propertyName);
    }

}


int MeshData::setDataProperty(int property, void const* value, int numElements)
{
    switch (property)
    {
        case NUM_VERTICES :
            return setNumVertices(*((unsigned int const*) value));
        case NUM_INDICES :
            return setNumIndices(*((unsigned int const*) value));
        case X_COORDINATES :
            setDataX((double const*) value, numElements);
            break;
        case Y_COORDINATES :
            setDataY((double const*) value, numElements);
            break;
        case Z_COORDINATES :
            setDataZ((double const*) value, numElements);
            break;
        case COORDINATES :
            setVertices((double const*) value, numElements);
            break;
        case INDICES :
            setIndices((unsigned int const*) value, numElements);
            break;
        case VALUES :
            setValues((double const*) value, numElements);
            break;
        case NUM_VERTICES_BY_ELEM :
            numberVerticesByElem = *((unsigned int const*) value);
            break;
        default :
            return Data3D::setDataProperty(property, value, numElements);
    }

    return 1;
}

void MeshData::getDataProperty(int property, void **_pvData)
{
    switch (property)
    {
        case NUM_VERTICES :
            ((int *)*_pvData)[0] = getNumVertices();
            break;
        case NUM_INDICES :
            ((int *)*_pvData)[0] = getNumIndices();
            break;
        case COORDINATES :
            *_pvData = getVertices();
            break;
        case INDICES :
            *_pvData = getIndices();
            break;
        case VALUES :
            *_pvData = getValues();
            break;
        case NUM_VERTICES_BY_ELEM :
            ((int *) *_pvData)[0] = numberVerticesByElem;
            break;
        default :
            Data3D::getDataProperty(property, _pvData);
    }
}

unsigned int MeshData::getNumVertices(void)
{
    return numberVertices;
}

/*
 * Values are considered as being specified per-vertex for now
 * To be corrected
 */
int MeshData::setNumVertices(unsigned int numVertices)
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

unsigned int MeshData::getNumIndices(void)
{
    return numberElements;
}

int MeshData::setNumIndices(unsigned int numIndices)
{
    int result = 1;

    if (numIndices != this->numberElements)
    {
        unsigned int* newIndices = NULL;

        try
        {
            newIndices = new unsigned int[numberVerticesByElem * numIndices];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }

        if (result)
        {
            if (this->numberElements > 0)
            {
                delete [] indices;
            }

            indices = newIndices;

            this->numberElements = numIndices;
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

double* MeshData::getVertices(void)
{
    return vertices;
}

void MeshData::setVertices(double const* vertices, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        memcpy(this->vertices, vertices, numElements * 3 * sizeof(double));
    }
}

unsigned int* MeshData::getIndices(void)
{
    return indices;
}

void MeshData::setIndices(unsigned int const* indices, unsigned int numElements)
{
    if (numElements <= numberElements)
    {
        memcpy(this->indices, indices, numElements * numberVerticesByElem * sizeof(unsigned int));
    }
}

void MeshData::setDataX(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        for (unsigned int i = 0 ; i < numElements ; i++)
        {
            vertices[3 * i] = data[i];
        }
    }
}

void MeshData::setDataY(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        for (unsigned int i = 0 ; i < numElements ; i++)
        {
            vertices[3 * i + 1] = data[i];
        }
    }
}

void MeshData::setDataZ(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        for (unsigned int i = 0; i < numElements; i++)
        {
            vertices[3 * i + 2] = data[i];
        }
    }
}

void MeshData::setValues(double const* data, unsigned int numElements)
{
    if (numElements <= numberVertices)
    {
        memcpy(this->values, data, numElements * sizeof(double));
    }
}

double* MeshData::getValues(void)
{
    return values;
}

unsigned int MeshData::scilabIndexToIndex(unsigned int scilabIndex)
{
    return (scilabIndex - 1);
}

void MeshData::resetCoordinates(void)
{
    memset(vertices, 0, numberVertices * 3 * sizeof(double));
}

