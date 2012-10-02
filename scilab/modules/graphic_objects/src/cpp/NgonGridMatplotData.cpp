/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "NgonGridMatplotData.hxx"

extern "C" {
#include <string.h>
#include <stdlib.h>

#include "graphicObjectProperties.h"
}

NgonGridMatplotData::NgonGridMatplotData(void)
{

}

NgonGridMatplotData::~NgonGridMatplotData(void)
{

}

int NgonGridMatplotData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
    case __GO_DATA_MODEL_GRID_SIZE__ :
        return GRID_SIZE;
    case __GO_DATA_MODEL_MATPLOT_BOUNDS__ :
        return MATPLOT_BOUNDS;
    case __GO_DATA_MODEL_MATPLOT_TYPE__ :
        return MATPLOT_TYPE;
    case __GO_DATA_MODEL_Z__ :
        return Z_COORDINATES;
    default :
        return NgonGridData::getPropertyFromName(propertyName);
    }

}

int NgonGridMatplotData::setDataProperty(int property, void const* value, int numElements)
{
    if (property == GRID_SIZE)
    {
        return setGridSize((int const*) value);
    }
    else if (property == MATPLOT_BOUNDS)
    {
        setBounds((double const*) value);
    }
    else if (property == Z_COORDINATES)
    {
        setDataZ((double const*) value, numElements);
    }
    else
    {
        return NgonGridData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void NgonGridMatplotData::getDataProperty(int property, void **_pvData)
{
    if (property == MATPLOT_BOUNDS)
    {
        *_pvData = getBounds();
    }
    else if (property == MATPLOT_TYPE)
    {
        ((int *) *_pvData)[0] = getType();
    }
    else if (property == Z_COORDINATES)
    {
        *_pvData = getDataZ();
    }
    else
    {
        NgonGridData::getDataProperty(property, _pvData);
    }
}

/*
 * To be done: refactoring with NgonGridData, as these two classes'
 * setGridSize methods are almost identical.
 */
int NgonGridMatplotData::setGridSize(int const* gridSize)
{
    int newXSize = 0;
    int newYSize = 0;
    int xModified = 0;
    int yModified = 0;
    int zModified = 0;
    int result = 0;
    int formerGridSize = 0;

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

    if (xSize > 0 && ySize > 0)
    {
        formerGridSize = (xSize - 1) * (ySize - 1);
    }
    else
    {
        formerGridSize = 0;
    }

    if ((newXSize - 1) * (newYSize - 1) != formerGridSize)
    {
        zModified = 1;

        try
        {
            newZCoordinates = new double[(newXSize - 1) * (newYSize - 1)];
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
            if (formerGridSize > 0)
            {
                delete [] zCoordinates;
            }

            zCoordinates = newZCoordinates;

            numGons = (xSize - 1) * (ySize - 1);

        }

        if (xModified || yModified)
        {
            computeCoordinates();
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

void NgonGridMatplotData::setBounds(double const* bounds)
{
    if (bounds == NULL)
    {
        /* Type 0 object: bounds are computed from element indices by the setGridSize method */
        type = 0;
    }
    else
    {
        /* To be done: test whether max > min */
        boundingRectangle[0] = bounds[0];
        boundingRectangle[1] = bounds[2];
        boundingRectangle[2] = bounds[1];
        boundingRectangle[3] = bounds[3];

        type = 1;
    }
}

double* NgonGridMatplotData::getBounds(void)
{
    return boundingRectangle;
}

void NgonGridMatplotData::computeCoordinates(void)
{
    if (type == 0)
    {
        for (int i = 0; i < xSize; i++)
        {
            xCoordinates[i] = 0.5 + (double) i;
        }

        for (int i = 0; i < ySize; i++)
        {
            yCoordinates[i] = 0.5 + (double) i;
        }

        boundingRectangle[0] = 0.5;
        boundingRectangle[1] = 0.5 + (double) (xSize - 1);
        boundingRectangle[2] = 0.5;
        boundingRectangle[3] = 0.5 + (double) (ySize - 1);
    }
    else
    {
        double min = boundingRectangle[0];
        double max = boundingRectangle[1];
        int numElements = 0;


        if (xSize == 1)
        {
            numElements = 1;
        }
        else
        {
            numElements = xSize - 1;
        }

        for (int i = 0; i < xSize; i++)
        {
            xCoordinates[i] = min + (double) i * (max - min) / (double) numElements;
        }

        min = boundingRectangle[2];
        max = boundingRectangle[3];

        if (ySize == 1)
        {
            numElements = 1;
        }
        else
        {
            numElements = ySize - 1;
        }

        for (int i = 0; i < ySize; i++)
        {
            yCoordinates[i] = min + (double) i * (max - min) / (double) numElements;
        }

    }

}

void NgonGridMatplotData::setDataZ(double const* data, int numElements)
{
    if (numElements > (xSize - 1) * (ySize - 1))
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        zCoordinates[i] = data[i];
    }
}

int NgonGridMatplotData::getType(void)
{
    return type;
}
