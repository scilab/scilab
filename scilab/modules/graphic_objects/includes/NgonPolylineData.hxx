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

#ifndef NGON_POLYLINE_DATA_HXX
#define NGON_POLYLINE_DATA_HXX

#include <string>

#include "NgonGeneralData.hxx"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

/**
 * A class for polyline data
 */

class NgonPolylineData : public NgonGeneralData
{
private :

    /** Specifies whether zCoordinates have been set */
    int zCoordinatesSet;

    /** The x-shift coordinates array */
    double* xShift;

    /** The y-shift coordinates array */
    double* yShift;

    /** The z-shift coordinates array */
    double* zShift;

    /** Specifies whether xShift coordinates have been set */
    int xShiftSet;

    /** Specifies whether yShift coordinates have been set */
    int yShiftSet;

    /** Specifies whether zShift coordinates have been set */
    int zShiftSet;

    /** The coordinates shift array */
    double* coordinatesShift;

    /** data of datatip display function */
    int* display_function_data;
    int display_function_data_size;

    /** The colors array */
    int* colors;
    int numColors;

public :
    /**
     * Constructor
     */
    NgonPolylineData(void);

    /**
     * Destructor
     */
    virtual ~NgonPolylineData(void);

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
     * Returns the number of elements (number of vertices making up the polyline)
     * @return the polyline's number of vertices
     */
    int getNumElements(void);

    /**
     * Returns the z coordinates flag
     * @return the z coordinates flag
     */
    int getZCoordinatesSet(void);

    /**
     * Sets the z coordinates flag
     * @param[in] zCoordinatesSet the z coordinates flag to set (1 to specify that z coordinates have been set, 0 otherwise)
     */
    void setZCoordinatesSet(int zCoordinatesSet);

    /**
     * Returns the x coordinates shift array
     * @return a pointer to the x coordinates shift array
     */
    double* getXCoordinatesShift(void);

    /**
     * Sets the x coordinates shift array
     * Resizes it if required.
     * @param[in] data a pointer to the x shift array (numElements values)
     * @param[in] numElements the number of elements in the array
     */
    int setXCoordinatesShift(double const* data, int numElements);

    /**
     * Returns the y coordinates shift array
     * @return a pointer to the y coordinates shift array
     */
    double* getYCoordinatesShift();

    /**
     * Sets the y coordinates shift array
     * Resizes it if required.
     * @param[in] data a pointer to the y shift array (numElements values)
     * @param[in] numElements the number of elements in the array
     */
    int setYCoordinatesShift(double const* data, int numElements);

    /**
     * Returns the z coordinates shift array
     * @return a pointer to the z coordinates shift array
     */
    double* getZCoordinatesShift();

    /**
     * Sets the z coordinates shift array
     * Resizes it if required.
     * @param data the z shift array (numElements values)
     * @param numElements the number of elements in the array
     */
    int setZCoordinatesShift(double const* data, int numElements);

    /**
     * Returns the x coordinates shift array flag
     * @return the x coordinates shift array flag
     */
    int getXCoordinatesShiftSet(void);

    /**
     * Sets the x coordinates shift array flag
     * Deletes it if xShiftSet equal to 0
     * @param[in] the x shift array set flag
     */
    void setXCoordinatesShiftSet(int xShiftSet);

    /**
     * Returns the y coordinates shift array flag
     * @return the y coordinates shift array flag
     */
    int getYCoordinatesShiftSet(void);

    /**
     * Sets the y coordinates shift array flag
     * Deletes it if yShiftSet equal to 0
     * @param[in] the y shift array set flag
     */
    void setYCoordinatesShiftSet(int yShiftSet);

    /**
     * Returns the z coordinates shift array flag
     * @return the z coordinates shift array flag
     */
    int getZCoordinatesShiftSet(void);

    /**
     * Sets the z coordinates shift array flag
     * Deletes it if zShiftSet equal to 0
     * @param[in] the z shift array set flag
     */
    void setZCoordinatesShiftSet(int zShiftSet);

    /**
     * Sets the number of elements composing the polyline
     * (i.e. the number of vertices per gon)
     * Updates the coordinates shift arrays if required.
     * The latter probably ought to be done in one of the
     * Data set methods.
     * @param[in] numElementsArray a pointer to a 2-element array (always 1, #vertices)
     * @return 1 if it succeeded, 0 if the allocation failed
     */
    int setNumElementsArray(int const* numElementsArray);

    /**
     * Copies the former shift coordinates array into the new one
     * and pads with zeroes if the new number of elements exceeds
     * the former one.
     * @param[in] newShift a pointer the new shift coordinates array
     * @param[in] oldShift a pointer to the former shift coordinates array
     * @param[in] numElementsNew the new array's number of elements
     */
    void copyShiftCoordinatesArray(double * newShift, double const* oldShift, int numElementsNew);

    /**
     * Sets the colors array
     * @param[in] newColors a pointer to the array with the colors
     * @param[in] numElements the colors array number of elements
     * @return 1 if it succeeded, 0 if the allocation failed
     */
    int setColors(int const* newColors, int numElements);

    /**
     * Gets the colors array
     * @return pointer to the array with the colors
     */
    int * getColors();

    /**
     * Gets the number of colors in the array
     * @return number of colors in the array
     */
    int getNumColors();

    /**
     * Deletes the coordinates and shift arrays
     */
    void deleteCoordinatesArrays(void);

    /**
     * Sets data for display function
     * @param data (numElements values)
     * @param numElements the number of elements in the array
     */
    int setDisplayFunctionData(int const* data, int numElements);
    int getDisplayFunctionDataSize();
    int* getDisplayFunctionData();
};

#endif
