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

#include <iostream>

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

    colorValues = NULL;
    numColors = 0;
}

NgonGeneralData::~NgonGeneralData(void)
{
    if (numGons * numVerticesPerGon > 0)
    {
        delete [] coordinates;
    }

    if (numColors > 0)
    {
        delete [] colorValues;
    }
}

int NgonGeneralData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
        case __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__ :
            return NUM_ELEMENTS_ARRAY;
        case __GO_DATA_MODEL_COORDINATES__ :
            return COORDINATES;
        case __GO_DATA_MODEL_X__ :
            return X_COORDINATES;
        case __GO_DATA_MODEL_Y__ :
            return Y_COORDINATES;
        case __GO_DATA_MODEL_Z__ :
            return Z_COORDINATES;
        case __GO_DATA_MODEL_COLORS__ :
            return COLORS;
        case __GO_DATA_MODEL_NUM_COLORS__ :
            return NUM_COLORS;
        default :
            return NgonData::getPropertyFromName(propertyName);
    }
}

int NgonGeneralData::setDataProperty(int property, void const* value, int numElements)
{
    switch (property)
    {
        case NUM_ELEMENTS_ARRAY :
            return setNumElementsArray((int const*) value);
        case COORDINATES :
            setData((double const*) value, numElements);
            break;
        case X_COORDINATES :
            setDataX((double const*) value, numElements);
            break;
        case Y_COORDINATES :
            setDataY((double const*) value, numElements);
            break;
        case Z_COORDINATES :
            setDataZ((double const*) value, numElements);
            break;
        case COLORS :
            setColors((double const*) value, numElements);
            break;
        default :
            return NgonData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void NgonGeneralData::getDataProperty(int property, void **_pvData)
{
    switch (property)
    {
        case NUM_ELEMENTS_ARRAY :
            /* Not implemented yet */
            //    getNumElementsArray();
            break;
        case COORDINATES :
            *_pvData = getData();
            break;
        case X_COORDINATES :
            *_pvData = getDataX();
            break;
        case Y_COORDINATES :
            *_pvData = getDataY();
            break;
        case Z_COORDINATES :
            *_pvData = getDataZ();
            break;
        case COLORS :
            *_pvData = getColors();
            break;
        case NUM_COLORS :
            ((int *) *_pvData)[0] = getNumColors();
            break;
        default :
            NgonData::getDataProperty(property, _pvData);
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
    return &coordinates[numGons * numVerticesPerGon];
}

double* NgonGeneralData::getDataZ(void)
{
    return &coordinates[2 * numGons * numVerticesPerGon];
}

/*
 * Only partially implemented
 * Must be made consistent with setNumElementsArray
 */
void NgonGeneralData::setData(double const* data, int numElements)
{
    if (numElements != numGons * numVerticesPerGon)
    {
        delete [] coordinates;

        numVerticesPerGon = numElements / numGons;
        coordinates = new double[3 * numElements];
    }

    memcpy(coordinates, data, 3 * numElements * sizeof(double));
}

void NgonGeneralData::setDataX(double const* data, int numElements)
{
    double* xCoordinates = NULL;

    xCoordinates = coordinates;
    memcpy(xCoordinates, data, numElements * sizeof(double));
}

void NgonGeneralData::setDataY(double const* data, int numElements)
{
    double* yCoordinates;

    yCoordinates = &coordinates[numGons * numVerticesPerGon];
    memcpy(yCoordinates, data, numElements * sizeof(double));
}

void NgonGeneralData::setDataZ(double const* data, int numElements)
{
    double* zCoordinates = NULL;

    zCoordinates = &coordinates[2 * numGons * numVerticesPerGon];
    memcpy(zCoordinates, data, numElements * sizeof(double));
}

int NgonGeneralData::getNumElements(void)
{
    return numGons;
}

int NgonGeneralData::setNumElementsArray(int const* numElementsArray)
{
    double* newCoordinates = NULL;
    double* newColorValues = NULL;
    int result = 1;

    /* Test whether the number of colors is valid */
    if ((numElementsArray[2] != numElementsArray[0]*numElementsArray[1]) &&
            (numElementsArray[2] != numElementsArray[0]) &&
            (numElementsArray[2] != 0))
    {
        return 0;
    }

    if (numGons * numVerticesPerGon != numElementsArray[0]*numElementsArray[1])
    {
        try
        {
            newCoordinates = new double[3 * numElementsArray[0]*numElementsArray[1]];
        }
        catch (const std::exception& e)
        {
            e.what();
            return 0;
        }

        result = 1;
    }

    if (numElementsArray[2] != this->numColors)
    {
        if (numElementsArray[2] > 0)
        {
            try
            {
                newColorValues = new double[numElementsArray[2]];
            }
            catch (const std::exception& e)
            {
                e.what();
                result = 0;
            }
        }

    }

    if (result == 1)
    {
        if (newCoordinates != NULL)
        {
            if (numGons * numVerticesPerGon > 0)
            {
                delete [] coordinates;
            }

            coordinates = newCoordinates;

            numGons = numElementsArray[0];
            numVerticesPerGon = numElementsArray[1];
        }

        if (newColorValues != NULL || numElementsArray[2] == 0)
        {
            if (this->numColors > 0)
            {
                delete [] colorValues;
            }

            colorValues = newColorValues;
            this->numColors = numElementsArray[2];
        }
    }
    else
    {
        if (newCoordinates != NULL)
        {
            delete [] newCoordinates;
        }

        if (newColorValues != NULL)
        {
            delete [] newColorValues;
        }
    }

    return result;
}

double* NgonGeneralData::getColors(void)
{
    return colorValues;
}

void NgonGeneralData::setColors(double const* colors, int numElements)
{
    if (numElements > numColors)
    {
        return;
    }
    memcpy(colorValues, colors, numElements * sizeof(double));
}

int NgonGeneralData::getNumColors(void)
{
    return numColors;
}

