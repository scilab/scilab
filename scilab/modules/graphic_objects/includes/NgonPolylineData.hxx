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

#ifndef NGON_POLYLINE_DATA_H
#define NGON_POLYLINE_DATA_H

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

public :
    NgonPolylineData(void);

    virtual ~NgonPolylineData(void);

    int getPropertyFromName(char* propertyName);

    int setDataProperty(int property, void* value, int numElements);

    void getDataProperty(int property, void **_pvData);

    /**
     * Returns the number of elements (number of vertices making up the polyline)
     */
    int getNumElements(void);

    /**
     * Returns the z coordinates flag
     */
    int getZCoordinatesSet(void);

    /**
     * Sets the z coordinates flag
     */
    void setZCoordinatesSet(int zCoordinatesSet);

    /**
     * Returns the x coordinates shift array
     */
    double* getXCoordinatesShift(void);

    /**
     * Sets the x coordinates shift array
     * Resizes it if required.
     * @param data the x shift array (numElements values)
     * @param numElements the number of elements in the array
     */
    int setXCoordinatesShift(double* data, int numElements);

    /**
     * Returns the y coordinates shift array
     */
    double* getYCoordinatesShift();

    /**
     * Sets the y coordinates shift array
     * Resizes it if required.
     * @param data the y shift array (numElements values)
     * @param numElements the number of elements in the array
     */
    int setYCoordinatesShift(double* data, int numElements);

    /**
     * Returns the z coordinates shift array
     */
    double* getZCoordinatesShift();

    /**
     * Sets the z coordinates shift array
     * Resizes it if required.
     * @param data the z shift array (numElements values)
     * @param numElements the number of elements in the array
     */
    int setZCoordinatesShift(double* data, int numElements);

    /**
     * Returns the x coordinates shift array flag
     */
    int getXCoordinatesShiftSet(void);

    /**
     * Sets the x coordinates shift array flag
     * Deletes it if xShiftSet equal to 0
     */
    void setXCoordinatesShiftSet(int xShiftSet);

    /**
     * Returns the y coordinates shift array flag
     */
    int getYCoordinatesShiftSet(void);

    /**
     * Sets the y coordinates shift array flag
     * Deletes it if yShiftSet equal to 0
     */
    void setYCoordinatesShiftSet(int yShiftSet);

    /**
     * Returns the z coordinates shift array flag
     */
    int getZCoordinatesShiftSet(void);

    /**
     * Sets the z coordinates shift array flag
     * Deletes it if zShiftSet equal to 0
     */
    void setZCoordinatesShiftSet(int zShiftSet);

    /**
     * Sets the number of elements composing the polyline
     * (i.e. the number of vertices per gon)
     * Updates the coordinates shift arrays if required.
     * The latter probably ought to be done in one of the
     * Data set methods.
     * @param numElementsArray a 2-element array (always 1, #vertices)
     */
    int setNumElementsArray(int* numElementsArray);

    /**
     * Copies the former shift coordinates array into the new one
     * and pads with zeroes if the new number of elements exceeds
     * the former one.
     * @param newShift the new shift coordinates array
     * @param oldShift the former shift coordinates array
     */
    void copyShiftCoordinatesArray(double* newShift, double* oldShift, int numElementsNew);

    /**
     * Deletes the coordinates and shift arrays
     */
    void deleteCoordinatesArrays(void);
};

#endif
