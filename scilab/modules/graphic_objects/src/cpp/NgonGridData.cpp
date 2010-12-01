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

int NgonGridData::getPropertyFromName(char* propertyName)
{
    if (strcmp(propertyName, __GO_DATA_MODEL_NUM_X__) == 0)
    {
        return NUM_X;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_NUM_Y__) == 0)
    {
        return NUM_Y;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_NUM_Z__) == 0)
    {
        return NUM_Z;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_GRID_SIZE__) == 0)
    {
        return GRID_SIZE;
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
        return NgonData::getPropertyFromName(propertyName);
    }

}

int NgonGridData::setDataProperty(int property, void* value, int numElements)
{
    if (property == GRID_SIZE)
    {
        return setGridSize((int*) value);
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

void* NgonGridData::getDataProperty(int property)
{
    /* Used for now to return the address of a single integer */
    static int localIntResult;

    if (property == NUM_X)
    {
        localIntResult = getNumX();
        return &localIntResult;
    }
    else if (property == NUM_Y)
    {
        localIntResult = getNumY();
        return &localIntResult;
    }
    else if (property == NUM_Z)
    {
        localIntResult = getNumZ();
        return &localIntResult;
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

int NgonGridData::setGridSize(int* gridSize)
{
    int xModified;
    int yModified;
    int result;

    double* newXCoordinates = NULL;
    double* newYCoordinates = NULL;
    double* newZCoordinates = NULL;

    result = 1;

    xModified = 0;
    yModified = 0;

    if (gridSize[0] != xSize)
    {
        xModified = 1;

        try
        {
            newXCoordinates = new double[gridSize[0]];
        }
        catch (const std::exception& e)
        {
            result = 0;
        }
    }

    if (gridSize[1] != ySize)
    {
        yModified = 1;

        try
        {
            newYCoordinates = new double[gridSize[1]];
        }
        catch (const std::exception& e)
        {
            result = 0;
        }
    }


    if (gridSize[0]*gridSize[1] != xSize*ySize)
    {
        try
        {
            newZCoordinates = new double[gridSize[0]*gridSize[1]];
        }
        catch (const std::exception& e)
        {
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
            xSize = gridSize[0];
        }

        if (yModified)
        {
            if (ySize > 0)
            {
                delete [] yCoordinates;
            }

            yCoordinates = newYCoordinates;
            ySize = gridSize[0];
        }

        if (xModified || yModified)
        {
            if (xSize*ySize > 0)
            {
                delete [] zCoordinates;
            }

            zCoordinates = newZCoordinates;

            numGons = (xSize-1)*(ySize-1);
        }

    }
    else
    {
        /* Failed allocation(s) */

        if (xModified && newXCoordinates != NULL)
        {
            delete [] newXCoordinates;
        }

        if (yModified && newYCoordinates != NULL)
        {
            delete [] newYCoordinates;
        }

        if ((xModified || yModified) && newZCoordinates != NULL)
        {
            delete [] newXCoordinates;
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
    return xSize*ySize;
}

void NgonGridData::setDataX(double* data, int numElements)
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

void NgonGridData::setDataY(double* data, int numElements)
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

void NgonGridData::setDataZ(double* data, int numElements)
{
    if (numElements > xSize*ySize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        zCoordinates[i] = data[i];
    }
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
