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

#ifndef NGON_GRID_DATA_HXX
#define NGON_GRID_DATA_HXX

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
protected :

    /** The x coordinates array */
    double* xCoordinates;

    /** The y coordinates array */
    double* yCoordinates;

    /** The z coordinates array */
    double* zCoordinates;

    /**
     * The z coordinates shift value
     * It is used exclusively to modify the z-position of Grayplot and Matplot objects,
     * as their z coordinates actually correspond to data values, and must be set to 0
     * in any other case.
     */
    double zCoordinatesShift;

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
     * Returns the identifier associated to a property name
     * @param[in] propertyName the property name
     * @return the property identifier
     */
    int getPropertyFromName(int propertyName);

    /**
     * Sets a data property
     * @param[in] property the property identifier
     * @param[in] value a pointer to the property values
     * @param[in] numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    int setDataProperty(int property, void const* value, int numElements);

    /**
     * Returns a data property
     * @param[in] property the property identifier
     * @param[out] a pointer to a pointer to the returned property values
     */
    void getDataProperty(int property, void **_pvData);

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
     * @param[in] gridSize a pointer to a 4-element array: x vector (nb rows, nb cols) and y vector (nb rows, nb cols) dimensions
     * @return 1 if the property has been successfully set, 0 otherwise (failed allocation)
     */
    int setGridSize(int const* gridSize);

    /**
     * Sets the grid's x data
     * @param[in] data a pointer to the x data (numElements values)
     * @param[in] numElements the number of elements to set
     */
    void setDataX(double const* data, int numElements);

    /**
     * Sets the grid's y data
     * @param[in] data a pointer to the y data (numElements values)
     * @param[in] numElements the number of elements to set
     */
    void setDataY(double const* data, int numElements);

    /**
     * Sets the grid's z data
     * @param[in] data a pointer to the z data (numElements values)
     * @param[in] numElements the number of elements to set
     */
    void setDataZ(double const* data, int numElements);

    /**
     * Sets the grid's z coordinates shift
     * @param[in] data a pointer to the z shift value
     */
    void setZCoordinatesShift(double const* data);

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

    /**
     * Returns the grid's z coordinates shift
     * @return the z shift value
     */
    double getZCoordinatesShift(void);
};

#endif
