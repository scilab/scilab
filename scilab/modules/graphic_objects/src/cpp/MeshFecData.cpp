/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "MeshFecData.hxx"
#include "MeshData.hxx"
#include "DataProperties.hxx"

extern "C" {
#include <string.h>
#include "BOOL.h"

#include "graphicObjectProperties.h"
}

MeshFecData::MeshFecData(void)
{
    vertices = NULL;
    indices = NULL;
    values = NULL;
    fecValues = NULL;

    numberVertices = 0;
    numberElements = 0;
    numberVerticesByElem = 3;
}

MeshFecData::MeshFecData(unsigned int numberVertices, unsigned int numberElements, unsigned int numberVerticesByElem)
{
    vertices = new double[numberVerticesByElem * numberVertices];
    indices = new unsigned int[numberVerticesByElem * numberElements];
    fecValues = new double[(numberVerticesByElem + 2) * numberElements];

    this->numberVerticesByElem = numberVerticesByElem;
    this->numberVertices = numberVertices;
    this->numberElements = numberElements;
}

MeshFecData::~MeshFecData(void)
{
    if (numberVertices > 0)
    {
        delete [] vertices;
        delete [] values;
    }

    if (numberElements > 0)
    {
        delete [] indices;
        delete [] fecValues;
    }

    numberVertices = 0;
    numberElements = 0;
}

int MeshFecData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
        case __GO_DATA_MODEL_NUM_INDICES__ :
            return NUM_INDICES;
        case __GO_DATA_MODEL_FEC_ELEMENTS__ :
            return FEC_ELEMENTS;
        default :
            return MeshData::getPropertyFromName(propertyName);
    }
}


int MeshFecData::setDataProperty(int property, void const* value, int numElements)
{
    if (property == NUM_INDICES)
    {
        return setNumIndices(*((unsigned int const*) value));
    }
    else if (property == FEC_ELEMENTS)
    {
        setFecElements((double const*) value, numElements);
    }
    else
    {
        return MeshData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void MeshFecData::getDataProperty(int property, void **_pvData)
{
    if (property == NUM_INDICES)
    {
        ((int *) *_pvData)[0] = getNumIndices();
    }
    else if (property == FEC_ELEMENTS)
    {
        *_pvData = getFecElements();
    }
    else
    {
        MeshData::getDataProperty(property, _pvData);
    }

}

unsigned int MeshFecData::getNumIndices(void)
{
    return numberElements;
}

int MeshFecData::setNumIndices(unsigned int numIndices)
{
    int result = 1;
    if (numIndices != this->numberElements)
    {
        unsigned int* newIndices = NULL;
        double* newFecValues = NULL;

        try
        {
            newIndices = new unsigned int[numberVerticesByElem * numIndices];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }

        try
        {
            newFecValues = new double[(numberVerticesByElem + 2) * numIndices];
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
                delete [] fecValues;
            }

            indices = newIndices;
            fecValues = newFecValues;

            this->numberElements =  numIndices;
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

void MeshFecData::setFecElements(double const* data, int numElements)
{
    if ((unsigned int)numElements > numberElements)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
	for (unsigned j = 0; j < numberVerticesByElem; ++j)
	{
	    indices[numberVerticesByElem * i + j] =  scilabIndexToIndex((unsigned int) data[(j + 1) * numElements + i]);
	}

        /* Element number */
        fecValues[i] = data[i];

        /* Element vertices */
	for (unsigned int j = 1; j <= numberVerticesByElem; ++j)
	{
	    fecValues[j * numElements + i] = data[j * numElements + i];
	}

        /* Flag */
	fecValues[(numberVerticesByElem + 1) * numElements + i] = data[(numberVerticesByElem + 1) * numElements + i];
    }
}

double* MeshFecData::getFecElements(void)
{
    return fecValues;
}

