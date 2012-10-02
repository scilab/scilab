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

#include "TriangleMeshFecData.hxx"
#include "TriangleMeshData.hxx"
#include "DataProperties.hxx"

extern "C" {
#include <string.h>
#include "BOOL.h"

#include "graphicObjectProperties.h"
}

TriangleMeshFecData::TriangleMeshFecData(void)
{
    vertices = NULL;
    indices = NULL;
    values = NULL;
    fecValues = NULL;

    numberVertices = 0;
    numberTriangles = 0;
}

/* To be correctly implemented */
TriangleMeshFecData::TriangleMeshFecData(unsigned int numberVertices, unsigned int numberTriangles)
{
    vertices = new double[3 * numberVertices];

    indices = new unsigned int[3 * numberTriangles];

    fecValues = new double[5 * numberTriangles];

    this->numberVertices = numberVertices;
    this->numberTriangles = numberTriangles;
}

/* To be correctly implemented */
TriangleMeshFecData::~TriangleMeshFecData(void)
{
    if (numberVertices > 0)
    {
        delete [] vertices;
        delete [] values;
    }

    if (numberTriangles > 0)
    {
        delete [] indices;
        delete [] fecValues;
    }

    numberVertices = 0;
    numberTriangles = 0;
}

int TriangleMeshFecData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
    case __GO_DATA_MODEL_NUM_INDICES__ :
        return NUM_INDICES;
    case __GO_DATA_MODEL_FEC_TRIANGLES__ :
        return FEC_TRIANGLES;
    default :
        return TriangleMeshData::getPropertyFromName(propertyName);
    }
}


int TriangleMeshFecData::setDataProperty(int property, void const* value, int numElements)
{
    if (property == NUM_INDICES)
    {
        return setNumIndices(*((unsigned int const*) value));
    }
    else if (property == FEC_TRIANGLES)
    {
        setFecTriangles((double const*) value, numElements);
    }
    else
    {
        return TriangleMeshData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void TriangleMeshFecData::getDataProperty(int property, void **_pvData)
{
    if (property == NUM_INDICES)
    {
        ((int *) *_pvData)[0] = getNumIndices();
    }
    else if (property == FEC_TRIANGLES)
    {
        *_pvData = getFecTriangles();
    }
    else
    {
        TriangleMeshData::getDataProperty(property, _pvData);
    }

}

unsigned int TriangleMeshFecData::getNumIndices(void)
{
    return numberTriangles;
}

int TriangleMeshFecData::setNumIndices(unsigned int numIndices)
{
    int result = 1;

    if (numIndices != this->numberTriangles)
    {
        unsigned int* newIndices = NULL;
        double* newFecValues = NULL;

        try
        {
            newIndices = new unsigned int[3 * numIndices];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }

        try
        {
            newFecValues = new double[5 * numIndices];
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
                delete [] fecValues;
            }

            indices = newIndices;
            fecValues = newFecValues;

            this->numberTriangles =  numIndices;
        }
        else
        {
            /* Failed allocation, nothing is set */
            if (newIndices != NULL)
            {
                delete [] newIndices;
            }

            if (newFecValues != NULL)
            {
                delete [] newFecValues;
            }
        }

    }

    return result;
}

void TriangleMeshFecData::setFecTriangles(double const* data, int numElements)
{
    if ((unsigned int)numElements > numberTriangles)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        indices[3 * i] =  scilabIndexToIndex((unsigned int) data[numElements + i]);
        indices[3 * i + 1] = scilabIndexToIndex((unsigned int) data[2 * numElements + i]);
        indices[3 * i + 2] = scilabIndexToIndex((unsigned int) data[3 * numElements + i]);

        /* Triangle number */
        fecValues[i] = data[i];

        /* Triangle vertices */
        fecValues[numElements + i] = data[numElements + i];
        fecValues[2 * numElements + i] = data[2 * numElements + i];
        fecValues[3 * numElements + i] = data[3 * numElements + i];

        /* Flag */
        fecValues[4 * numElements + i] = data[4 * numElements + i];
    }
}

double* TriangleMeshFecData::getFecTriangles(void)
{
    return fecValues;
}

