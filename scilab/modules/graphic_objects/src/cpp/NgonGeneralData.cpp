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

#include "NgonGeneralData.hxx"
#include "DataProperties.hxx"

extern "C" {
#include "BOOL.h"

#include <string.h>

#include "graphicObjectProperties.h"
}

NgonGeneralData::NgonGeneralData(void)
{
    numGons = 0;
    numVerticesPerGon = 0;

    coordinates = NULL;

    xCoordinates = NULL;
    yCoordinates = NULL;
    zCoordinates = NULL;

    colorValues = NULL;
}

NgonGeneralData::~NgonGeneralData(void)
{
    /* To be fully implemented */

    if (numGons*numVerticesPerGon > 0)
    {
        delete [] coordinates;
    }
}

int NgonGeneralData::getPropertyFromName(char* propertyName)
{

    if (strcmp(propertyName, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__) == 0)
    {
        return NUM_ELEMENTS_ARRAY;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_COORDINATES__) == 0)
    {
        return COORDINATES;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_X__) == 0)
    {
        return X_COORDINATES;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_Y__) == 0)
    {
        return Y_COORDINATES;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_Z__) == 0)
    {
        return Z_COORDINATES;
    }
    else
    {
        NgonData::getPropertyFromName(propertyName);
    }

}

int NgonGeneralData::setDataProperty(int property, void* value, int numElements)
{

    if (property == NUM_ELEMENTS_ARRAY)
    {
        return setNumElementsArray((int*) value);
    }
    else if (property == COORDINATES)
    {
        setData((double*) value, numElements); 
    }
    else if (property == X_COORDINATES)
    {
        setDataX((double*) value, numElements);
    }
    else if (property == Y_COORDINATES)
    {
        setDataY((double*) value, numElements);
    }
    else if (property == Z_COORDINATES)
    {
        setDataZ((double*) value, numElements);
    }
    else
    {
        return NgonData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void* NgonGeneralData::getDataProperty(int property)
{
    /* Used for now to return the address of a single integer */
    static int localIntResult;

    if (property == NUM_ELEMENTS_ARRAY)
    {
          /* Not implemented yet */
    //    getNumElementsArray();
    }
    else if (property == COORDINATES)
    {
        return getData();
    }
    else if (property == X_COORDINATES)
    {
        return getDataX();
    }
    else if (property == Y_COORDINATES)
    {
        return getDataY();
    }
    else if (property == Z_COORDINATES)
    {
        return getDataZ();
    }
    else
    {
        return NgonData::getDataProperty(property);
    }

}

double* NgonGeneralData::getData()
{
    return coordinates;
}

double* NgonGeneralData::getDataX(void)
{

    return coordinates;
}

double* NgonGeneralData::getDataY(void)
{
    return &coordinates[numGons*numVerticesPerGon];
}

double* NgonGeneralData::getDataZ(void)
{
    return &coordinates[2*numGons*numVerticesPerGon];
}

/*
 * Only partially implemented
 * Must be made consistent with setNumElementsArray
 */
void NgonGeneralData::setData(double* data, int numElements)
{
    if (numElements != numGons*numVerticesPerGon)
    {
        delete [] coordinates;

        coordinates = new double[3*numElements];
    }

    for (int i = 0; i < 3*numElements; i++)
    {
        coordinates[i] = data[i];
    }
}

void NgonGeneralData::setDataX(double* data, int numElements)
{
    double* xCoordinates;

    xCoordinates = coordinates;

    for (int i = 0; i < numElements; i++)
    {
        xCoordinates[i] = data[i];
    } 

}

void NgonGeneralData::setDataY(double* data, int numElements)
{
    double* yCoordinates;

    yCoordinates = &coordinates[numGons*numVerticesPerGon];

    for (int i = 0; i < numElements; i++)
    {
        yCoordinates[i] = data[i];
    } 

}

void NgonGeneralData::setDataZ(double* data, int numElements)
{
    double* zCoordinates;


    zCoordinates = &coordinates[2*numGons*numVerticesPerGon];

    for (int i = 0; i < numElements; i++)
    {
        zCoordinates[i] = data[i];
    }

}

int NgonGeneralData::getNumElements(void)
{
    return numGons;
}

void NgonGeneralData::setNumElements(int numElements)
{
    if (numElements !=  numGons * numVerticesPerGon)
    {

        delete [] xCoordinates;
        delete [] yCoordinates;
        delete [] zCoordinates;

        xCoordinates = new double[numElements];
        yCoordinates = new double[numElements];
        zCoordinates = new double[numElements];
    }
}

int NgonGeneralData::setNumElementsArray(int* numElementsArray)
{

    if (numGons*numVerticesPerGon != numElementsArray[0]*numElementsArray[1])
    {
        double* newCoordinates;


        try {
            newCoordinates = new double[3*numElementsArray[0]*numElementsArray[1]];
        }
        catch (const std::exception& e)
        {
            return 0;
        }

        if (numGons*numVerticesPerGon > 0)
        {
            delete [] coordinates;
        }

        coordinates = newCoordinates;

    }

    numGons = numElementsArray[0];
    numVerticesPerGon = numElementsArray[1];

    return 1;
}

/* To be implemented */
double* NgonGeneralData::getColors(void)
{
    return NULL;
}

/* To be implemented */
void NgonGeneralData::setColors(double* colors, int numElements)
{

}

