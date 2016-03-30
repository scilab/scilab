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

#ifndef NGON_GENERAL_DATA_HXX
#define NGON_GENERAL_DATA_HXX

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
    int numColorValues;

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
     * Returns the data coordinates array
     * @return a pointer to the coordinates array
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
     * @param[in] data a pointer to the data (3 * numElements values)
     * @param[in] numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setData(double const* data, int numElements);

    /**
     * Sets the x coordinates data
     * @param[in] data a pointer to the data (numElements values)
     * @param[in] numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setDataX(double const* data, int numElements);

    /**
     * Sets the y coordinates data
     * @param[in] data a pointer to the data (numElements values)
     * @param[in] numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setDataY(double const* data, int numElements);

    /**
     * Sets the z coordinates data
     * @param[in] data a pointer to the data (numElements values)
     * @param[in] numElements the number of points in the data array ( #n-gons * #vertices per n-gon)
     */
    void setDataZ(double const* data, int numElements);

    /**
     * Returns the number of elements (number of n-gons)
     * @return the number of n-gons
     */
    int getNumElements(void);

    /**
     * Sets the number of elements (number of n-gons,
     * number of vertices per n-gon, and number of color values)
     * Resizes the data coordinates array if required, must therefore
     * be called before any setData call.
     * @param[in] numElementsArray a pointer to a 3-element array (# n-gons, # vertices per n-gon, # color values)
     * @return 1 if it succeeded, 0 if the allocation failed
     */
    int setNumElementsArray(int const* numElementsArray);

    /**
     * Returns the color values array
     * @return a pointer to the color values array
     */
    double* getColors(void);

    /**
     * Sets the color values array
     * @param[in] colors a pointer to the color values array
     * @param[in] numElements the number of color values in the data array
     */
    void setColors(double const* colors, int numElements);

    /**
     * Returns the number of colors
     * @return the number of colors
     */
    int getNumColors(void);

};

#endif
