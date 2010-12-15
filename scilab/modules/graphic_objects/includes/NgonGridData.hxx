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

#ifndef NGON_GRID_DATA_H
#define NGON_GRID_DATA_H

#include <string>

#include "NgonData.hxx"

extern "C" {
#include "BOOL.h"
}

/**
 * N-gons grid data class
 */

class NgonGridData : public NgonData
{
private :

    /** The x coordinates array */
    double* xCoordinates;

    /** The y coordinates array */
    double* yCoordinates;

    /** The z coordinates array */
    double* zCoordinates;

    /** The grid x size */
    int xSize;

    /** The grid y size */
    int ySize;

    /**
     * The grid x vector dimensions (rows, columns)
     * One of them must be equal to 1 and the other one to xSize.
     */
    int xDimensions[2];

    /**
     * The grid y vector dimensions (rows, colums)
     * One of them must be equal to 1 and the other one to ySize.
     */
    int yDimensions[2];

public :

    /**
     * Constructor
     */
    NgonGridData(void);

    /**
     * Destructor
     */
    ~NgonGridData(void);

    /**
     * Returns the value corresponding to a property name
     * @param propertyName the property name
     * @return the property value
     */
    int getPropertyFromName(char* propertyName);

    /**
     * Sets a data property
     * @param property the property value
     * @param value pointer to the property
     * @param numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    int setDataProperty(int property, void* value, int numElements);

    /**
     * Returns a data property
     * @param property the property value
     * @return a pointer to the property
     */
    void* getDataProperty(int property);

    /**
     * Returns the grid's x size
     * @return the grid's x size
     */
    int getNumX(void);

    /**
     * Returns the grid's y size
     * @return the grid's y size
     */
    int getNumY(void);

    /**
     * Returns the grid's z size
     * @return the grid's number of elements
     */
    int getNumZ(void);

    /**
     * Returns the grid's x vector dimensions
     * @return a pointer to a 2-element array (nb rows, nb columns)
     */
    int* getXDimensions(void);

    /**
     * Returns the grid's y vector dimensions
     * @return a pointer to a 2-element array (nb rows, nb columns)
     */
    int* getYDimensions(void);

    /**
     * Sets the grid's x and y vectors dimensions
     * Resizes the x, y, and z data coordinates arrays if required
     * and must therefore be called prior to any setData call
     * @param gridSize 4-element array: x vector (nb rows, nb cols) and y vector (nb rows, nb cols) dimensions
     * @return 1 if the property has been successfully set, 0 otherwise (failed allocation)
     */
    int setGridSize(int* gridSize);

    /**
     * Sets the grid's x data
     * @param data the data (numElements values)
     * @param numElements the number of elements to set
     */
    void setDataX(double* data, int numElements);

    /**
     * Sets the grid's y data
     * @param data the data (numElements values)
     * @param numElements the number of elements to set
     */
    void setDataY(double* data, int numElements);

    /**
     * Sets the grid's z data
     * @param data the data (numElements values)
     * @param numElements the number of elements to set
     */
    void setDataZ(double* data, int numElements);

    /**
     * Returns the grid's x data
     * @return a pointer to the grid's x data
     */
    double* getDataX(void);

    /**
     * Returns the grid's y data
     * @return a pointer to the grid's y data
     */
    double* getDataY(void);

    /**
     * Returns the grid's z data
     * @return a pointer to the grid's z data
     */
    double* getDataZ(void);
};

#endif
