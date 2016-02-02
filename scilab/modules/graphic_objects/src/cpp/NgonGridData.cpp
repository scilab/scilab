/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
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

#include "NgonGridData.hxx"

extern "C" {
#include <string.h>
#include <stdlib.h>

#include "graphicObjectProperties.h"
}

NgonGridData::NgonGridData(void)
{
    numGons = 0;

    /* Grid: must be set to 4 */
    numVerticesPerGon = 4;

    xCoordinates = NULL;
    yCoordinates = NULL;
    zCoordinates = NULL;

    xSize = 0;
    ySize = 0;

    xDimensions[0] = 0;
    xDimensions[1] = 0;

    yDimensions[0] = 0;
    yDimensions[1] = 0;

    /* Set to 0 as a default */
    zCoordinatesShift = 0.0;
}

NgonGridData::~NgonGridData(void)
{
    if (xSize > 0)
    {
        delete [] xCoordinates;
    }

    if (ySize > 0)
    {
        delete [] yCoordinates;
    }

    if (xSize > 0 && ySize > 0)
    {
        delete [] zCoordinates;
    }
}

int NgonGridData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
        case __GO_DATA_MODEL_NUM_X__ :
            return NUM_X;
        case __GO_DATA_MODEL_NUM_Y__ :
            return NUM_Y;
        case __GO_DATA_MODEL_NUM_Z__ :
            return NUM_Z;
        case __GO_DATA_MODEL_X_DIMENSIONS__ :
            return X_DIMENSIONS;
        case __GO_DATA_MODEL_Y_DIMENSIONS__ :
            return Y_DIMENSIONS;
        case __GO_DATA_MODEL_GRID_SIZE__ :
            return GRID_SIZE;
        case __GO_DATA_MODEL_X__ :
            return X_COORDINATES;
        case __GO_DATA_MODEL_Y__ :
            return Y_COORDINATES;
        case __GO_DATA_MODEL_Z__ :
            return Z_COORDINATES;
        case __GO_DATA_MODEL_Z_COORDINATES_SHIFT__ :
            return Z_COORDINATES_SHIFT;
        default :
            return NgonData::getPropertyFromName(propertyName);
    }

}

int NgonGridData::setDataProperty(int property, void const* value, int numElements)
{
    switch (property)
    {
        case GRID_SIZE :
            return setGridSize((int const*) value);
        case X_COORDINATES :
            setDataX((double const*) value, numElements);
            break;
        case Y_COORDINATES :
            setDataY((double const*) value, numElements);
            break;
        case Z_COORDINATES :
            setDataZ((double const*) value, numElements);
            break;
        case Z_COORDINATES_SHIFT :
            setZCoordinatesShift((double const*) value);
            break;
        default :
            return NgonData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void NgonGridData::getDataProperty(int property, void **_pvData)
{
    switch (property)
    {
        case NUM_X:
            ((int *) *_pvData)[0] = getNumX();
            break;
        case NUM_Y :
            ((int *) *_pvData)[0] = getNumY();
            break;
        case NUM_Z :
            ((int *) *_pvData)[0] = getNumZ();
            break;
        case X_DIMENSIONS :
            *_pvData = getXDimensions();
            break;
        case Y_DIMENSIONS :
            *_pvData = getYDimensions();
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
        case Z_COORDINATES_SHIFT :
            ((double *) *_pvData)[0] = getZCoordinatesShift();
            break;
        default :
            NgonData::getDataProperty(property, _pvData);
    }
}

int NgonGridData::setGridSize(int const* gridSize)
{
    int newXSize = 0;
    int newYSize = 0;
    int xModified = 0;
    int yModified = 0;
    int zModified = 0;
    int result = 0;

    double* newXCoordinates = NULL;
    double* newYCoordinates = NULL;
    double* newZCoordinates = NULL;

    result = 1;

    xModified = 0;
    yModified = 0;
    zModified = 0;

    if ((gridSize[0] != 1) && (gridSize[1] != 1))
    {
        return 0;
    }

    if ((gridSize[2] != 1) && (gridSize[3] != 1))
    {
        return 0;
    }

    newXSize = gridSize[0] * gridSize[1];
    newYSize = gridSize[2] * gridSize[3];


    if (newXSize != xSize)
    {
        xModified = 1;

        try
        {
            newXCoordinates = new double[newXSize];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }
    }

    if (newYSize != ySize)
    {
        yModified = 1;

        try
        {
            newYCoordinates = new double[newYSize];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }
    }

    if (newXSize * newYSize != xSize * ySize)
    {
        zModified = 1;

        try
        {
            newZCoordinates = new double[newXSize * newYSize];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }
    }

    if (result)
    {
        if (xModified)
        {
            if (xSize > 0)
            {
                delete [] xCoordinates;
            }

            xCoordinates = newXCoordinates;
            xSize = newXSize;
        }

        xDimensions[0] = gridSize[0];
        xDimensions[1] = gridSize[1];

        if (yModified)
        {
            if (ySize > 0)
            {
                delete [] yCoordinates;
            }

            yCoordinates = newYCoordinates;
            ySize = newYSize;
        }

        yDimensions[0] = gridSize[2];
        yDimensions[1] = gridSize[3];

        if (zModified)
        {
            if (xSize * ySize > 0)
            {
                delete [] zCoordinates;
            }

            zCoordinates = newZCoordinates;

            numGons = (xSize - 1) * (ySize - 1);
        }

    }
    else
    {
        /* Failed allocation(s) */

        if (xModified && (newXCoordinates != NULL))
        {
            delete [] newXCoordinates;
        }

        if (yModified && (newYCoordinates != NULL))
        {
            delete [] newYCoordinates;
        }

        if (zModified && (newZCoordinates != NULL))
        {
            delete [] newZCoordinates;
        }

    }

    return result;
}

int NgonGridData::getNumX(void)
{
    return xSize;
}

int NgonGridData::getNumY(void)
{
    return ySize;
}

int NgonGridData::getNumZ(void)
{
    return xSize * ySize;
}

int* NgonGridData::getXDimensions(void)
{
    return xDimensions;
}

int* NgonGridData::getYDimensions(void)
{
    return yDimensions;
}

void NgonGridData::setDataX(double const* data, int numElements)
{
    if (numElements > xSize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        xCoordinates[i] = data[i];
    }
}

void NgonGridData::setDataY(double const* data, int numElements)
{
    if (numElements > ySize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        yCoordinates[i] = data[i];
    }
}

void NgonGridData::setDataZ(double const* data, int numElements)
{
    if (numElements > xSize * ySize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        zCoordinates[i] = data[i];
    }
}

void NgonGridData::setZCoordinatesShift(double const* data)
{
    zCoordinatesShift = *data;
}

double* NgonGridData::getDataX(void)
{
    return xCoordinates;
}

double* NgonGridData::getDataY(void)
{
    return yCoordinates;
}

double* NgonGridData::getDataZ(void)
{
    return zCoordinates;
}

double NgonGridData::getZCoordinatesShift(void)
{
    return zCoordinatesShift;
}
