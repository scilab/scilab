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

#ifndef NGON_GRID_MATPLOT_DATA_HXX
#define NGON_GRID_MATPLOT_DATA_HXX

#include <string>

#include "NgonGridData.hxx"

extern "C" {
#include "BOOL.h"
}

/**
 * N-gons grid Matplot data class
 * The xDimensions and yDimensions properties inherited from NgonGridData have no
 * useful purpose, as the x and y coordinates arrays are not currently visible to
 * the user.
 * To be done: refactoring
 */

class NgonGridMatplotData : public NgonGridData
{
private :

    /** The 2D bounding rectangle {xmin, xmax, ymin, ymax} */
    double boundingRectangle[4];

    /**
     * Matplot type
     * Specifies whether x and y coordinates are computed directly from element indices (0)
     * or computed using the 2D bounding rectangle (1)
     */
    int type;

protected :

    /**
     * Computes the x and y coordinates depending on the type member
     * If type is equal to 0, x and y coordinates are directly computed from matrix indices
     * and the rectangle bounds are updated
     * else, if equal to 1, the coordinates are computed by interpolating between the bounding
     * rectangle's min and max vertices
     */
    void computeCoordinates(void);

public :

    /**
     * Constructor
     */
    NgonGridMatplotData(void);

    /**
     * Destructor
     */
    ~NgonGridMatplotData(void);

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
     * Sets the grid's x and y vectors dimensions
     * Resizes the x, y, and z data coordinates arrays if required
     * and must therefore be called prior to any setData call
     * Similar to NgonGridData's setGridSize method, the main difference being how the z data array's size
     * is computed.
     * To be done: refactoring
     * @param[in] gridSize a pointer to a 4-element array: x vector (nb rows, nb cols) and y vector (nb rows, nb cols) dimensions
     * @return 1 if the property has been successfully set, 0 otherwise (failed allocation)
     */
    int setGridSize(int const* gridSize);

    /**
     * Sets the 2D bounding rectangle's coordinates
     * Also sets the internal property type according to the value of its argument
     * To be done: use the same coordinate ordering as the internal array
     * @param[in] a pointer to a 4-element array: {xmin, ymin, xmax, ymax} bounds. If bounds is NULL, type is set to 0, else, it is set to 1
     */
    void setBounds(double const* bounds);

    /**
     * Returns the 2D bounding rectangle's coordinates
     * @return a pointer to the bounding rectangle coordinates array {xmin, xmax, ymin, ymax}
     */
    double* getBounds(void);

    /**
     * Returns the Matplot type
     * @return the Matplot type value
     */
    int getType(void);

    /**
     * Sets the grid's z data
     * This method is almost identical to NgonGridData's own setDataZ method
     * the only difference being how the maximum number of elements is computed
     * To be done: refactoring
     * @param[in] data a pointer to the z data (numElements values)
     * @param[in] numElements the number of elements to set
     */
    void setDataZ(double const* data, int numElements);
};

#endif
