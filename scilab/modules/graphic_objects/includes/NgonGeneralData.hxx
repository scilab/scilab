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

}

/**
 * General n-gon data class
 */

class NgonGeneralData : public NgonData
{
private :

    /** The array of color values */
    double* colorValues;

    /** The size of the color values array */
    int numColors;

protected :
    /** The coordinates array */
    double* coordinates;

public :
    /**
     * Constructor
     */
    NgonGeneralData(void);

    /**
     * Destructor
     */
    virtual ~NgonGeneralData(void);

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
    void getDataProperty(int property, void **_pvData);

    /**
     * Returns the data coordinates array
     * @return pointer to the coordinates array
     */
    double* getData();

    /**
     * Returns the data x coordinates array
     * @return a pointer to the x coordinates
     */
    double* getDataX(void);

    /**
     * Returns the data y coordinates array
     * @return a pointer to the y coordinates
     */
    double* getDataY(void);

    /**
     * Returns the data z coordinates array
     * @return a pointer to the z coordinates
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
     * Sets the number of elements (number of n-gons,
     * number of vertices per n-gon, and number of color values)
     * Resizes the data coordinates array if required, must therefore
     * be called before any setData call.
     * @param numElementsArray 3-element array (# n-gons, # vertices per n-gon, # color values)
     * @return 1 if it succeeded, 0 if the allocation failed
     */
    int setNumElementsArray(int* numElementsArray);

    /**
     * Returns the color values array
     * @return a pointer to the color values array
     */
    double* getColors(void);

    /**
     * Sets the color values array
     * @param colors the color values array
     * @param numElements the number of color values in the data array
     */
    void setColors(double* colors, int numElements);

    /**
     * Returns the number of colors
     * @return the number of colors
     */
    int getNumColors(void);

};

#endif
