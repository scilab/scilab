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

#ifndef NGON_GENERAL_DATA_H
#define NGON_GENERAL_DATA_H

#include <string>

#include "NgonData.hxx"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

/**
 * General n-gon data class
 */

class NgonGeneralData : public NgonData
{
private :


    /** The x coordinates array */
    double* xCoordinates;

    /** The y coordinates array */
    double* yCoordinates;

    /** The z coordinates array */
    double* zCoordinates;

    /** The array of color values */
    double* colorValues;

protected : 
    /** The coordinates array */
    double* coordinates;

public :
    NgonGeneralData(void);

    virtual ~NgonGeneralData(void);

    int getPropertyFromName(char* propertyName);

    int setDataProperty(int property, void* value, int numElements);

    void* getDataProperty(int property);

    /**
     * Returns the data coordinates array
     */
    double* getData();

    /**
     * Returns the data x coordinates array
     */
    double* getDataX(void);

    /**
     * Returns the data y coordinates array
     */
    double* getDataY(void);

    /**
     * Returns the data z coordinates array
     */
    double* getDataZ(void);

    /**
     * Sets the coordinates data
     * @param data the data (3 * numElements values)
     * @param numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setData(double* data, int numElements);

    /**
     * Sets the x coordinates data
     * @param data the data (numElements values)
     * @param numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setDataX(double* data, int numElements);

    /**
     * Sets the y coordinates data
     * @param data the data (numElements values)
     * @param numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setDataY(double* data, int numElements);

    /**
     * Sets the z coordinates data
     * @param data the data (numElements values)
     * @param numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setDataZ(double* data, int numElements);

    /**
     * Returns the number of elements (number of n-gons)
     */
    int getNumElements(void);

    /**
     * Sets the number of elements (number of n-gons)
     * Incorrectly implemented for now.
     * setNumElementsArray must be used instead.
     */
    void setNumElements(int numElements);

    /**
     * Sets the number of elements (number of n-gons and
     * the number of vertices per n-gon)
     * Resizes the data coordinates array if required, must therefore
     * be called any setData* call.
     * @param numElementsArray 2-element array (# n-gons, # vertices per n-gon)
     * @return 1 if it succeeded, 0 if the allocation failed
     */
    int setNumElementsArray(int* numElementsArray);

    /**
     * Returns the color values array
     * To be implemented
     */
    double* getColors(void);

    /**
     * Sets the color values array
     * To be implemented
     */
    void setColors(double* colors, int numElements);
};

#endif
